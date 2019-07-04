// Copyright (c) 2019 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#include <stdio.h>
#include <stdlib.h>

#include "grid_engine/grid_engine.h"

typedef struct user_data {
  ge_coord_t coord;
} user_data_t;

void test_loop_func(ge_grid_t* restrict grid, void* restrict user_data_, uint32_t time_ms)
{
  // Cast the user data back to the right type
  user_data_t* user_data = (user_data_t*) user_data_;
  // Get the data from the grid for convenience
  const size_t width = ge_grid_get_width(grid);
  const size_t height = ge_grid_get_height(grid);
  // Calculate the values!
  const uint64_t time_s = time_ms / 1000;
  const bool odd_second = time_s % 2 != 0;
  for (size_t j = 0; j < height; ++j) {
    for (size_t i = 0; i < width; ++i) {
      const bool pixel_on = ((i % 2 == 0) ^ (j % 2 == 0)) ^ odd_second;
      const uint8_t pixel_value = (pixel_on ? 255.0f * (1.0f - (i / 100.0f)) : 0);
      ge_grid_set_coord(grid, (ge_coord_t){i, j}, pixel_value);
    }
  }
  // Invert the user pixel
  const size_t user_x = user_data->coord.x;
  const size_t user_y = user_data->coord.y;
  const bool user_on = !(((user_x % 2 == 0) ^ (user_y % 2 == 0)) ^ odd_second);
  const uint8_t user_value = (user_on ? 255.0f * (1.0f - (user_x / 100.0f)) : 0);
  ge_grid_set_coord(grid, user_data->coord, user_value);
}

void test_event_func(ge_grid_t* restrict grid, void* restrict user_data_, uint32_t time_ms,
                     const ge_event_t* restrict event)
{
  // Time is unused here
  (void) time_ms;
  // Cast the user data back to the right type
  user_data_t* user_data = (user_data_t*) user_data_;
  const size_t width = ge_grid_get_width(grid);
  const size_t height = ge_grid_get_height(grid);
  // Process arrow keys to get the direction to move in
  ge_coord_t offset_dir = {0, 0};
  if (event->type == GE_EVENT_KEYDOWN) {
    if (event->keydown_data.keycode == GE_KEYCODE_UP) {
      offset_dir.y = -1;
    }
    else if (event->keydown_data.keycode == GE_KEYCODE_DOWN) {
      offset_dir.y = 1;
    }
    else if (event->keydown_data.keycode == GE_KEYCODE_LEFT) {
      offset_dir.x = -1;
    }
    else if (event->keydown_data.keycode == GE_KEYCODE_RIGHT) {
      offset_dir.x = 1;
    }
  }
  // Actually move the coordinate with wrapping
  user_data->coord = ge_coord_add(user_data->coord, offset_dir);
  user_data->coord = ge_coord_wrap(user_data->coord, width, height);
}

int main(void)
{
  // Memory is in row-major order
  ge_grid_t* grid = ge_grid_create(100, 50);
  // User data to track the inverted pixel
  user_data_t user_data = {.coord = (ge_coord_t){50, 25}};
  // The EZ loop data
  ez_loop_data_t ez_loop_data = {
      .grid = grid,
      .user_data = &user_data,
      .loop_func = test_loop_func,
      .event_func = test_event_func,
  };
  // RUN THE LOOP!
  return ge_ez_loop(&ez_loop_data);
}
