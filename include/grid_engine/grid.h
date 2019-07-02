// Copyright (c) 2019 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#ifndef GE_GRID_H_
#define GE_GRID_H_

#include <stddef.h>
#include <stdint.h>

#include "grid_engine/coord.h"

#define GE_MAX_NUM_NEIGHBORS 8

typedef struct ge_grid {
  size_t width;
  size_t height;
  uint8_t* restrict pixel_arr;
} ge_grid_t;

typedef struct ge_neighbor_res {
  size_t num_neighbors;
  ge_coord_t neighbors[GE_MAX_NUM_NEIGHBORS];
} ge_neighbor_res_t;

bool ge_grid_has_coord(const ge_grid_t* restrict grid, ge_coord_t coord);
uint8_t ge_grid_get_coord(const ge_grid_t* restrict grid, ge_coord_t coord);
void ge_grid_set_coord(ge_grid_t* restrict grid, ge_coord_t coord, uint8_t value);
uint8_t ge_grid_get_coord_wrapped(const ge_grid_t* restrict grid, ge_coord_t offset);
void ge_grid_set_coord_wrapped(ge_grid_t* restrict grid, ge_coord_t offset, uint8_t value);
ge_neighbor_res_t ge_grid_get_neighbors(const ge_grid_t* restrict grid, ge_coord_t coord);
ge_neighbor_res_t ge_grid_get_neighbors_wrapped(const ge_grid_t* restrict grid, ge_coord_t coord);

#endif  // GE_GRID_H_
