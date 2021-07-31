// Copyright (c) 2019 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#ifndef GE_GRID_H_
#define GE_GRID_H_

#include <stddef.h>
#include <stdint.h>

#include "grid_engine/coord.h"
#include "grid_engine/neighbors.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ge_grid ge_grid_t;

ge_grid_t* ge_grid_create(size_t width, size_t height);
void ge_grid_free(ge_grid_t* grid);
size_t ge_grid_get_width(const ge_grid_t* grid);
size_t ge_grid_get_height(const ge_grid_t* grid);
const uint8_t* ge_grid_get_pixel_arr(const ge_grid_t* grid);
uint8_t* ge_grid_get_pixel_arr_mut(ge_grid_t* grid);
void ge_grid_copy_pixel_arr(ge_grid_t* grid, const ge_grid_t* other);
void ge_grid_clear_pixel_arr(ge_grid_t* grid);
bool ge_grid_has_coord(const ge_grid_t* grid, ge_coord_t coord);
uint8_t ge_grid_get_coord(const ge_grid_t* grid, ge_coord_t coord);
void ge_grid_set_coord(ge_grid_t* grid, ge_coord_t coord, uint8_t value);
uint8_t ge_grid_get_coord_wrapped(const ge_grid_t* grid, ge_coord_t coord);
void ge_grid_set_coord_wrapped(ge_grid_t* grid, ge_coord_t coord, uint8_t value);
ge_neighbors_t ge_grid_get_neighbors(const ge_grid_t* grid, ge_coord_t coord);
ge_neighbors_t ge_grid_get_neighbors_wrapped(const ge_grid_t* grid, ge_coord_t coord);

#ifdef __cplusplus
}
#endif

#endif  // GE_GRID_H_
