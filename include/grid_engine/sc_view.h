// Copyright (c) 2021 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#ifndef GE_SC_VIEW_H_
#define GE_SC_VIEW_H_

#include <stddef.h>
#include <stdint.h>

#include "grid_engine/grid.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ge_sc_view ge_sc_view_t;

/**
 * Create a new view. The view must eventually be freed.
 *
 * @param width The width of the view.
 * @param height The height of the view.
 * @param pixel_multiplier How much to scale the source grid.
 * @param source_grid The grid which the view will display.
 * @return The newly created view.
 */
ge_sc_view_t* ge_sc_view_create(size_t width, size_t height, size_t pixel_multiplier,
                                const ge_grid_t* source_grid);

/**
 * Free the view.
 */
void ge_sc_view_free(ge_sc_view_t* view);

/**
 * Refresh the contents of the view's render grid.
 */
void ge_sc_view_refresh(ge_sc_view_t* view);

/**
 * Get the view's render grid.
 */
ge_grid_t* ge_sc_view_get_render_grid(ge_sc_view_t* view);

/**
 * Resize the view, not affecting the source grid, and get the new render grid.
 * The old render grid is freed and can no longer be used.
 *
 * @param view The scroll view.
 * @param width The new width of the view.
 * @param height The new height of the view.
 * @return The view's new render grid.
 */
ge_grid_t* ge_sc_view_resize(ge_sc_view_t* view, size_t width, size_t height);

/**
 * Get the maximum absolute X position available to scroll. This is basically
 * the width of the original grid minus the size of the scroll view. It may be a
 * fractional value to indicate subpixels. (The minimum value is always zero.)
 *
 * @param view The scroll view.
 * @return The maximum absolute X position.
 */
double ge_sc_view_get_max_x_abs_scroll(const ge_sc_view_t* view);

/**
 * Get the maximum absolute Y position available to scroll. This is basically
 * the height of the original grid minus the size of the scroll view. It may be a
 * fractional value to indicate subpixels. (The minimum value is always zero.)
 *
 * @param view The scroll view.
 * @return The maximum absolute Y position.
 */
double ge_sc_view_get_max_y_abs_scroll(const ge_sc_view_t* view);

/**
 * Scroll the view to an absolute X position.
 *
 * @param view The scroll view.
 * @param x The absolute X position for the corner of the view.
 */
void ge_sc_view_scroll_to_x_abs(ge_sc_view_t* view, double x);

/**
 * Scroll the view to an absolute Y position.
 *
 * @param view The scroll view.
 * @param y The absolute Y position for the corner of the view.
 */
void ge_sc_view_scroll_to_y_abs(ge_sc_view_t* view, double y);

/**
 * Scroll the view to a percentage of it's X position, where 0.0 is the
 * left-most position, and 100.0 is the right-most position.
 *
 * @param view The scroll view.
 * @param percent_x The percent X position of the view.
 */
void ge_sc_view_scroll_to_x_pcnt(ge_sc_view_t* view, double percent_x);

/**
 * Scroll the view to a percentage of it's Y position, where 0.0 is the top-most
 * position, and 100.0 is the bottom-most position.
 *
 * @param view The scroll view.
 * @param percent_y The percent Y position of the view.
 */
void ge_sc_view_scroll_to_y_pcnt(ge_sc_view_t* view, double percent_y);

#ifdef __cplusplus
}
#endif

#endif  // GE_SC_VIEW_H_
