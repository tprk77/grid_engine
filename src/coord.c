// Copyright (c) 2019 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#include "grid_engine/coord.h"

static ptrdiff_t pd_min(ptrdiff_t a, ptrdiff_t b);
static ptrdiff_t pd_max(ptrdiff_t a, ptrdiff_t b);
static ptrdiff_t pd_mod(ptrdiff_t a, ptrdiff_t b);

ge_coord_t ge_coord_add(ge_coord_t coord, ge_coord_t other)
{
  return (ge_coord_t){coord.x + other.x, coord.y + other.y};
}

ge_coord_t ge_coord_sub(ge_coord_t coord, ge_coord_t other)
{
  return (ge_coord_t){coord.x - other.x, coord.y - other.y};
}

ge_coord_t ge_coord_mul(ge_coord_t coord, ptrdiff_t scalar)
{
  return (ge_coord_t){coord.x * scalar, coord.y * scalar};
}

ge_coord_t ge_coord_div(ge_coord_t coord, ptrdiff_t scalar)
{
  return (ge_coord_t){coord.x / scalar, coord.y / scalar};
}

ge_coord_t ge_coord_clamp(ge_coord_t coord, size_t width, size_t height)
{
  const ptrdiff_t last_col = width > 0 ? width - 1 : 0;
  const ptrdiff_t last_row = height > 0 ? height - 1 : 0;
  return (ge_coord_t){
      .x = pd_max(0, pd_min(coord.x, last_col)),
      .y = pd_max(0, pd_min(coord.y, last_row)),
  };
}

ge_coord_t ge_coord_wrap(ge_coord_t coord, size_t width, size_t height)
{
  return (ge_coord_t){
      .x = pd_mod(coord.x, width),
      .y = pd_mod(coord.y, height),
  };
}

static ptrdiff_t pd_min(ptrdiff_t a, ptrdiff_t b)
{
  return (a < b ? a : b);
}

static ptrdiff_t pd_max(ptrdiff_t a, ptrdiff_t b)
{
  return (a > b ? a : b);
}

static ptrdiff_t pd_mod(ptrdiff_t a, ptrdiff_t b)
{
  // Handle negative numbers, so the result is always positive
  return (a >= 0 ? a % b : (b + a % -b) % b);
}
