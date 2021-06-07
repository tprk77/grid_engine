// Copyright (c) 2019 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#include "grid_engine/neighbors.h"

#include <stdlib.h>

#include "grid_engine/log.h"

static void abort_on_invalid_coord(ge_coord_t coord);
static bool coord_inside(ge_coord_t coord, size_t width, size_t height);

static const ge_coord_t GE_NEIGHBOR_OFFSETS[GE_MAX_NUM_NEIGHBORS] = {
    {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}};

static const ge_neighbors_t GE_NEIGHBORS_DEFAULTS = {
    .neighbors =
        {
            GE_INVALID_COORD_K,
            GE_INVALID_COORD_K,
            GE_INVALID_COORD_K,
            GE_INVALID_COORD_K,
            GE_INVALID_COORD_K,
            GE_INVALID_COORD_K,
            GE_INVALID_COORD_K,
            GE_INVALID_COORD_K,
        },
};

ge_neighbors_t ge_neighbors_from_coord(ge_coord_t coord)
{
  ge_neighbors_t neighbors = GE_NEIGHBORS_DEFAULTS;
  for (size_t ii = 0; ii < GE_MAX_NUM_NEIGHBORS; ++ii) {
    neighbors.neighbors[ii] = ge_coord_add(coord, GE_NEIGHBOR_OFFSETS[ii]);
  }
  return neighbors;
}

ge_neighbors_t ge_neighbors_from_coord_inside(ge_coord_t coord, size_t width, size_t height)
{
  ge_neighbors_t neighbors = GE_NEIGHBORS_DEFAULTS;
  for (size_t ii = 0; ii < GE_MAX_NUM_NEIGHBORS; ++ii) {
    ge_coord_t nbr_coord = ge_coord_add(coord, GE_NEIGHBOR_OFFSETS[ii]);
    if (!coord_inside(nbr_coord, width, height)) {
      continue;
    }
    neighbors.neighbors[ii] = nbr_coord;
  }
  return neighbors;
}

ge_neighbors_t ge_neighbors_from_coord_wrapped(ge_coord_t coord, size_t width, size_t height)
{
  ge_neighbors_t neighbors = GE_NEIGHBORS_DEFAULTS;
  for (size_t ii = 0; ii < GE_MAX_NUM_NEIGHBORS; ++ii) {
    neighbors.neighbors[ii] =
        ge_coord_wrap(ge_coord_add(coord, GE_NEIGHBOR_OFFSETS[ii]), width, height);
  }
  return neighbors;
}

bool ge_neighbors_has_neighbor(const ge_neighbors_t* neighbors, ge_direction_t direction)
{
  return !ge_coord_is_invalid(neighbors->neighbors[direction]);
}

ge_coord_t ge_neighbors_get_neighbor(const ge_neighbors_t* neighbors, ge_direction_t direction)
{
  abort_on_invalid_coord(neighbors->neighbors[direction]);
  return neighbors->neighbors[direction];
}

const ge_coord_t* ge_neighbors_next_coord(const ge_neighbors_t* neighbors,
                                          const ge_coord_t* prev_coord)
{
  const ge_coord_t* const begin_coord = neighbors->neighbors;
  const ge_coord_t* const end_coord = neighbors->neighbors + GE_MAX_NUM_NEIGHBORS;
  const ge_coord_t* coord = (prev_coord == NULL ? begin_coord : prev_coord + 1);
  while (coord < end_coord && ge_coord_is_invalid(*coord)) {
    ++prev_coord;
  }
  return (coord != end_coord ? coord : NULL);
}

ge_coord_t ge_neighbors_get_offset(ge_direction_t direction)
{
  return GE_NEIGHBOR_OFFSETS[direction];
}

static void abort_on_invalid_coord(ge_coord_t coord)
{
  if (ge_coord_is_invalid(coord)) {
    GE_LOG_ERROR("Coord is invalid!");
    abort();
  }
}

static bool coord_inside(ge_coord_t coord, size_t width, size_t height)
{
  return (coord.x >= 0 && coord.x < (ptrdiff_t) width && coord.y >= 0
          && coord.y < (ptrdiff_t) height);
}
