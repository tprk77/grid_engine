// Copyright (c) 2021 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#include "grid_engine/mz_grid.h"

#include <stdlib.h>

#include "grid_engine/bitset.h"
#include "grid_engine/grid.h"
#include "grid_engine/log.h"

typedef struct ge_mz_grid {
  ge_grid_t* logic_grid;
  ge_grid_t* render_grid;
  ge_bitset_t* edge_bitset;
} ge_mz_grid_t;

const ge_mz_con_t GE_MZ_CONS[GE_MZ_NUM_CONS] = {
    GE_MZ_CON_NORTH,
    GE_MZ_CON_EAST,
    GE_MZ_CON_SOUTH,
    GE_MZ_CON_WEST,
};

#define GE_MZ_CON_BIT_MASK UINT8_C(0x0F)
#define GE_MZ_CON_BIT_OFFSET 0
#define GE_MZ_CON_NORTH_BIT ((UINT8_C(1) << GE_MZ_CON_NORTH) << GE_MZ_CON_BIT_OFFSET)
#define GE_MZ_CON_EAST_BIT ((UINT8_C(1) << GE_MZ_CON_EAST) << GE_MZ_CON_BIT_OFFSET)
#define GE_MZ_CON_SOUTH_BIT ((UINT8_C(1) << GE_MZ_CON_SOUTH) << GE_MZ_CON_BIT_OFFSET)
#define GE_MZ_CON_WEST_BIT ((UINT8_C(1) << GE_MZ_CON_WEST) << GE_MZ_CON_BIT_OFFSET)

static const uint8_t GE_MZ_CON_TO_BITS[GE_MZ_NUM_CONS] = {
    GE_MZ_CON_NORTH_BIT,
    GE_MZ_CON_EAST_BIT,
    GE_MZ_CON_SOUTH_BIT,
    GE_MZ_CON_WEST_BIT,
};

static const ge_mz_con_t GE_MZ_CON_TO_OPPOSITE[GE_MZ_NUM_CONS] = {
    GE_MZ_CON_SOUTH,
    GE_MZ_CON_WEST,
    GE_MZ_CON_NORTH,
    GE_MZ_CON_EAST,
};

static const ge_direction_t GE_MZ_CON_TO_DIRECTION[GE_MZ_NUM_CONS] = {
    GE_DIRECTION_NORTH,
    GE_DIRECTION_EAST,
    GE_DIRECTION_SOUTH,
    GE_DIRECTION_WEST,
};

const ge_mz_path_t GE_MZ_PATHS[GE_MZ_NUM_PATHS] = {
    GE_MZ_PATH_UNVISITED,
    GE_MZ_PATH_PATHED,
    GE_MZ_PATH_EDGE,
    GE_MZ_PATH_VISITED,
};

#define GE_MZ_PATH_BIT_MASK UINT8_C(0x30)
#define GE_MZ_PATH_BIT_OFFSET 4
#define GE_MZ_PATH_UNVISITED_BITS (UINT8_C(GE_MZ_PATH_UNVISITED) << GE_MZ_PATH_BIT_OFFSET)
#define GE_MZ_PATH_PATHED_BITS (UINT8_C(GE_MZ_PATH_PATHED) << GE_MZ_PATH_BIT_OFFSET)
#define GE_MZ_PATH_EDGE_BITS (UINT8_C(GE_MZ_PATH_EDGE) << GE_MZ_PATH_BIT_OFFSET)
#define GE_MZ_PATH_VISITED_BITS (UINT8_C(GE_MZ_PATH_VISITED) << GE_MZ_PATH_BIT_OFFSET)

static const uint8_t GE_MZ_PATH_TO_BITS[GE_MZ_NUM_PATHS] = {
    GE_MZ_PATH_UNVISITED_BITS,
    GE_MZ_PATH_PATHED_BITS,
    GE_MZ_PATH_EDGE_BITS,
    GE_MZ_PATH_VISITED_BITS,
};

uint8_t ge_mz_con_to_bits(ge_mz_con_t con)
{
  return GE_MZ_CON_TO_BITS[con];
}

