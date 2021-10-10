// Copyright (c) 2019 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#include "grid_engine/img.h"

#include <SDL2/SDL_image.h>

#include "grid_engine/grid.h"
#include "grid_engine/log.h"
#include "grid_engine/palette.h"

static ge_color_t pixel_rbga(uint32_t tex_pixel);

ge_grid_t* ge_img_load(const char* filename)
{
  SDL_Surface* const sdl_load_surface = IMG_Load(filename);
  if (sdl_load_surface == NULL) {
    GE_LOG_ERROR("Grid engine failed to load image!");
    GE_LOG_ERROR("SDL image error: %s", IMG_GetError());
    return NULL;
  }
  // Convert to a predetermined format
  SDL_Surface* const sdl_surface =
      SDL_ConvertSurfaceFormat(sdl_load_surface, SDL_PIXELFORMAT_RGBA8888, 0);
  SDL_FreeSurface(sdl_load_surface);
  if (sdl_surface == NULL) {
    return NULL;
  }
  // Convert the surface to a grid, converting to grayscale, ignoring alpha
  ge_grid_t* const grid = ge_grid_create(sdl_surface->w, sdl_surface->h);
  if (grid == NULL) {
    SDL_FreeSurface(sdl_surface);
    return NULL;
  }
  uint8_t* const tex_pixel_arr = sdl_surface->pixels;
  for (size_t jj = 0; jj < (size_t) sdl_surface->h; ++jj) {
    uint32_t* const tex_pixel_row = (uint32_t*) &tex_pixel_arr[sdl_surface->pitch * jj];
    for (size_t ii = 0; ii < (size_t) sdl_surface->w; ++ii) {
      uint32_t* const tex_pixel = &tex_pixel_row[ii];
      const ge_color_t pixel_color = pixel_rbga(*tex_pixel);
      // Convert pixel to grayscale using Rec. 709 coefficients (round by adding 0.5)
      const uint8_t pixel_value = (0.2126f * pixel_color.red + 0.7152f * pixel_color.green
                                   + 0.0722f * pixel_color.blue + 0.5f);
      ge_grid_set_coord(grid, (ge_coord_t){ii, jj}, pixel_value);
    }
  }
  SDL_FreeSurface(sdl_surface);
  return grid;
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

static ge_color_t pixel_rbga(uint32_t tex_pixel)
{
  // Assumes texture format is SDL_PIXELFORMAT_RGBA8888. The conversion does not
  // handle alpha, and completely ignores it in the conversion.
  ge_color_t pixel_color;
  pixel_color.red = (tex_pixel >> RGBA_R_SHIFT) & 0xFF;
  pixel_color.green = (tex_pixel >> RGBA_G_SHIFT) & 0xFF;
  pixel_color.blue = (tex_pixel >> RGBA_B_SHIFT) & 0xFF;
  return pixel_color;
}

#undef RGBA_R_SHIFT
#undef RGBA_G_SHIFT
#undef RGBA_B_SHIFT
#undef RGBA_A_SHIFT
