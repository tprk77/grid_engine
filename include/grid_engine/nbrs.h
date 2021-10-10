// Copyright (c) 2019 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#ifndef GE_NBRS_H_
#define GE_NBRS_H_

#include <stdbool.h>
#include <stddef.h>

#include "grid_engine/coord.h"
#include "grid_engine/dir.h"
#include "grid_engine/meta.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ge_nbrs {
  ge_coord_t nbrs[GE_NUM_DIRS];
} ge_nbrs_t;

#define GE_NBRS_DEFAULTS_K \
  {                        \
    .nbrs = {              \
      GE_INVALID_COORD_K,  \
      GE_INVALID_COORD_K,  \
      GE_INVALID_COORD_K,  \
      GE_INVALID_COORD_K,  \
      GE_INVALID_COORD_K,  \
      GE_INVALID_COORD_K,  \
      GE_INVALID_COORD_K,  \
      GE_INVALID_COORD_K,  \
    }                      \
  }

extern const ge_nbrs_t GE_NBRS_DEFAULTS;

ge_nbrs_t ge_nbrs_from_coord(ge_coord_t coord);
ge_nbrs_t ge_nbrs_from_coord_inside(ge_coord_t coord, size_t width, size_t height);
ge_nbrs_t ge_nbrs_from_coord_wrapped(ge_coord_t coord, size_t width, size_t height);
bool ge_nbrs_has_nbr(const ge_nbrs_t* nbrs, ge_dir_t dir);
ge_coord_t ge_nbrs_get_nbr(const ge_nbrs_t* nbrs, ge_dir_t dir);

/**
 * Used to iterate through a set of neighboring coords. If `prev_dir` is
 * `GE_DIR_NONE`, this function will return a direction corresponding to
 * the first valid coord in the set. If `prev_dir` corresponds to the last
 * valid coord in the set, the result will be `GE_DIR_NONE`. Intended to
 * be used with a while loop, similarly to this:
 *
 * ```
 * ge_dir_t nbr_dir = GE_DIR_NONE;
 * while ((dir = ge_nbrs_next_dir(&nbrs, nbr_dir)) != GE_DIR_NONE) {
 *   const ge_coord_t nbr_coord = ge_nbrs_get_nbr(&nbrs, nbr_dir);
 *   ...
 * }
 * ```
 *
 * @param nbrs The set of neighboring coords.
 *
 * @param prev_dir The previous direction, or `GE_DIR_NONE` if there wasn't one.
 *
 * @return The next direction in the set, or `GE_DIR_NONE` if there isn't one.
 */
ge_dir_t ge_nbrs_next_dir(const ge_nbrs_t* nbrs, ge_dir_t prev_dir);

// Allow easier use of the function above using a macro
#define GE_FOR_NBR_DIRS(NBR_DIR, NBRS) \
  for (ge_dir_t NBR_DIR = GE_DIR_NONE; (NBR_DIR = ge_nbrs_next_dir(NBRS, NBR_DIR)) != GE_DIR_NONE;)

/**
 * Used to iterate through a set of neighboring coords. If `prev_coord` is
 * `NULL`, this function will return the first coord in the set. If `prev_coord`
 * is the last coord in the set, the result will be `NULL`. Intended to be used
 * with a while loop, similarly to this:
 *
 * ```
 * const ge_coord_t* nbr_coord = NULL;
 * while ((nbr_coord = ge_nbrs_next_coord(&nbrs, nbr_coord)) != NULL) {
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
const ge_coord_t* ge_nbrs_next_coord(const ge_nbrs_t* nbrs, const ge_coord_t* prev_coord);

// Allow easier use of the function above using a macro. This macros does a
// somewhat evil const cast using `(void *)` out of necessity, because both the
// coord and the pointer to a coord must fit in the same for loop init
// statement. I think this is probably ok, because the pointer is supposed to be
// an internal detail and is never used to modify the actual value.
#define GE_FOR_NBR_COORDS(NBR_COORD, NBRS)                                                        \
  for (ge_coord_t NBR_COORD = GE_INVALID_COORD, *GE_PASTE(NBR_COORD, _ptr_) = NULL;               \
       GE_PASTE(NBR_COORD, _ptr_) = (void*) ge_nbrs_next_coord(NBRS, GE_PASTE(NBR_COORD, _ptr_)), \
                  (GE_PASTE(NBR_COORD, _ptr_) != NULL                                             \
                       ? (NBR_COORD = *GE_PASTE(NBR_COORD, _ptr_), true)                          \
                       : false);)

#ifdef __cplusplus
}
#endif

#endif  // GE_NBRS_H_
