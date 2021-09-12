// Copyright (c) 2021 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#ifndef GE_MZ_GRID_H_
#define GE_MZ_GRID_H_

#include <stddef.h>
#include <stdint.h>

#include "grid_engine/coord.h"
#include "grid_engine/coord_vec.h"
#include "grid_engine/grid.h"
#include "grid_engine/nbrs.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GE_MZ_NUM_CONS 4

typedef enum ge_mz_con {
  GE_MZ_CON_NORTH = 0,
  GE_MZ_CON_EAST,
  GE_MZ_CON_SOUTH,
  GE_MZ_CON_WEST,
  GE_MZ_CON_NONE,  // Special value to indicate no connections
  GE_MZ_CON_ALL,   // Special value to indicate all connections
} ge_mz_con_t;

extern const ge_mz_con_t GE_MZ_CONS[GE_MZ_NUM_CONS];

// Take advantage of the enum layout to make this as simple as possible
#define GE_MZ_FOR_ALL_CONS(CON) for (ge_mz_con_t CON = 0; CON < GE_MZ_NUM_CONS; ++CON)

uint8_t ge_mz_con_to_bits(ge_mz_con_t con);
ge_mz_con_t ge_mz_con_get_opposite(ge_mz_con_t con);
ge_dir_t ge_mz_con_get_dir(ge_mz_con_t con);

uint8_t ge_mz_value_set_con(uint8_t value, ge_mz_con_t con);
uint8_t ge_mz_value_add_con(uint8_t value, ge_mz_con_t con);
uint8_t ge_mz_value_rm_con(uint8_t value, ge_mz_con_t con);
bool ge_mz_value_has_con(uint8_t value, ge_mz_con_t con);

#define GE_MZ_NUM_PATHS 4

typedef enum ge_mz_path {
  GE_MZ_PATH_UNVISITED = 0,
  GE_MZ_PATH_PATHED,
  GE_MZ_PATH_EDGE,
  GE_MZ_PATH_VISITED,
} ge_mz_path_t;

extern const ge_mz_path_t GE_MZ_PATHS[GE_MZ_NUM_PATHS];

// Take advantage of the enum layout to make this as simple as possible
#define GE_MZ_FOR_ALL_PATHS(PATH) for (ge_mz_path_t PATH = 0; PATH < GE_MZ_NUM_PATHS; ++PATHS)

uint8_t ge_mz_path_to_bits(ge_mz_path_t path);

uint8_t ge_mz_value_set_path(uint8_t value, ge_mz_path_t path);
bool ge_mz_value_is_path(uint8_t value, ge_mz_path_t path);

typedef struct ge_mz_grid ge_mz_grid_t;

ge_mz_grid_t* ge_mz_grid_create(size_t width, size_t height);
void ge_mz_grid_free(ge_mz_grid_t* grid);
ge_grid_t* ge_mz_grid_get_render_grid(ge_mz_grid_t* grid);
size_t ge_mz_grid_get_width(const ge_mz_grid_t* grid);
size_t ge_mz_grid_get_height(const ge_mz_grid_t* grid);
bool ge_mz_grid_has_coord(const ge_mz_grid_t* grid, ge_coord_t coord);
uint8_t ge_mz_grid_get_coord(const ge_mz_grid_t* grid, ge_coord_t coord);
bool ge_mz_grid_has_con_at_coord(const ge_mz_grid_t* grid, ge_coord_t coord, ge_mz_con_t con);
bool ge_mz_grid_is_path_at_coord(const ge_mz_grid_t* grid, ge_coord_t coord, ge_mz_path_t path);
void ge_mz_grid_set_coord(ge_mz_grid_t* grid, ge_coord_t coord, uint8_t value);
void ge_mz_grid_set_con_at_coord(ge_mz_grid_t* grid, ge_coord_t coord, ge_mz_con_t con);
void ge_mz_grid_add_con_at_coord(ge_mz_grid_t* grid, ge_coord_t coord, ge_mz_con_t con);
void ge_mz_grid_rm_con_at_coord(ge_mz_grid_t* grid, ge_coord_t coord, ge_mz_con_t con);
void ge_mz_grid_set_coord_set_path(ge_mz_grid_t* grid, ge_coord_t coord, ge_mz_path_t path);
ge_nbrs_t ge_mz_grid_get_nbrs(const ge_mz_grid_t* grid, ge_coord_t coord);
ge_nbrs_t ge_mz_grid_get_nbrs_connected(const ge_mz_grid_t* grid, ge_coord_t coord);
ge_coord_vec_t* ge_mz_grid_get_edge_coords(const ge_mz_grid_t* grid);
ge_coord_t ge_mz_grid_next_edge_coord(const ge_mz_grid_t* grid, ge_coord_t start_coord);

#ifdef __cplusplus
}
#endif

#endif  // GE_MZ_GRID_H_
