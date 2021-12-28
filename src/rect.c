// Copyright (c) 2021 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#include "grid_engine/rect.h"

static ptrdiff_t pd_min(ptrdiff_t a, ptrdiff_t b);
static ptrdiff_t pd_max(ptrdiff_t a, ptrdiff_t b);

const ge_rect_t GE_INVALID_RECT = GE_INVALID_RECT_K;

ge_rect_t ge_rect_from_wh(size_t width, size_t height)
{
  return (ge_rect_t){
      (ge_coord_t){0, 0},
      (ge_coord_t){width, height},
  };
}

ge_rect_t ge_rect_from_coord_wh(ge_coord_t coord, size_t width, size_t height)
{
  return (ge_rect_t){
      coord,
      ge_coord_add(coord, (ge_coord_t){width, height}),
  };
}

size_t ge_rect_get_width(ge_rect_t rect)
{
  return rect.max_coord.x - rect.min_coord.x;
}

size_t ge_rect_get_height(ge_rect_t rect)
{
  return rect.max_coord.y - rect.min_coord.y;
}

ge_rect_t ge_rect_set_coord(ge_rect_t rect, ge_coord_t coord)
{
  const ge_coord_t offset_coord = ge_coord_sub(rect.max_coord, rect.min_coord);
  return (ge_rect_t){
      coord,
      ge_coord_add(coord, offset_coord),
  };
}

ge_rect_t ge_rect_set_width(ge_rect_t rect, size_t width)
{
  const size_t height = rect.max_coord.y - rect.min_coord.y;
  const ge_coord_t offset_coord = {width, height};
  return (ge_rect_t){
      rect.min_coord,
      ge_coord_add(rect.min_coord, offset_coord),
  };
}

ge_rect_t ge_rect_set_height(ge_rect_t rect, size_t height)
{
  const size_t width = rect.max_coord.x - rect.min_coord.x;
  const ge_coord_t offset_coord = {width, height};
  return (ge_rect_t){
      rect.min_coord,
      ge_coord_add(rect.min_coord, offset_coord),
  };
}

ge_rect_t ge_rect_add(ge_rect_t rect, ge_coord_t coord)
{
  return (ge_rect_t){
      ge_coord_add(rect.min_coord, coord),
      ge_coord_add(rect.max_coord, coord),
  };
}

ge_rect_t ge_rect_sub(ge_rect_t rect, ge_coord_t coord)
{
  return (ge_rect_t){
      ge_coord_sub(rect.min_coord, coord),
      ge_coord_sub(rect.max_coord, coord),
  };
}

ge_rect_t ge_rect_mul(ge_rect_t rect, size_t scalar)
{
  const ge_coord_t offset_coord = ge_coord_sub(rect.max_coord, rect.min_coord);
  return (ge_rect_t){
      rect.min_coord,
      ge_coord_add(rect.min_coord, ge_coord_mul(offset_coord, scalar)),
  };
}

ge_rect_t ge_rect_div(ge_rect_t rect, size_t scalar)
{
  const ge_coord_t offset_coord = ge_coord_sub(rect.max_coord, rect.min_coord);
  return (ge_rect_t){
      rect.min_coord,
      ge_coord_add(rect.min_coord, ge_coord_div(offset_coord, scalar)),
  };
}

ge_rect_t ge_rect_overlap(ge_rect_t rect, ge_rect_t other)
{
  return (ge_rect_t){
      (ge_coord_t){
          pd_max(rect.min_coord.x, other.min_coord.x),
          pd_max(rect.min_coord.y, other.min_coord.y),
      },
      (ge_coord_t){
          pd_min(rect.max_coord.x, other.max_coord.x),
          pd_min(rect.max_coord.y, other.max_coord.y),
      },
  };
}

ge_rect_t ge_rect_clamp_rect(ge_rect_t rect, ge_rect_t other)
{
  // Clamp each coorner of the other rect to be within this rect
  return (ge_rect_t){
      (ge_coord_t){
          pd_max(rect.min_coord.x, pd_min(other.min_coord.x, rect.max_coord.x)),
          pd_max(rect.min_coord.y, pd_min(other.min_coord.y, rect.max_coord.y)),
      },
      (ge_coord_t){
          pd_max(rect.min_coord.x, pd_min(other.max_coord.x, rect.max_coord.x)),
          pd_max(rect.min_coord.y, pd_min(other.max_coord.y, rect.max_coord.y)),
      },
  };
}

ge_coord_t ge_rect_clamp_coord(ge_rect_t rect, ge_coord_t coord)
{
  // Make sure the rect does not have zero width or height
  if (rect.min_coord.x == rect.max_coord.x || rect.min_coord.y == rect.max_coord.y) {
    return GE_INVALID_COORD;
  }
  // When we clamp coords, remember that the max coord is exclusive
  return (ge_coord_t){
      pd_max(rect.min_coord.x, pd_min(coord.x, rect.max_coord.x - 1)),
      pd_max(rect.min_coord.y, pd_min(coord.y, rect.max_coord.y - 1)),
  };
}

bool ge_rect_equals(ge_rect_t rect, ge_rect_t other)
{
  return (ge_coord_equals(rect.min_coord, other.min_coord)
          && ge_coord_equals(rect.max_coord, other.max_coord));
}

bool ge_rect_is_invalid(ge_rect_t rect)
{
  return (ge_coord_is_invalid(rect.min_coord) || ge_coord_is_invalid(rect.max_coord));
}

bool ge_rect_within_rect(ge_rect_t rect, ge_rect_t other)
{
  return ((rect.min_coord.x <= other.min_coord.x && other.min_coord.x <= rect.max_coord.x)
          && (rect.min_coord.y <= other.min_coord.y && other.min_coord.y <= rect.max_coord.y)
          && (rect.min_coord.x <= other.max_coord.x && other.max_coord.x <= rect.max_coord.x)
          && (rect.min_coord.y <= other.max_coord.y && other.max_coord.y <= rect.max_coord.y));
}

bool ge_rect_within_coord(ge_rect_t rect, ge_coord_t coord)
{
  // When checking coords, remember that the max coord is exclusive
  return ((rect.min_coord.x <= coord.x && coord.x < rect.max_coord.x)
          && (rect.min_coord.y <= coord.y && coord.y < rect.max_coord.y));
}

static ptrdiff_t pd_min(ptrdiff_t a, ptrdiff_t b)
{
  return (a < b ? a : b);
}

static ptrdiff_t pd_max(ptrdiff_t a, ptrdiff_t b)
{
  return (a > b ? a : b);
}
