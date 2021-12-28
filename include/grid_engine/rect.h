// Copyright (c) 2021 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#ifndef GE_RECT_H_
#define GE_RECT_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "grid_engine/coord.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Represents a rectangle, i.e., an Axis Aligned Bounding Box (AABB), using a
 * minimum and maximum corner coord. The minimum coord is inclusive, and the
 * maximum coord is exclusive.
 */
typedef struct ge_rect {
  ge_coord_t min_coord;
  ge_coord_t max_coord;
} ge_rect_t;

#define GE_INVALID_RECT_K                                             \
  {                                                                   \
    .min_coord = GE_INVALID_COORD_K, .max_coord = GE_INVALID_COORD_K, \
  }

extern const ge_rect_t GE_INVALID_RECT;

ge_rect_t ge_rect_from_wh(size_t width, size_t height);
ge_rect_t ge_rect_from_coord_wh(ge_coord_t coord, size_t width, size_t height);
size_t ge_rect_get_width(ge_rect_t rect);
size_t ge_rect_get_height(ge_rect_t rect);
ge_rect_t ge_rect_set_coord(ge_rect_t rect, ge_coord_t coord);
ge_rect_t ge_rect_set_width(ge_rect_t rect, size_t width);
ge_rect_t ge_rect_set_height(ge_rect_t rect, size_t height);
ge_rect_t ge_rect_add(ge_rect_t rect, ge_coord_t coord);
ge_rect_t ge_rect_sub(ge_rect_t rect, ge_coord_t coord);
ge_rect_t ge_rect_mul(ge_rect_t rect, size_t scalar);
ge_rect_t ge_rect_div(ge_rect_t rect, size_t scalar);
ge_rect_t ge_rect_overlap(ge_rect_t rect, ge_rect_t other);
ge_rect_t ge_rect_clamp_rect(ge_rect_t rect, ge_rect_t other);
ge_coord_t ge_rect_clamp_coord(ge_rect_t rect, ge_coord_t coord);
bool ge_rect_equals(ge_rect_t rect, ge_rect_t other);
bool ge_rect_is_invalid(ge_rect_t rect);
bool ge_rect_within_rect(ge_rect_t rect, ge_rect_t other);
bool ge_rect_within_coord(ge_rect_t rect, ge_coord_t coord);

#ifdef __cplusplus
}
#endif

#endif  // GE_RECT_H_
