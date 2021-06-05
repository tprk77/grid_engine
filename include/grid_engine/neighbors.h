// Copyright (c) 2019 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#ifndef GE_NEIGHBORS_H_
#define GE_NEIGHBORS_H_

#include <stdbool.h>
#include <stddef.h>

#include "grid_engine/coord.h"

#define GE_MAX_NUM_NEIGHBORS 8

typedef enum ge_direction {
  GE_DIRECTION_NONE = -1,
  GE_DIRECTION_NORTH = 0,
  GE_DIRECTION_NORTHEAST,
  GE_DIRECTION_EAST,
  GE_DIRECTION_SOUTHEAST,
  GE_DIRECTION_SOUTH,
  GE_DIRECTION_SOUTHWEST,
  GE_DIRECTION_WEST,
  GE_DIRECTION_NORTHWEST,
} ge_direction_t;

typedef struct ge_neighbors {
  ge_coord_t neighbors[GE_MAX_NUM_NEIGHBORS];
} ge_neighbors_t;

ge_neighbors_t ge_neighbors_from_coord(ge_coord_t coord);
ge_neighbors_t ge_neighbors_from_coord_inside(ge_coord_t coord, size_t width, size_t height);
ge_neighbors_t ge_neighbors_from_coord_wrapped(ge_coord_t coord, size_t width, size_t height);
bool ge_neighbors_has_neighbor(const ge_neighbors_t* neighbors, ge_direction_t direction);
ge_coord_t ge_neighbors_get_neighbor(const ge_neighbors_t* neighbors, ge_direction_t direction);
size_t ge_neighbors_next_index(const ge_neighbors_t* neighbors, size_t index);
ge_coord_t ge_neighbors_get_offset(ge_direction_t direction);

#endif  // GE_NEIGHBORS_H_
