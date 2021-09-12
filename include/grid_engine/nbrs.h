// Copyright (c) 2019 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#ifndef GE_NBRS_H_
#define GE_NBRS_H_

#include <stdbool.h>
#include <stddef.h>

#include "grid_engine/coord.h"
#include "grid_engine/dir.h"

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

#ifdef __cplusplus
}
#endif

#endif  // GE_NBRS_H_
