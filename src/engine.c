// Copyright (c) 2019 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#include "grid_engine/engine.h"

#include "SDL2/SDL.h"

#include "grid_engine/log.h"

static void abort_on_null(const void* ptr);
static uint32_t pixel_rbga(uint8_t pixel_value);
static void destroy_engine_sdl();

typedef struct ge_engine {
  bool inited;
  const ge_grid_t* grid;
  ge_gfx_opts_t gfx_opts;
  bool has_window;
  SDL_Window* sdl_window;
  SDL_Renderer* sdl_renderer;
  SDL_Texture* sdl_texture;
  bool should_quit;
} ge_engine_t;

// clang-format off
#define GE_ENGINE_DEFAULTS_K { \
    .inited = false, \
    .grid = NULL, \
    .gfx_opts = GE_GFX_OPTS_DEFAULTS_K, \
    .has_window = false, \
    .sdl_window = NULL, \
    .should_quit = false, \
}
// clang-format on

const ge_gfx_opts_t GE_GFX_OPTS_DEFAULTS = GE_GFX_OPTS_DEFAULTS_K;
static const ge_engine_t GE_ENGINE_DEFAULTS = GE_ENGINE_DEFAULTS_K;

// TODO Calculating this from a ratio might be better?
static const size_t GE_SCREEN_BORDER = 100;

static ge_engine_t ge_engine = GE_ENGINE_DEFAULTS_K;

ge_error_t ge_init(void)
{
  if (ge_engine.inited) {
    return GE_ERROR_ALREADY_INITED;
  }
  GE_LOG_INFO("Grid engine initializing!");
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    return GE_ERROR_ENGINE_INIT;
  }
  ge_engine = GE_ENGINE_DEFAULTS;
  ge_engine.inited = true;
  return GE_OK;
}

void ge_quit(void)
{
  if (!ge_engine.inited) {
    return;
  }
  GE_LOG_INFO("Grid engine is exiting!");
  SDL_Quit();
  ge_engine.inited = false;
}

ge_error_t ge_set_grid(const ge_grid_t* grid)
{
  abort_on_null(grid);
  if (!ge_engine.inited) {
    return GE_ERROR_NOT_INITED;
  }
  ge_engine.grid = grid;
  return GE_OK;
}

ge_error_t ge_set_gfx_opts(const ge_gfx_opts_t* gfx_opts)
{
  abort_on_null(gfx_opts);
  if (!ge_engine.inited) {
    return GE_ERROR_NOT_INITED;
  }
  ge_engine.gfx_opts = *gfx_opts;
  return GE_OK;
}

size_t ge_auto_detect_pixel_multiplier(void)
{
  const size_t FALLBACK_PIXEL_MULTIPLIER = 1;
  if (!ge_engine.inited) {
    return FALLBACK_PIXEL_MULTIPLIER;
  }
  else if (ge_engine.grid == NULL) {
    return FALLBACK_PIXEL_MULTIPLIER;
  }
  SDL_Rect disp_rect;
  if (SDL_GetDisplayBounds(0, &disp_rect) != 0) {
    return FALLBACK_PIXEL_MULTIPLIER;
  }
  // Check minimum screen size
  if ((size_t) disp_rect.w < 3 * GE_SCREEN_BORDER || (size_t) disp_rect.h < 3 * GE_SCREEN_BORDER) {
    return FALLBACK_PIXEL_MULTIPLIER;
  }
  // Essentially the size of the screen divided by the number of grid pixels
  const size_t grid_width = ge_grid_get_width(ge_engine.grid);
  const size_t grid_height = ge_grid_get_height(ge_engine.grid);
  const size_t width_px_mul = (disp_rect.w - 2 * GE_SCREEN_BORDER) / grid_width;
  const size_t height_px_mul = (disp_rect.h - 2 * GE_SCREEN_BORDER) / grid_height;
  const size_t min_px_mul = (width_px_mul < height_px_mul ? width_px_mul : height_px_mul);
  return (min_px_mul != 0 ? min_px_mul : 1);
}

ge_error_t ge_create_window()
{
  if (!ge_engine.inited) {
    return GE_ERROR_NOT_INITED;
  }
  else if (ge_engine.has_window) {
    return GE_ERROR_ALREADY_HAS_WINDOW;
  }
  else if (ge_engine.grid == NULL) {
    return GE_ERROR_NO_GRID_SET;
  }
  GE_LOG_INFO("Grid engine window being created!");
  const size_t width = ge_grid_get_width(ge_engine.grid);
  const size_t height = ge_grid_get_height(ge_engine.grid);
  const size_t window_width = width * ge_engine.gfx_opts.pixel_multiplier;
  const size_t window_height = height * ge_engine.gfx_opts.pixel_multiplier;
  ge_engine.sdl_window = SDL_CreateWindow(ge_engine.gfx_opts.window_name, SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED, window_width, window_height, 0);
  if (ge_engine.sdl_window == NULL) {
    return GE_ERROR_CREATE_WINDOW;
  }
  ge_engine.sdl_renderer = SDL_CreateRenderer(ge_engine.sdl_window, -1, 0);
  if (ge_engine.sdl_renderer == NULL) {
    destroy_engine_sdl();
    return GE_ERROR_CREATE_WINDOW;
  }
  if (SDL_RenderSetLogicalSize(ge_engine.sdl_renderer, width, height) != 0) {
    destroy_engine_sdl();
    return GE_ERROR_CREATE_WINDOW;
  }
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
  ge_engine.sdl_texture = SDL_CreateTexture(ge_engine.sdl_renderer, SDL_PIXELFORMAT_RGBA8888,
                                            SDL_TEXTUREACCESS_STREAMING, width, height);
  if (ge_engine.sdl_texture == NULL) {
    destroy_engine_sdl();
    return GE_ERROR_CREATE_WINDOW;
  }
  SDL_SetRenderDrawColor(ge_engine.sdl_renderer, 0, 0, 0, 255);
  if (SDL_RenderClear(ge_engine.sdl_renderer) != 0) {
    destroy_engine_sdl();
    return GE_ERROR_CREATE_WINDOW;
  }
  SDL_RenderPresent(ge_engine.sdl_renderer);
  ge_engine.has_window = true;
  return GE_OK;
}