ge_mz_con_t ge_mz_con_get_opposite(ge_mz_con_t con)
{
  return GE_MZ_CON_TO_OPPOSITE[con];
}

ge_direction_t ge_mz_con_get_direction(ge_mz_con_t con)
{
  return GE_MZ_CON_TO_DIRECTION[con];
}

uint8_t ge_mz_value_set_con(uint8_t value, ge_mz_con_t con)
{
  if (con == GE_MZ_CON_NONE) {
    return value & ~GE_MZ_CON_BIT_MASK;
  }
  if (con == GE_MZ_CON_ALL) {
    return value | GE_MZ_CON_BIT_MASK;
  }
  return (value & ~GE_MZ_CON_BIT_MASK) | GE_MZ_CON_TO_BITS[con];
}

uint8_t ge_mz_value_add_con(uint8_t value, ge_mz_con_t con)
{
  if (con == GE_MZ_CON_NONE) {
    return value;
  }
  if (con == GE_MZ_CON_ALL) {
    return value | GE_MZ_CON_BIT_MASK;
  }
  return value | GE_MZ_CON_TO_BITS[con];
}

uint8_t ge_mz_value_rm_con(uint8_t value, ge_mz_con_t con)
{
  if (con == GE_MZ_CON_NONE) {
    return value;
  }
  if (con == GE_MZ_CON_ALL) {
    return value & ~GE_MZ_CON_BIT_MASK;
  }
  return value & ~GE_MZ_CON_TO_BITS[con];
}

bool ge_mz_value_has_con(uint8_t value, ge_mz_con_t con)
{
  if (con == GE_MZ_CON_NONE) {
    return (value & GE_MZ_CON_BIT_MASK) == 0;
  }
  if (con == GE_MZ_CON_ALL) {
    return (value & GE_MZ_CON_BIT_MASK) == GE_MZ_CON_BIT_MASK;
  }
  return value & GE_MZ_CON_TO_BITS[con];
}

uint8_t ge_mz_path_to_bits(ge_mz_path_t path)
{
  return GE_MZ_PATH_TO_BITS[path];
}

uint8_t ge_mz_value_set_path(uint8_t value, ge_mz_path_t path)
{
  return (value & ~GE_MZ_PATH_BIT_MASK) | GE_MZ_PATH_TO_BITS[path];
}

bool ge_mz_value_is_path(uint8_t value, ge_mz_path_t path)
{
  return (value & GE_MZ_PATH_BIT_MASK) == GE_MZ_PATH_TO_BITS[path];
}

ge_mz_grid_t* ge_mz_grid_create(size_t width, size_t height)
{
  ge_mz_grid_t* grid = calloc(1, sizeof(ge_mz_grid_t));
  if (grid == NULL) {
    return NULL;
  }
  grid->logic_grid = ge_grid_create(width, height);
  if (grid->logic_grid == NULL) {
    free(grid);
    return NULL;
  }
  const size_t render_width = width * 2 + 1;
  const size_t render_height = height * 2 + 1;
  grid->render_grid = ge_grid_create(render_width, render_height);
  if (grid->render_grid == NULL) {
    ge_grid_free(grid->logic_grid);
    free(grid);
    return NULL;
  }
  grid->edge_bitset = ge_bitset_create(width * height);
  if (grid->edge_bitset == NULL) {
    ge_grid_free(grid->logic_grid);
    ge_grid_free(grid->render_grid);
    free(grid);
    return NULL;
  }
  // Render the cells of the maze (assuming all unconnected)
  for (size_t jj = 1; jj < render_height; jj += 2) {
    for (size_t ii = 1; ii < render_width; ii += 2) {
      ge_grid_set_coord(grid->render_grid, (ge_coord_t){ii, jj}, 255);
    }
  }
  return grid;
}

void ge_mz_grid_free(ge_mz_grid_t* grid)
{
  if (grid == NULL) {
    return;
  }
  ge_grid_free(grid->logic_grid);
  ge_grid_free(grid->render_grid);
  ge_bitset_free(grid->edge_bitset);
  free(grid);
}

ge_grid_t* ge_mz_grid_get_render_grid(ge_mz_grid_t* grid)
{
  return grid->render_grid;
}

