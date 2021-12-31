// Copyright (c) 2021 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#include "grid_engine/sc_view.h"

#include <assert.h>
#include <math.h>
#include <stdlib.h>

#include "grid_engine/log.h"

typedef struct ge_sc_view {
  const ge_grid_t* source_grid;
  ge_grid_t* render_grid;
  size_t pixel_multiplier;
  ge_rect_t sc_rect;
} ge_sc_view_t;

ge_sc_view_t* ge_sc_view_create(size_t width, size_t height, size_t pixel_multiplier,
                                const ge_grid_t* source_grid)
{
  // TODO Support smaller grid viewing
  assert(width / pixel_multiplier <= ge_grid_get_width(source_grid));
  assert(height / pixel_multiplier <= ge_grid_get_height(source_grid));
  ge_sc_view_t* view = calloc(1, sizeof(ge_sc_view_t));
  if (view == NULL) {
    return NULL;
  }
  view->source_grid = source_grid;
  view->render_grid = ge_grid_create(width, height);
  if (view->render_grid == NULL) {
    free(view);
    return NULL;
  }
  view->pixel_multiplier = pixel_multiplier;
  // The width and height should be constant, but the coord can change
  view->sc_rect = ge_rect_from_wh(width / pixel_multiplier, height / pixel_multiplier);
  return view;
}

void ge_sc_view_free(ge_sc_view_t* view)
{
  if (view == NULL) {
    return;
  }
  ge_grid_free(view->render_grid);
  free(view);
}

void ge_sc_view_refresh(ge_sc_view_t* view)
{
  ge_grid_scale_blit_rect(view->render_grid, view->source_grid, view->sc_rect, (ge_coord_t){0, 0},
                          view->pixel_multiplier);
}

ge_grid_t* ge_sc_view_get_render_grid(ge_sc_view_t* view)
{
  return view->render_grid;
}

ge_grid_t* ge_sc_view_resize(ge_sc_view_t* view, size_t width, size_t height)
{
  // Create a new render grid
  ge_grid_t* const new_render_grid = ge_grid_create(width, height);
  if (new_render_grid == NULL) {
    return NULL;
  }
  // Replace the old render grid
  ge_grid_free(view->render_grid);
  view->render_grid = new_render_grid;
  return view->render_grid;
}

void ge_sc_view_scroll_to_x_abs(ge_sc_view_t* view, double x)
{
  // IEEE 754 doubles can exactly represent up to 52-bit integers
  const ptrdiff_t xf = floor(x);
  const ge_coord_t new_coord = {xf, view->sc_rect.min_coord.y};
  const ge_rect_t new_rect = ge_rect_set_coord(view->sc_rect, new_coord);
  const ge_rect_t source_rect = ge_grid_get_rect(view->source_grid);
  if (!ge_rect_within_rect(source_rect, new_rect)) {
    GE_LOG_ERROR("Scroll view exeeds bounds of source grid");
    abort();
  }
  view->sc_rect = new_rect;
}

void ge_sc_view_scroll_to_y_abs(ge_sc_view_t* view, double y)
{
  // IEEE 754 doubles can exactly represent up to 52-bit integers
  const ptrdiff_t yf = floor(y);
  const ge_coord_t new_coord = {view->sc_rect.min_coord.x, yf};
  const ge_rect_t new_rect = ge_rect_set_coord(view->sc_rect, new_coord);
  const ge_rect_t source_rect = ge_grid_get_rect(view->source_grid);
  if (!ge_rect_within_rect(source_rect, new_rect)) {
    GE_LOG_ERROR("Scroll view exeeds bounds of source grid");
    abort();
  }
  view->sc_rect = new_rect;
}

void ge_sc_view_scroll_to_x_pcnt(ge_sc_view_t* view, double percent_x)
{
  const double ratio_x = fmax(0.0, fmin(percent_x / 100.0, 1.0));
  const size_t delta_x = ge_grid_get_width(view->source_grid) - ge_rect_get_width(view->sc_rect);
  ge_sc_view_scroll_to_x_abs(view, round(ratio_x * delta_x));
}

void ge_sc_view_scroll_to_y_pcnt(ge_sc_view_t* view, double percent_y)
{
  const double ratio_y = fmax(0.0, fmin(percent_y / 100.0, 1.0));
  const size_t delta_y = ge_grid_get_height(view->source_grid) - ge_rect_get_height(view->sc_rect);
  ge_sc_view_scroll_to_y_abs(view, round(ratio_y * delta_y));
}
