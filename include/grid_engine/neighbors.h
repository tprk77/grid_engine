// Copyright (c) 2019 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#ifndef GE_NEIGHBORS_H_
#define GE_NEIGHBORS_H_

#include <stdbool.h>
#include <stddef.h>

#include "grid_engine/coord.h"

#define GE_NUM_DIRECTIONS 8

typedef enum ge_direction {
  GE_DIRECTION_NORTH = 0,
  GE_DIRECTION_NORTHEAST,
  GE_DIRECTION_EAST,
  GE_DIRECTION_SOUTHEAST,
  GE_DIRECTION_SOUTH,
  GE_DIRECTION_SOUTHWEST,
  GE_DIRECTION_WEST,
  GE_DIRECTION_NORTHWEST,
  GE_DIRECTION_NONE,  // Special value to indicate no direction
  GE_DIRECTION_ALL,   // Special value to indicate all directions
} ge_direction_t;

typedef struct ge_neighbors {
  ge_coord_t neighbors[GE_NUM_DIRECTIONS];
} ge_neighbors_t;

#define GE_NEIGHBORS_DEFAULTS_K \
  {                             \
    .neighbors = {              \
      GE_INVALID_COORD_K,       \
      GE_INVALID_COORD_K,       \
      GE_INVALID_COORD_K,       \
      GE_INVALID_COORD_K,       \
      GE_INVALID_COORD_K,       \
      GE_INVALID_COORD_K,       \
      GE_INVALID_COORD_K,       \
      GE_INVALID_COORD_K,       \
    }                           \
  }

extern const ge_neighbors_t GE_NEIGHBORS_DEFAULTS;

ge_direction_t ge_direction_get_opposite(ge_direction_t direction);
ge_coord_t ge_direction_get_offset(ge_direction_t direction);

ge_neighbors_t ge_neighbors_from_coord(ge_coord_t coord);
ge_neighbors_t ge_neighbors_from_coord_inside(ge_coord_t coord, size_t width, size_t height);
ge_neighbors_t ge_neighbors_from_coord_wrapped(ge_coord_t coord, size_t width, size_t height);
bool ge_neighbors_has_neighbor(const ge_neighbors_t* neighbors, ge_direction_t direction);
ge_coord_t ge_neighbors_get_neighbor(const ge_neighbors_t* neighbors, ge_direction_t direction);

/**
 * Used to iterate through a set of neighboring coords. If `prev_direction` is
 * `GE_DIRECTION_NONE`, this function will return a direction corresponding to
 * the first valid coord in the set. If `prev_direction` corresponds to the last
 * valid coord in the set, the result will be `GE_DIRECTION_NONE`. Intended to
 * be used with a while loop, similarly to this:
 *
 * ```
 * ge_direction_t nbr_dir = GE_DIRECTION_NONE;
 * while ((dir = ge_neighbors_next_direction(&nbrs, nbr_dir)) != GE_DIRECTION_NONE) {
 *   const ge_coord_t nbr_coord = ge_neighbors_get_neighbor(&nbrs, nbr_dir);
 *   ...
 * }
 * ```
 *
 * @param neighbors The set of neighboring coords.
 *
 * @param prev_direction The previous direction, or `GE_DIRECTION_NONE` if there wasn't one.
 *
 * @return The next direction in the set, or `GE_DIRECTION_NONE` if there isn't one.
 */
ge_direction_t ge_neighbors_next_direction(const ge_neighbors_t* neighbors,
                                           ge_direction_t prev_direction);

#endif  // GE_NEIGHBORS_H_