size_t ge_mz_grid_get_width(const ge_mz_grid_t* grid)
{
  return ge_grid_get_width(grid->logic_grid);
}

size_t ge_mz_grid_get_height(const ge_mz_grid_t* grid)
{
  return ge_grid_get_height(grid->logic_grid);
}

bool ge_mz_grid_has_coord(const ge_mz_grid_t* grid, ge_coord_t coord)
{
  return ge_grid_has_coord(grid->logic_grid, coord);
}

uint8_t ge_mz_grid_get_coord(const ge_mz_grid_t* grid, ge_coord_t coord)
{
  return ge_grid_get_coord(grid->logic_grid, coord);
}

bool ge_mz_grid_has_con_at_coord(const ge_mz_grid_t* grid, ge_coord_t coord, ge_mz_con_t con)
{
  const uint8_t value = ge_grid_get_coord(grid->logic_grid, coord);
  return ge_mz_value_has_con(value, con);
}

bool ge_mz_grid_is_path_at_coord(const ge_mz_grid_t* grid, ge_coord_t coord, ge_mz_path_t path)
{
  const uint8_t value = ge_grid_get_coord(grid->logic_grid, coord);
  return ge_mz_value_is_path(value, path);
}

void ge_mz_grid_set_coord(ge_mz_grid_t* grid, ge_coord_t coord, uint8_t value)
{
  // Update the logical grid
  const uint8_t prv_value = ge_grid_get_coord(grid->logic_grid, coord);
  ge_grid_set_coord(grid->logic_grid, coord, value);
  for (size_t ii = 0; ii < GE_MZ_NUM_CONS; ++ii) {
    const ge_mz_con_t con = GE_MZ_CONS[ii];
    const bool prv_con_exists = ge_mz_value_has_con(prv_value, con);
    const bool cur_con_exists = ge_mz_value_has_con(value, con);
    const bool added_con = !prv_con_exists && cur_con_exists;
    const bool removed_con = prv_con_exists && !cur_con_exists;
    if (!added_con && !removed_con) {
      continue;
    }
    const ge_coord_t nbr_offset = ge_direction_get_offset(GE_MZ_CON_TO_DIRECTION[con]);
    const ge_coord_t nbr_coord = ge_coord_add(coord, nbr_offset);
    if (!ge_grid_has_coord(grid->logic_grid, nbr_coord)) {
      continue;
    }
    const uint8_t nbr_value = ge_grid_get_coord(grid->logic_grid, nbr_coord);
    const ge_mz_con_t ops_con = GE_MZ_CON_TO_OPPOSITE[con];
    const uint8_t nxt_value = (added_con ? ge_mz_value_add_con(nbr_value, ops_con)
                                         : ge_mz_value_rm_con(nbr_value, ops_con));
    ge_grid_set_coord(grid->logic_grid, nbr_coord, nxt_value);
  }
  // Update the edge bitset
  const bool prv_edge = ge_mz_value_is_path(prv_value, GE_MZ_PATH_EDGE);
  const bool cur_edge = ge_mz_value_is_path(value, GE_MZ_PATH_EDGE);
  if (prv_edge != cur_edge) {
    const size_t width = ge_grid_get_width(grid->logic_grid);
    const size_t bitset_index = width * coord.y + coord.x;
    ge_bitset_set(grid->edge_bitset, bitset_index, cur_edge);
  }
  // Update the rendered grid
  const ge_coord_t render_coord = {2 * coord.x + 1, 2 * coord.y + 1};
  for (size_t ii = 0; ii < GE_MZ_NUM_CONS; ++ii) {
    const ge_mz_con_t con = GE_MZ_CONS[ii];
    const ge_coord_t nbr_offset = ge_direction_get_offset(GE_MZ_CON_TO_DIRECTION[con]);
    const ge_coord_t nbr_coord = ge_coord_add(render_coord, nbr_offset);
    const uint8_t nbr_value = (ge_mz_value_has_con(value, con) ? 255 : 0);
    ge_grid_set_coord(grid->render_grid, nbr_coord, nbr_value);
  }
}

