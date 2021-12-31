// Copyright (c) 2019 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#include "grid_engine/grid.h"

#include <stdlib.h>
#include <string.h>

#include "grid_engine/log.h"

typedef struct ge_grid {
  size_t width;
  size_t height;
  uint8_t* pixel_arr;
} ge_grid_t;

static void ge_grid_scale_blit_rect_impl(ge_grid_t* grid, const ge_grid_t* blit_grid,
                                         ge_rect_t blit_rect, ge_coord_t coord,
                                         size_t pixel_multiplier);
static void abort_on_coord_out_of_bounds(const ge_grid_t* grid, ge_coord_t coord);
static void abort_on_rect_out_of_bounds(const ge_grid_t* grid, ge_rect_t rect);

ge_grid_t* ge_grid_create(size_t width, size_t height)
{
  ge_grid_t* grid = calloc(1, sizeof(ge_grid_t));
  if (grid == NULL) {
    return NULL;
  }
  grid->width = width;
  grid->height = height;
  grid->pixel_arr = calloc(grid->width * grid->height, sizeof(uint8_t));
  if (grid->pixel_arr == NULL) {
    free(grid);
    return NULL;
  }
  return grid;
}

void ge_grid_free(ge_grid_t* grid)
{
  if (grid == NULL) {
    return;
  }
  free(grid->pixel_arr);
  free(grid);
}

size_t ge_grid_get_width(const ge_grid_t* grid)
{
  return grid->width;
}

size_t ge_grid_get_height(const ge_grid_t* grid)
{
  return grid->height;
}

ge_rect_t ge_grid_get_rect(const ge_grid_t* grid)
{
  return ge_rect_from_wh(grid->width, grid->height);
}

const uint8_t* ge_grid_get_pixel_arr(const ge_grid_t* grid)
{
  return grid->pixel_arr;
}

uint8_t* ge_grid_get_pixel_arr_mut(ge_grid_t* grid)
{
  return grid->pixel_arr;
}

void ge_grid_copy_pixel_arr(ge_grid_t* src_grid, const ge_grid_t* dest_grid)
{
  if (src_grid == dest_grid) {
    GE_LOG_ERROR("Source and destination grids are the same!");
    abort();
  }
  if (src_grid->width != dest_grid->width || src_grid->height != dest_grid->height) {
    GE_LOG_ERROR("Source and destination grids are not the same size!");
    abort();
  }
  const size_t size = src_grid->width * src_grid->height;
  memcpy(src_grid->pixel_arr, dest_grid->pixel_arr, size);
}

void ge_grid_clear_pixel_arr(ge_grid_t* grid)
{
  const size_t size = grid->width * grid->height;
  memset(grid->pixel_arr, 0, size);
}

bool ge_grid_has_coord(const ge_grid_t* grid, ge_coord_t coord)
{
  return (coord.x >= 0 && coord.x < (ptrdiff_t) grid->width && coord.y >= 0
          && coord.y < (ptrdiff_t) grid->height);
}

uint8_t ge_grid_get_coord(const ge_grid_t* grid, ge_coord_t coord)
{
  abort_on_coord_out_of_bounds(grid, coord);
  return grid->pixel_arr[grid->width * coord.y + coord.x];
}

void ge_grid_set_coord(ge_grid_t* grid, ge_coord_t coord, uint8_t value)
{
  abort_on_coord_out_of_bounds(grid, coord);
  grid->pixel_arr[grid->width * coord.y + coord.x] = value;
}

uint8_t ge_grid_get_coord_wrapped(const ge_grid_t* grid, ge_coord_t coord)
{
  coord = ge_coord_wrap(coord, grid->width, grid->height);
  return ge_grid_get_coord(grid, coord);
}

void ge_grid_set_coord_wrapped(ge_grid_t* grid, ge_coord_t coord, uint8_t value)
{
  coord = ge_coord_wrap(coord, grid->width, grid->height);
  ge_grid_set_coord(grid, coord, value);
}

ge_nbrs_t ge_grid_get_nbrs(const ge_grid_t* grid, ge_coord_t coord)
{
  abort_on_coord_out_of_bounds(grid, coord);
  return ge_nbrs_from_coord_inside(coord, grid->width, grid->height);
}

ge_nbrs_t ge_grid_get_nbrs_wrapped(const ge_grid_t* grid, ge_coord_t coord)
{
  abort_on_coord_out_of_bounds(grid, coord);
  return ge_nbrs_from_coord_wrapped(coord, grid->width, grid->height);
}

ge_grid_t* ge_grid_copy_rect(const ge_grid_t* grid, ge_rect_t rect)
{
  // Create the grid and copy data
  abort_on_rect_out_of_bounds(grid, rect);
  const size_t width = ge_rect_get_width(rect);
  const size_t height = ge_rect_get_height(rect);
  ge_grid_t* const copy_grid = ge_grid_create(width, height);
  for (size_t jj = 0; jj < height; ++jj) {
    uint8_t* const dest_pixel_row = copy_grid->pixel_arr + (width * jj);
    uint8_t* const src_pixel_row =
        grid->pixel_arr + (grid->width * (jj + rect.min_coord.y)) + rect.min_coord.x;
    memcpy(dest_pixel_row, src_pixel_row, width);
  }
  return copy_grid;
}

