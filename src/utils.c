// Copyright (c) 2019 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#include "grid_engine/utils.h"

static ptrdiff_t pd_abs(ptrdiff_t v);

size_t ge_utils_line_coords(ge_coord_t start_coord, ge_coord_t end_coord, ge_coord_t* output_coords,
                            size_t max_num_coords)
{
  // See also: https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
  const ptrdiff_t diff_x = pd_abs(end_coord.x - start_coord.x);
  const ptrdiff_t diff_y = pd_abs(end_coord.y - start_coord.y);
  const ptrdiff_t step_x = (end_coord.x > start_coord.x ? 1 : -1);
  const ptrdiff_t step_y = (end_coord.y > start_coord.y ? 1 : -1);
  ptrdiff_t x = start_coord.x;
  ptrdiff_t y = start_coord.y;
  size_t num_coords = 0;
  if (max_num_coords > 0) {
    output_coords[num_coords++] = (ge_coord_t){x, y};
  }
  ptrdiff_t error = (diff_x >= diff_y ? diff_x : -diff_y) / 2;
  while (num_coords < max_num_coords && (x != end_coord.x || y != end_coord.y)) {
    ptrdiff_t prev_error = error;
    if (prev_error > -diff_x) {
      error -= diff_y;
      x += step_x;
    }
    if (prev_error < diff_y) {
      error += diff_x;
      y += step_y;
    }
    output_coords[num_coords++] = (ge_coord_t){x, y};
  }
  return num_coords;
}

static ptrdiff_t pd_abs(ptrdiff_t v)
{
  return (v > 0 ? v : -v);
}
