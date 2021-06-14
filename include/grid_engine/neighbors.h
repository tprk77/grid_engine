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

ge_direction_t ge_direction_get_opposite(ge_direction_t direction);
ge_coord_t ge_direction_get_offset(ge_direction_t direction);

ge_neighbors_t ge_neighbors_from_coord(ge_coord_t coord);
ge_neighbors_t ge_neighbors_from_coord_inside(ge_coord_t coord, size_t width, size_t height);
ge_neighbors_t ge_neighbors_from_coord_wrapped(ge_coord_t coord, size_t width, size_t height);
bool ge_neighbors_has_neighbor(const ge_neighbors_t* neighbors, ge_direction_t direction);
ge_coord_t ge_neighbors_get_neighbor(const ge_neighbors_t* neighbors, ge_direction_t direction);

/**
 * Used to iterate through a set of neighboring coords. If `prev_coord` is
 * `NULL`, this function will return the first coord in the set. If `prev_coord`
 * is the last coord in the set, the result will be `NULL`. Intended to be used
 * with a while loop, similarly to this:
 *
 * ```
 * const ge_coord_t* nbr_coord = NULL;
 * while ((nbr_coord = ge_neighbors_next_coord(&nbrs, nbr_coord)) != NULL) {
 *   ...
 * }
 * ```
 *
 * @param neighbors The set of neighboring coords.
 *
 * @param prev_coord The previous coord, or `NULL` if there wasn't one.
 *
 * @return The next coord in the set, or `NULL` if there isn't one.
 */
const ge_coord_t* ge_neighbors_next_coord(const ge_neighbors_t* neighbors,
                                          const ge_coord_t* prev_coord);

#endif  // GE_NEIGHBORS_H_