void ge_mz_grid_set_con_at_coord(ge_mz_grid_t* grid, ge_coord_t coord, ge_mz_con_t con)
{
  const uint8_t prv_value = ge_grid_get_coord(grid->logic_grid, coord);
  const uint8_t nxt_value = ge_mz_value_set_con(prv_value, con);
  ge_mz_grid_set_coord(grid, coord, nxt_value);
}

void ge_mz_grid_add_con_at_coord(ge_mz_grid_t* grid, ge_coord_t coord, ge_mz_con_t con)
{
  const uint8_t prv_value = ge_grid_get_coord(grid->logic_grid, coord);
  const uint8_t nxt_value = ge_mz_value_add_con(prv_value, con);
  ge_mz_grid_set_coord(grid, coord, nxt_value);
}

void ge_mz_grid_rm_con_at_coord(ge_mz_grid_t* grid, ge_coord_t coord, ge_mz_con_t con)
{
  const uint8_t prv_value = ge_grid_get_coord(grid->logic_grid, coord);
  const uint8_t nxt_value = ge_mz_value_rm_con(prv_value, con);
  ge_mz_grid_set_coord(grid, coord, nxt_value);
}

void ge_mz_grid_set_coord_set_path(ge_mz_grid_t* grid, ge_coord_t coord, ge_mz_path_t path)
{
  const uint8_t prv_value = ge_grid_get_coord(grid->logic_grid, coord);
  const uint8_t nxt_value = ge_mz_value_set_path(prv_value, path);
  ge_mz_grid_set_coord(grid, coord, nxt_value);
}

ge_neighbors_t ge_mz_grid_get_neighbors(const ge_mz_grid_t* grid, ge_coord_t coord)
{
  return ge_grid_get_neighbors(grid->logic_grid, coord);
}

ge_neighbors_t ge_mz_grid_get_neighbors_connected(const ge_mz_grid_t* grid, ge_coord_t coord)
{
  const uint8_t value = ge_grid_get_coord(grid->logic_grid, coord);
  const ge_neighbors_t nbrs = ge_grid_get_neighbors(grid->logic_grid, coord);
  ge_neighbors_t connected_nbrs = GE_NEIGHBORS_DEFAULTS;
  for (size_t ii = 0; ii < GE_MZ_NUM_CONS; ++ii) {
    const ge_mz_con_t con = GE_MZ_CONS[ii];
    const ge_direction_t direction = GE_MZ_CON_TO_DIRECTION[con];
    // Ignore invalid neighbors, even if connected
    if (!ge_neighbors_has_neighbor(&nbrs, direction)) {
      continue;
    }
    // Copy connected neighbors to the result
    if (ge_mz_value_has_con(value, con)) {
      connected_nbrs.neighbors[direction] = nbrs.neighbors[direction];
    }
  }
  return connected_nbrs;
}

ge_coord_vec_t* ge_mz_grid_get_edge_coords(const ge_mz_grid_t* grid)
{
  const size_t width = ge_grid_get_width(grid->logic_grid);
  ge_coord_vec_t* edge_coords = ge_coord_vec_create();
  size_t edge_index = GE_BITSET_SEARCH_INIT;
  while ((edge_index = ge_bitset_search(grid->edge_bitset, edge_index)) != GE_BITSET_SEARCH_INIT) {
    ge_coord_vec_push_back(edge_coords, (ge_coord_t){edge_index % width, edge_index / width});
  }
  return edge_coords;
}

ge_coord_t ge_mz_grid_next_edge_coord(const ge_mz_grid_t* grid, ge_coord_t start_coord)
{
  const size_t width = ge_grid_get_width(grid->logic_grid);
  const size_t start_edge_index =
      (!ge_coord_is_invalid(start_coord) ? width * start_coord.y + start_coord.x
                                         : GE_BITSET_SEARCH_INIT);
  const size_t edge_index = ge_bitset_search(grid->edge_bitset, start_edge_index);
  return (edge_index != GE_BITSET_SEARCH_INIT ? (ge_coord_t){edge_index % width, edge_index / width}
                                              : GE_INVALID_COORD);
}
