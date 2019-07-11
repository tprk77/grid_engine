// Copyright (c) 2019 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#include "grid_engine/engine.h"

#include "SDL2/SDL.h"

#include "grid_engine/log.h"

static void abort_on_null(const void* ptr);
static void putpixel(SDL_Surface* surface, int x, int y, Uint32 pixel);

typedef struct ge_engine {
  bool inited;
  const ge_grid_t* grid;
  ge_gfx_opts_t gfx_opts;
  bool has_window;
  SDL_Window* sdl_window;
  SDL_Surface* sdl_surface;
  bool should_quit;
} ge_engine_t;

// clang-format off
#define GE_ENGINE_DEFAULTS_K { \
    .inited = false, \
    .grid = NULL, \
    .gfx_opts = GE_GFX_OPTS_DEFAULTS_K, \
    .has_window = false, \
    .sdl_window = NULL, \
    .sdl_surface = NULL, \
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

ge_error_t ge_set_grid(const ge_grid_t* restrict grid)
{
  abort_on_null(grid);
  if (!ge_engine.inited) {
    return GE_ERROR_NOT_INITED;
  }
  ge_engine.grid = grid;
  return GE_OK;
}

ge_error_t ge_set_gfx_opts(const ge_gfx_opts_t* restrict gfx_opts)
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
    // GE_ERROR_NOT_INITED
    return FALLBACK_PIXEL_MULTIPLIER;
  }
  else if (!ge_engine.grid) {
    // GE_ERROR_NO_GRID_SET
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
  GE_LOG_INFO("Grid engine window being created!");
  // Find the right window size
  const size_t width = ge_grid_get_width(ge_engine.grid);
  const size_t height = ge_grid_get_height(ge_engine.grid);
  const size_t window_width = width * ge_engine.gfx_opts.pixel_multiplier;
  const size_t window_height = height * ge_engine.gfx_opts.pixel_multiplier;
  // TODO Check window size with actual desktop size
  ge_engine.sdl_window =
      SDL_CreateWindow(ge_engine.gfx_opts.window_name, GE_SCREEN_BORDER, GE_SCREEN_BORDER,
                       window_width, window_height, SDL_WINDOW_SHOWN);
  if (ge_engine.sdl_window == NULL) {
    return GE_ERROR_CREATE_WINDOW;
  }
  ge_engine.sdl_surface = SDL_GetWindowSurface(ge_engine.sdl_window);
  if (ge_engine.sdl_surface == NULL) {
    // Clean up the window we just made
    SDL_DestroyWindow(ge_engine.sdl_window);
    ge_engine.sdl_window = NULL;
    return GE_ERROR_WINDOW_SURFACE;
  }
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
  SDL_DestroyWindow(ge_engine.sdl_window);
  ge_engine.sdl_window = NULL;
  ge_engine.sdl_surface = NULL;
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
  for (size_t ii = 0; ii < width; ii++) {
    for (size_t jj = 0; jj < height; jj++) {
      // Compute the color of the grid pixel
      const uint8_t pixel_value = pixel_arr[jj * width + ii];
      const uint32_t pixel_color =
          SDL_MapRGBA(ge_engine.sdl_surface->format, pixel_value, pixel_value, pixel_value, 255);
      // Set the actual surface pixels
      const size_t xo = ii * ge_engine.gfx_opts.pixel_multiplier;
      const size_t yo = jj * ge_engine.gfx_opts.pixel_multiplier;
      for (size_t xd = 0; xd < ge_engine.gfx_opts.pixel_multiplier; ++xd) {
        for (size_t yd = 0; yd < ge_engine.gfx_opts.pixel_multiplier; ++yd) {
          putpixel(ge_engine.sdl_surface, xo + xd, yo + yd, pixel_color);
        }
      }
    }
  }
  if (SDL_UpdateWindowSurface(ge_engine.sdl_window) != 0) {
    return GE_ERROR_UPDATE_SURFACE;
  }
  return GE_OK;
}

bool ge_poll_events(ge_event_t* restrict event)
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
    abort();
  }
}

static void putpixel(SDL_Surface* restrict surface, int x, int y, uint32_t pixel)
{
  const int bpp = surface->format->BytesPerPixel;
  uint8_t* p = (uint8_t*) surface->pixels + y * surface->pitch + x * bpp;
  switch (bpp) {
  case 1:
    *p = pixel;
    break;
  case 2:
    *(uint16_t*) p = pixel;
    break;
  case 3:
    if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
      p[0] = (pixel >> 16) & 0xff;
      p[1] = (pixel >> 8) & 0xff;
      p[2] = pixel & 0xff;
    }
    else {
      p[0] = pixel & 0xff;
      p[1] = (pixel >> 8) & 0xff;
      p[2] = (pixel >> 16) & 0xff;
    }
    break;
  case 4:
    *(uint32_t*) p = pixel;
    break;
  }
}
