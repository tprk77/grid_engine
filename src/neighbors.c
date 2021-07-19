// Copyright (c) 2019 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#include "grid_engine/neighbors.h"

#include <stdlib.h>

#include "grid_engine/log.h"

static bool coord_inside(ge_coord_t coord, size_t width, size_t height);

const ge_neighbors_t GE_NEIGHBORS_DEFAULTS = GE_NEIGHBORS_DEFAULTS_K;

static const ge_direction_t GE_DIRECTION_OPPOSITES[GE_NUM_DIRECTIONS] = {
    GE_DIRECTION_SOUTH, GE_DIRECTION_SOUTHWEST, GE_DIRECTION_WEST, GE_DIRECTION_NORTHWEST,
    GE_DIRECTION_NORTH, GE_DIRECTION_NORTHEAST, GE_DIRECTION_EAST, GE_DIRECTION_SOUTHEAST,
};

static const ge_coord_t GE_DIRECTION_OFFSETS[GE_NUM_DIRECTIONS] = {
    {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1},
};

ge_direction_t ge_direction_get_opposite(ge_direction_t direction)
{
  if (direction == GE_DIRECTION_NONE || direction == GE_DIRECTION_ALL) {
    GE_LOG_ERROR("Cannot get opposite for special direction ALL or NONE!");
    abort();
  }
  return GE_DIRECTION_OPPOSITES[direction];
}

ge_coord_t ge_direction_get_offset(ge_direction_t direction)
{
  if (direction == GE_DIRECTION_NONE || direction == GE_DIRECTION_ALL) {
    GE_LOG_ERROR("Cannot get offset for special direction ALL or NONE!");
    abort();
  }
  return GE_DIRECTION_OFFSETS[direction];
}

ge_neighbors_t ge_neighbors_from_coord(ge_coord_t coord)
{
  ge_neighbors_t neighbors = GE_NEIGHBORS_DEFAULTS;
  for (size_t ii = 0; ii < GE_NUM_DIRECTIONS; ++ii) {
    neighbors.neighbors[ii] = ge_coord_add(coord, GE_DIRECTION_OFFSETS[ii]);
  }
  return neighbors;
}

ge_neighbors_t ge_neighbors_from_coord_inside(ge_coord_t coord, size_t width, size_t height)
{
  ge_neighbors_t neighbors = GE_NEIGHBORS_DEFAULTS;
  for (size_t ii = 0; ii < GE_NUM_DIRECTIONS; ++ii) {
    ge_coord_t nbr_coord = ge_coord_add(coord, GE_DIRECTION_OFFSETS[ii]);
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
  for (size_t ii = 0; ii < GE_NUM_DIRECTIONS; ++ii) {
    neighbors.neighbors[ii] =
        ge_coord_wrap(ge_coord_add(coord, GE_DIRECTION_OFFSETS[ii]), width, height);
  }
  return neighbors;
}

bool ge_neighbors_has_neighbor(const ge_neighbors_t* neighbors, ge_direction_t direction)
{
  if (direction == GE_DIRECTION_NONE || direction == GE_DIRECTION_ALL) {
    const ge_coord_t* const begin_coord = neighbors->neighbors;
    const ge_coord_t* const end_coord = neighbors->neighbors + GE_NUM_DIRECTIONS;
    for (const ge_coord_t* nbr_coord = begin_coord; nbr_coord < end_coord; ++nbr_coord) {
      const bool has_neighbor = !ge_coord_is_invalid(*nbr_coord);
      if ((direction == GE_DIRECTION_NONE && has_neighbor)
          || (direction == GE_DIRECTION_ALL && !has_neighbor)) {
        return false;
      }
    }
    return true;
  }
  return !ge_coord_is_invalid(neighbors->neighbors[direction]);
}

ge_coord_t ge_neighbors_get_neighbor(const ge_neighbors_t* neighbors, ge_direction_t direction)
{
  if (direction == GE_DIRECTION_NONE || direction == GE_DIRECTION_ALL) {
    GE_LOG_ERROR("Cannot get neighbor with special dirction ALL or NONE!");
    abort();
  }
  if (ge_coord_is_invalid(neighbors->neighbors[direction])) {
    GE_LOG_ERROR("Neighbor does not exist for direction");
    abort();
  }
  return neighbors->neighbors[direction];
}

const ge_coord_t* ge_neighbors_next_coord(const ge_neighbors_t* neighbors,
                                          const ge_coord_t* prev_coord)
{
  const ge_coord_t* const begin_coord = neighbors->neighbors;
  const ge_coord_t* const end_coord = neighbors->neighbors + GE_NUM_DIRECTIONS;
  const ge_coord_t* coord = (prev_coord == NULL ? begin_coord : prev_coord + 1);
  while (coord < end_coord && ge_coord_is_invalid(*coord)) {
    ++coord;
  }
  return (coord != end_coord ? coord : NULL);
}

static bool coord_inside(ge_coord_t coord, size_t width, size_t height)
{
  return (coord.x >= 0 && coord.x < (ptrdiff_t) width && coord.y >= 0
          && coord.y < (ptrdiff_t) height);
}
