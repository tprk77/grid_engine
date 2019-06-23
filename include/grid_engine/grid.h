// Copyright (c) 2019 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#ifndef GE_GRID_H_
#define GE_GRID_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "grid_engine/error.h"

/**
 * A struct representing a grid with graphics, etc.
 */
typedef struct ge_grid ge_grid_t;

typedef struct ge_gfx_opts {
  size_t pixel_multiplier;
  // TODO
  // bool is_fullscreen;
  // bool is_borderless_window;
  const char* window_name;
} ge_gfx_opts_t;

extern const ge_gfx_opts_t GE_GFX_OPTS_DEFAULTS;

ge_grid_t* ge_grid_new(size_t width, size_t height, const uint8_t* restrict pixel_arr);
void ge_grid_free(const ge_grid_t* restrict grid);
ge_error_t ge_grid_set_data(ge_grid_t* restrict grid, size_t width, size_t height,
                            const uint8_t* restrict pixel_arr);
ge_error_t ge_grid_set_gfx_opts(ge_grid_t* restrict grid, const ge_gfx_opts_t* restrict gfx_opts);
ge_error_t ge_grid_window_create(ge_grid_t* restrict grid);
ge_error_t ge_grid_window_destroy(ge_grid_t* restrict grid);
ge_error_t ge_grid_window_redraw(ge_grid_t* restrict grid);

#endif  // GE_GRID_H_