void ge_grid_blit(ge_grid_t* grid, const ge_grid_t* blit_grid, ge_coord_t coord)
{
  // Get rect overlapping the grid
  const ge_rect_t grid_rect = ge_grid_get_rect(grid);
  const ge_rect_t shift_rect = ge_rect_add(ge_grid_get_rect(blit_grid), coord);
  const ge_rect_t overlap_rect = ge_rect_overlap(grid_rect, shift_rect);
  const ge_rect_t blit_rect = ge_rect_sub(overlap_rect, coord);
  const size_t blit_width = ge_rect_get_width(blit_rect);
  const size_t blit_height = ge_rect_get_height(blit_rect);
  for (size_t jj = 0; jj < blit_height; ++jj) {
    uint8_t* const dest_pixel_row =
        (grid->pixel_arr + (grid->width * (jj + overlap_rect.min_coord.y))
         + overlap_rect.min_coord.x);
    uint8_t* const src_pixel_row =
        (blit_grid->pixel_arr + (blit_grid->width * (jj + blit_rect.min_coord.y))
         + blit_rect.min_coord.x);
    memcpy(dest_pixel_row, src_pixel_row, blit_width);
  }
}

void ge_grid_scale_blit(ge_grid_t* grid, const ge_grid_t* blit_grid, ge_coord_t coord,
                        size_t pixel_multiplier)
{
  ge_grid_scale_blit_rect_impl(grid, blit_grid, ge_grid_get_rect(blit_grid), coord,
                               pixel_multiplier);
}

void ge_grid_scale_blit_rect(ge_grid_t* grid, const ge_grid_t* blit_grid, ge_rect_t blit_rect,
                             ge_coord_t coord, size_t pixel_multiplier)
{
  abort_on_rect_out_of_bounds(blit_grid, blit_rect);
  ge_grid_scale_blit_rect_impl(grid, blit_grid, blit_rect, coord, pixel_multiplier);
}

static void ge_grid_scale_blit_rect_impl(ge_grid_t* grid, const ge_grid_t* blit_grid,
                                         ge_rect_t blit_rect, ge_coord_t coord,
                                         size_t pixel_multiplier)
{
  const ge_rect_t grid_rect = ge_grid_get_rect(grid);
  const ge_rect_t shift_rect = ge_rect_add(ge_rect_sub(blit_rect, blit_rect.min_coord), coord);
  const ge_rect_t scaled_rect = ge_rect_mul(shift_rect, pixel_multiplier);
  const ge_rect_t overlap_rect = ge_rect_overlap(grid_rect, scaled_rect);
  const ge_rect_t scaled_blit_rect = ge_rect_sub(overlap_rect, coord);
  const size_t scaled_blit_width = ge_rect_get_width(scaled_blit_rect);
  const size_t scaled_blit_height = ge_rect_get_height(scaled_blit_rect);
  const ge_coord_t blit_offset = {scaled_blit_rect.min_coord.x / pixel_multiplier,
                                  scaled_blit_rect.min_coord.y / pixel_multiplier};
  const ge_coord_t blit_corner = ge_coord_add(blit_offset, blit_rect.min_coord);
  const ge_coord_t blit_subpx_offset = {scaled_blit_rect.min_coord.x % pixel_multiplier,
                                        scaled_blit_rect.min_coord.y % pixel_multiplier};
  // Loop over the scaled blit grid
  const uint8_t* last_filed_row = NULL;
  for (size_t jj = 0; jj < scaled_blit_height; ++jj) {
    uint8_t* const dest_pixel_row =
        (grid->pixel_arr + (grid->width * (jj + overlap_rect.min_coord.y))
         + overlap_rect.min_coord.x);
    if (jj == 0 || (jj + blit_subpx_offset.y) % pixel_multiplier == 0) {
      // This is a new row of pixels from the blit grid
      const size_t blit_y = ((jj + blit_subpx_offset.y) / pixel_multiplier) + blit_corner.y;
      uint8_t* const src_pixel_row =
          (blit_grid->pixel_arr + (blit_grid->width * blit_y) + blit_corner.x);
      for (size_t ii = 0; ii < scaled_blit_width; ++ii) {
        const size_t blit_x = (ii + blit_subpx_offset.x) / pixel_multiplier;
        dest_pixel_row[ii] = src_pixel_row[blit_x];
      }
      last_filed_row = dest_pixel_row;
    }
    else {
      // This is a repeat row of pixels, so do a copy instead
      memcpy(dest_pixel_row, last_filed_row, scaled_blit_width);
    }
  }
}

static void abort_on_coord_out_of_bounds(const ge_grid_t* grid, ge_coord_t coord)
{
  if (!ge_grid_has_coord(grid, coord)) {
    GE_LOG_ERROR("Coord is out of bounds! (%li, %li)", coord.x, coord.y);
    abort();
  }
}

static void abort_on_rect_out_of_bounds(const ge_grid_t* grid, ge_rect_t rect)
{
  const ge_rect_t grid_rect = ge_grid_get_rect(grid);
  if (!ge_rect_within_rect(grid_rect, rect)) {
    GE_LOG_ERROR("Rect is out of bounds! [(%li, %li), (%li, %li)]", rect.min_coord.x,
                 rect.min_coord.y, rect.max_coord.x, rect.max_coord.y);
    abort();
  }
}
