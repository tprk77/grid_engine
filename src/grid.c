// Copyright (c) 2019 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#include "grid_engine/grid.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "SDL2/SDL.h"

const ge_gfx_opts_t GE_GFX_OPTS_DEFAULTS = {
    .pixel_multiplier = 8,
    .window_name = "Grid Engine",
};

typedef struct ge_grid {
  size_t width;
  size_t height;
  const uint8_t* pixel_arr;
  ge_gfx_opts_t* gfx_opts;
  bool has_window;
  SDL_Window* sdl_window;
  SDL_Surface* sdl_surface;
  bool should_quit;
} ge_grid_t;

static void abort_on_null(const void* ptr);
static void log_line(const char* restrict format, ...);

ge_grid_t* ge_grid_new(size_t width, size_t height, const uint8_t* restrict pixel_arr)
{
  ge_grid_t* grid = calloc(1, sizeof(ge_grid_t));
  grid->width = width;
  grid->height = height;
  grid->pixel_arr = pixel_arr;
  grid->gfx_opts = calloc(1, sizeof(ge_gfx_opts_t));
  *grid->gfx_opts = GE_GFX_OPTS_DEFAULTS;
  grid->has_window = false;
  grid->sdl_window = NULL;
  grid->sdl_surface = NULL;
  grid->should_quit = false;
  return grid;
}

void ge_grid_free(const ge_grid_t* restrict grid)
{
  if (grid == NULL) {
    return;
  }
  memset((void*) grid, 0, sizeof(ge_grid_t));
  free((void*) grid);
}

ge_error_t ge_grid_set_data(ge_grid_t* restrict grid, size_t width, size_t height,
                            const uint8_t* restrict pixel_arr)
{
  abort_on_null(grid);
  abort_on_null(pixel_arr);
  grid->width = width;
  grid->height = height;
  grid->pixel_arr = pixel_arr;
  return GE_OK;
}

ge_error_t ge_grid_set_gfx_opts(ge_grid_t* restrict grid, const ge_gfx_opts_t* restrict gfx_opts)
{
  abort_on_null(grid);
  abort_on_null(gfx_opts);
  memset(grid->gfx_opts, 0, sizeof(ge_gfx_opts_t));
  grid->gfx_opts->pixel_multiplier = gfx_opts->pixel_multiplier;
  return GE_OK;
}

ge_error_t ge_grid_window_create(ge_grid_t* restrict grid)
{
  abort_on_null(grid);
  if (grid->has_window) {
    return GE_ERROR_ALREADY_HAS_WINDOW;
  }
  log_line("Grid engine window being created!\n");
  // Find the right window size
  size_t window_width = grid->width * grid->gfx_opts->pixel_multiplier;
  size_t window_height = grid->height * grid->gfx_opts->pixel_multiplier;
  // TODO More options need to go into the struct, etc
  grid->sdl_window = SDL_CreateWindow(grid->gfx_opts->window_name, 100, 100, window_width,
                                      window_height, SDL_WINDOW_SHOWN);
  grid->sdl_surface = SDL_GetWindowSurface(grid->sdl_window);
  grid->has_window = true;
  return GE_OK;
}

ge_error_t ge_grid_window_destroy(ge_grid_t* restrict grid)
{
  abort_on_null(grid);
  if (!grid->has_window) {
    return GE_ERROR_NO_WINDOW;
  }
  log_line("Grid engine window being destroyed!\n");
  SDL_DestroyWindow(grid->sdl_window);
  grid->sdl_window = NULL;
  grid->sdl_surface = NULL;
  grid->has_window = false;
  return GE_OK;
}

static void putpixel(SDL_Surface* surface, int x, int y, Uint32 pixel)
{
  int bpp = surface->format->BytesPerPixel;
  Uint8* p = (Uint8*) surface->pixels + y * surface->pitch + x * bpp;
  switch (bpp) {
  case 1:
    *p = pixel;
    break;
  case 2:
    *(Uint16*) p = pixel;
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
    *(Uint32*) p = pixel;
    break;
  }
}

ge_error_t ge_grid_window_redraw(ge_grid_t* restrict grid)
{
  abort_on_null(grid);
  if (!grid->has_window) {
    return GE_ERROR_NO_WINDOW;
  }
  for (size_t i = 0; i < grid->width; i++) {
    for (size_t j = 0; j < grid->height; j++) {
      // Compute the color of the grid pixel
      uint8_t pixel_value = grid->pixel_arr[j * grid->width + i];
      uint32_t pixel_color =
          SDL_MapRGBA(grid->sdl_surface->format, pixel_value, pixel_value, pixel_value, 255);
      // Set the actual surface pixels
      const size_t xo = i * grid->gfx_opts->pixel_multiplier;
      const size_t yo = j * grid->gfx_opts->pixel_multiplier;
      for (size_t xd = 0; xd < grid->gfx_opts->pixel_multiplier; ++xd) {
        for (size_t yd = 0; yd < grid->gfx_opts->pixel_multiplier; ++yd) {
          putpixel(grid->sdl_surface, xo + xd, yo + yd, pixel_color);
        }
      }
    }
  }
  SDL_UpdateWindowSurface(grid->sdl_window);
  return GE_OK;
}

static void abort_on_null(const void* ptr)
{
  if (ptr == NULL) {
    abort();
  }
}

static void log_line(const char* restrict format, ...)
{
  va_list vlist;
  va_start(vlist, format);
  vfprintf(stderr, format, vlist);
  va_end(vlist);
}
