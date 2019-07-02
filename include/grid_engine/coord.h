// Copyright (c) 2019 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#ifndef GE_COORD_H_
#define GE_COORD_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * Represents a coordinate or an offset on the grid.
 *
 * Because the coord can be relative to another coord, the coord can have negative values. (The type
 * `ptrdiff_t` is used here because it's the signed counterpart of `size_t`.)
 */
typedef struct ge_coord {
  ptrdiff_t x;
  ptrdiff_t y;
} ge_coord_t;

ge_coord_t ge_coord_add(ge_coord_t coord, ge_coord_t other);
ge_coord_t ge_coord_sub(ge_coord_t coord, ge_coord_t other);
ge_coord_t ge_coord_mul(ge_coord_t coord, ptrdiff_t scalar);
ge_coord_t ge_coord_div(ge_coord_t coord, ptrdiff_t scalar);
ge_coord_t ge_coord_clamp(ge_coord_t coord, size_t width, size_t height);
ge_coord_t ge_coord_wrap(ge_coord_t coord, size_t width, size_t height);

#endif  // GE_COORD_H_
