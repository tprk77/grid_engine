// Copyright (c) 2019 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "grid_engine/grid_engine.h"

typedef struct user_data {
  size_t last_update_time_s;
  ge_grid_t* temp_grid;
} user_data_t;

void conway_loop_func(ge_grid_t* restrict grid, void* restrict user_data_, uint32_t time_ms)
{
  // Cast the user data back to the right type
  user_data_t* user_data = (user_data_t*) user_data_;
  // Check the time so we only update once a second
  const uint64_t time_s = time_ms / 1000;
  if (time_s == user_data->last_update_time_s) {
    return;
  }
  user_data->last_update_time_s = time_s;
  // Get width, height, etc
  const size_t width = ge_grid_get_width(grid);
  const size_t height = ge_grid_get_height(grid);
  // Compute the new grid from the old grid
  for (size_t jj = 0; jj < height; ++jj) {
    for (size_t ii = 0; ii < width; ++ii) {
      const ge_coord_t coord = (ge_coord_t){ii, jj};
      const bool is_live = (ge_grid_get_coord(grid, coord) != 0);
      const ge_neighbor_res_t neighbor_res = ge_grid_get_neighbors_wrapped(grid, coord);
      size_t num_live_neighbors = 0;
      for (size_t kk = 0; kk < neighbor_res.num_neighbors; ++kk) {
        if (ge_grid_get_coord(grid, neighbor_res.neighbors[kk])) {
          ++num_live_neighbors;
        }
      }
      // Apply the Game Of Life rules
      if (is_live) {
        if (num_live_neighbors < 2 || num_live_neighbors > 3) {
          ge_grid_set_coord(user_data->temp_grid, coord, 0);
        }
        else {
          ge_grid_set_coord(user_data->temp_grid, coord, 255);
        }
      }
      else {
        if (num_live_neighbors == 3) {
          ge_grid_set_coord(user_data->temp_grid, coord, 255);
        }
        else {
          ge_grid_set_coord(user_data->temp_grid, coord, 0);
        }
      }
    }
  }
  // Swap the results with the current grid
  ge_grid_copy_pixel_arr(grid, user_data->temp_grid);
  ge_grid_clear_pixel_arr(user_data->temp_grid);
}

int main(void)
{
  ge_grid_t* grid = ge_grid_create(100, 100);
  uint8_t(*pixel_arr)[100] = (uint8_t(*)[100]) ge_grid_get_pixel_arr_mut(grid);
  // Make an X pattern
  for (size_t j = 0; j < 100; ++j) {
    for (size_t i = 0; i < 100; ++i) {
      if (i == j || (99 - i) == j) {
        pixel_arr[j][i] = 255;
      }
    }
  }
  // User data to track state, etc
  user_data_t user_data = {
      .last_update_time_s = 0,
      .temp_grid = ge_grid_create(100, 100),
  };
  // The EZ loop data
  ez_loop_data_t ez_loop_data = {
      .grid = grid,
      .user_data = &user_data,
      .loop_func = conway_loop_func,
      .event_func = NULL,
  };
  // RUN THE LOOP!
  return ge_ez_loop(&ez_loop_data);
}