ge_error_t ge_destroy_window()
{
  if (!ge_engine.inited) {
    return GE_ERROR_NOT_INITED;
  }
  else if (!ge_engine.has_window) {
    return GE_ERROR_NO_WINDOW;
  }
  GE_LOG_INFO("Grid engine window being destroyed!");
  destroy_engine_sdl();
  ge_engine.has_window = false;
  return GE_OK;
}

ge_error_t ge_redraw_window()
{
  if (!ge_engine.inited) {
    return GE_ERROR_NOT_INITED;
  }
  else if (!ge_engine.has_window) {
    return GE_ERROR_NO_WINDOW;
  }
  const size_t width = ge_grid_get_width(ge_engine.grid);
  const size_t height = ge_grid_get_height(ge_engine.grid);
  const uint8_t* const pixel_arr = ge_grid_get_pixel_arr(ge_engine.grid);
  void* tex_pixel_arr_raw = NULL;
  int tex_pitch_b = 0;
  if (SDL_LockTexture(ge_engine.sdl_texture, NULL, &tex_pixel_arr_raw, &tex_pitch_b) != 0) {
    return GE_ERROR_DRAWING;
  }
  uint8_t* const tex_pixel_arr = tex_pixel_arr_raw;
  for (size_t jj = 0; jj < height; jj++) {
    uint32_t* const tex_pixel_row = (uint32_t*) &tex_pixel_arr[tex_pitch_b * jj];
    for (size_t ii = 0; ii < width; ii++) {
      uint32_t* const tex_pixel = &tex_pixel_row[ii];
      const uint8_t pixel_value = pixel_arr[jj * width + ii];
      *tex_pixel = pixel_rbga(pixel_value);
    }
  }
  SDL_UnlockTexture(ge_engine.sdl_texture);
  if (SDL_RenderClear(ge_engine.sdl_renderer) != 0) {
    return GE_ERROR_DRAWING;
  }
  if (SDL_RenderCopy(ge_engine.sdl_renderer, ge_engine.sdl_texture, NULL, NULL) != 0) {
    return GE_ERROR_DRAWING;
  }
  SDL_RenderPresent(ge_engine.sdl_renderer);
  return GE_OK;
}

bool ge_poll_events(ge_event_t* event)
{
  if (!ge_engine.inited) {
    return false;
  }
  SDL_Event sdl_event;
  while (SDL_PollEvent(&sdl_event)) {
    if (sdl_event.type == SDL_QUIT) {
      // Handle quiting, but this isn't a GE event
      GE_LOG_INFO("Grid engine going to quit!");
      ge_engine.should_quit = true;
    }
    else if (ge_fill_event(event, &sdl_event)) {
      // If this is a GE event, we are done
      return true;
    }
  }
  return false;
}

bool ge_should_quit(void)
{
  return ge_engine.should_quit;
}

uint32_t ge_get_time_ms(void)
{
  return SDL_GetTicks();
}

void ge_sleep_ms(uint32_t duration_ms)
{
  SDL_Delay(duration_ms);
}

static void abort_on_null(const void* ptr)
{
  if (ptr == NULL) {
    GE_LOG_ERROR("Null pointer!");
    abort();
  }
}

#if SDL_BYTEORDER == SDL_LIL_ENDIAN
#define RGBA_R_SHIFT 24
#define RGBA_B_SHIFT 16
#define RGBA_G_SHIFT 8
#define RGBA_A_SHIFT 0
#else
#define RGBA_R_SHIFT 0
#define RGBA_B_SHIFT 8
#define RGBA_G_SHIFT 16
#define RGBA_A_SHIFT 24
#endif

static uint32_t pixel_rbga(uint8_t pixel_value)
{
  // Assumes texture format is SDL_PIXELFORMAT_RGBA8888
  return ((pixel_value << RGBA_R_SHIFT) | (pixel_value << RGBA_G_SHIFT)
          | (pixel_value << RGBA_B_SHIFT) | (255 << RGBA_A_SHIFT));
}

#undef RGBA_R_SHIFT
#undef RGBA_G_SHIFT
#undef RGBA_B_SHIFT
#undef RGBA_A_SHIFT

static void destroy_engine_sdl()
{
  if (ge_engine.sdl_texture != NULL) {
    SDL_DestroyTexture(ge_engine.sdl_texture);
  }
  if (ge_engine.sdl_renderer != NULL) {
    SDL_DestroyRenderer(ge_engine.sdl_renderer);
  }
  if (ge_engine.sdl_window != NULL) {
    SDL_DestroyWindow(ge_engine.sdl_window);
  }
  ge_engine.sdl_texture = NULL;
  ge_engine.sdl_renderer = NULL;
  ge_engine.sdl_window = NULL;
}
