// Copyright (c) 2019 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#include <stdlib.h>

#include "grid_engine/grid_engine.h"

typedef struct user_data {
  ge_sc_view_t* view;
  uint32_t last_update_time_ms;
  double scroll_speed;  // percent/s
  ge_dir_t scroll_dir;
  double scroll_x;  // percent
  double scroll_y;  // percent
} user_data_t;

void scroll_loop_func(ge_grid_t* grid, void* user_data_, uint32_t time_ms)
{
  (void) grid;
  // Cast the user data back to the right type
  user_data_t* user_data = (user_data_t*) user_data_;
  // Get the change in time since last time
  const uint32_t dt_ms = time_ms - user_data->last_update_time_ms;
  const double dt_s = dt_ms / 1000.0;
  user_data->last_update_time_ms = time_ms;
  // Update the scroll position
  if (user_data->scroll_dir == GE_DIR_NORTH) {
    user_data->scroll_y -= user_data->scroll_speed * dt_s;
    user_data->scroll_y = user_data->scroll_y > 0.0 ? user_data->scroll_y : 0.0;
  }
  else if (user_data->scroll_dir == GE_DIR_SOUTH) {
    user_data->scroll_y += user_data->scroll_speed * dt_s;
    user_data->scroll_y = user_data->scroll_y < 100.0 ? user_data->scroll_y : 100.0;
  }
  else if (user_data->scroll_dir == GE_DIR_WEST) {
    user_data->scroll_x -= user_data->scroll_speed * dt_s;
    user_data->scroll_x = user_data->scroll_x > 0.0 ? user_data->scroll_x : 0.0;
  }
  else if (user_data->scroll_dir == GE_DIR_EAST) {
    user_data->scroll_x += user_data->scroll_speed * dt_s;
    user_data->scroll_x = user_data->scroll_x < 100.0 ? user_data->scroll_x : 100.0;
  }
  // Update the view
  ge_sc_view_scroll_to_x_pcnt(user_data->view, user_data->scroll_x);
  ge_sc_view_scroll_to_y_pcnt(user_data->view, user_data->scroll_y);
  ge_sc_view_refresh(user_data->view);
}

void scroll_event_func(ge_grid_t* grid, void* user_data_, uint32_t time_ms, const ge_event_t* event)
{
  (void) grid;
  (void) time_ms;
  // Cast the user data back to the right type
  user_data_t* user_data = (user_data_t*) user_data_;
  // Process key up and down events to get scroll direction
  if (event->type == GE_EVENT_KEYDOWN) {
    if (event->keydown_data.keycode == GE_KEYCODE_F
        || event->keydown_data.keycode == GE_KEYCODE_W) {
      user_data->scroll_dir = GE_DIR_NORTH;
    }
    else if (event->keydown_data.keycode == GE_KEYCODE_S) {
      user_data->scroll_dir = GE_DIR_SOUTH;
    }
    else if (event->keydown_data.keycode == GE_KEYCODE_R
             || event->keydown_data.keycode == GE_KEYCODE_A) {
      user_data->scroll_dir = GE_DIR_WEST;
    }
    else if (event->keydown_data.keycode == GE_KEYCODE_T
             || event->keydown_data.keycode == GE_KEYCODE_D) {
      user_data->scroll_dir = GE_DIR_EAST;
    }
  }
  else if (event->type == GE_EVENT_KEYUP) {
    if (event->keydown_data.keycode == GE_KEYCODE_F
        || event->keydown_data.keycode == GE_KEYCODE_W) {
      if (user_data->scroll_dir == GE_DIR_NORTH) user_data->scroll_dir = GE_DIR_NONE;
    }
    else if (event->keydown_data.keycode == GE_KEYCODE_S) {
      if (user_data->scroll_dir == GE_DIR_SOUTH) user_data->scroll_dir = GE_DIR_NONE;
    }
    else if (event->keydown_data.keycode == GE_KEYCODE_R
             || event->keydown_data.keycode == GE_KEYCODE_A) {
      if (user_data->scroll_dir == GE_DIR_WEST) user_data->scroll_dir = GE_DIR_NONE;
    }
    else if (event->keydown_data.keycode == GE_KEYCODE_T
             || event->keydown_data.keycode == GE_KEYCODE_D) {
      if (user_data->scroll_dir == GE_DIR_EAST) user_data->scroll_dir = GE_DIR_NONE;
    }
  }
}

int main(void)
{
  ge_grid_t* grid = ge_img_load("demos/assets/grid_engine.png");
  // Set up and refresh the view
  ge_sc_view_t* view = ge_sc_view_create(200, 200, 4, grid);
  ge_sc_view_refresh(view);
  // User data to track state, etc
  user_data_t user_data = {
      .view = view,
      .scroll_speed = 10.0,
      .scroll_dir = GE_DIR_NONE,
      .scroll_x = 0.0,
      .scroll_y = 0.0,
  };
  // The EZ loop data
  ez_loop_data_t ez_loop_data = {
      .grid = ge_sc_view_get_render_grid(view),
      .palette = NULL,
      .user_data = &user_data,
      .loop_func = scroll_loop_func,
      .event_func = scroll_event_func,
  };
  // RUN THE LOOP!
  const int result = ge_ez_loop(&ez_loop_data);
  ge_sc_view_free(view);
  ge_grid_free(grid);
  return result;
}
