// Copyright (c) 2019 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#include "grid_engine/grid.h"

#include <stdlib.h>

static void abort_on_out_of_bounds(const ge_grid_t* restrict grid, ge_coord_t coord);

static const ge_coord_t GE_NEIGHBOR_OFFSETS[GE_MAX_NUM_NEIGHBORS] = {
    {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1},
};

static const ge_neighbor_res_t GE_NEIGHBOR_RES_DEFAULTS = {
    .num_neighbors = 0,
    .neighbors = {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}},
};

bool ge_grid_has_coord(const ge_grid_t* restrict grid, ge_coord_t coord)
{
  return (coord.x > 0 && coord.x < (ptrdiff_t) grid->width && coord.y > 0
          && coord.y < (ptrdiff_t) grid->height);
}

uint8_t ge_grid_get_coord(const ge_grid_t* restrict grid, ge_coord_t coord)
{
  abort_on_out_of_bounds(grid, coord);
  return grid->pixel_arr[grid->width * coord.y + coord.x];
}

void ge_grid_set_coord(ge_grid_t* restrict grid, ge_coord_t coord, uint8_t value)
{
  abort_on_out_of_bounds(grid, coord);
  grid->pixel_arr[grid->width * coord.y + coord.x] = value;
}

uint8_t ge_grid_get_coord_wrapped(const ge_grid_t* restrict grid, ge_coord_t coord)
{
  coord = ge_coord_wrap(coord, grid->width, grid->height);
  return ge_grid_get_coord(grid, coord);
}

void ge_grid_set_coord_wrapped(ge_grid_t* restrict grid, ge_coord_t coord, uint8_t value)
{
  coord = ge_coord_wrap(coord, grid->width, grid->height);
  ge_grid_set_coord(grid, coord, value);
}

ge_neighbor_res_t ge_grid_get_neighbors(const ge_grid_t* restrict grid, ge_coord_t coord)
{
  ge_neighbor_res_t result = GE_NEIGHBOR_RES_DEFAULTS;
  for (size_t ii = 0; ii < GE_MAX_NUM_NEIGHBORS; ++ii) {
    ge_coord_t nbr_coord = ge_coord_add(coord, GE_NEIGHBOR_OFFSETS[ii]);
    if (!ge_grid_has_coord(grid, nbr_coord)) {
      continue;
    }
    result.neighbors[result.num_neighbors++] = nbr_coord;
  }
  return result;
}

ge_neighbor_res_t ge_grid_get_neighbors_wrapped(const ge_grid_t* restrict grid, ge_coord_t coord)
{
  ge_neighbor_res_t result = GE_NEIGHBOR_RES_DEFAULTS;
  for (size_t ii = 0; ii < GE_MAX_NUM_NEIGHBORS; ++ii) {
    ge_coord_t nbr_coord = ge_coord_add(coord, GE_NEIGHBOR_OFFSETS[ii]);
    result.neighbors[result.num_neighbors++] = ge_coord_wrap(nbr_coord, grid->width, grid->height);
  }
  return result;
}

static void abort_on_out_of_bounds(const ge_grid_t* restrict grid, ge_coord_t coord)
{
  if (!ge_grid_has_coord(grid, coord)) {
    abort();
  }
}
