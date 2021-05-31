// Copyright (c) 2019 Joey Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#include "grid_engine/grid_engine.h"

#include <stdbool.h>
#include <stdio.h>

typedef enum direction {
  DIRECTION_NORTH = 0,
  DIRECTION_EAST = 1,
  DIRECTION_SOUTH = 2,
  DIRECTION_WEST = 3,
} direction_t;

typedef struct user_data {
  size_t last_update_time_s;
  ge_grid_t* temp_grid;
  ge_coord_t ant_coords;
  direction_t orientation;
} user_data_t;

int turn(int direction, int live)
{
  if (live) {
    if (direction < DIRECTION_WEST) {
      return ++direction;
    }
    else {
      return DIRECTION_NORTH;
    }
  }
  else {
    if (direction > DIRECTION_NORTH) {
      return --direction;
    }
    else {
      return DIRECTION_WEST;
    }
  }
}

void langton_loop_func(ge_grid_t* grid, void* user_data_, uint32_t time_ms)
{
  bool ant_found = false;
  // Cast the user data back to the right type
  uint8_t val;
  user_data_t* user_data = (user_data_t*) user_data_;
  // Check the time so we only update once a second
  const uint64_t time_s = time_ms / 500;
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
      // Apply the Langton Ant rules
      if (coord.x == user_data->ant_coords.x && coord.y == user_data->ant_coords.y && !ant_found) {
        ant_found = true;
        user_data->orientation = turn(user_data->orientation, is_live);
        switch (user_data->orientation) {
        case DIRECTION_NORTH:
          user_data->ant_coords.y--;
          break;
        case DIRECTION_EAST:
          user_data->ant_coords.x++;
          break;
        case DIRECTION_SOUTH:
          user_data->ant_coords.y++;
          break;
        case DIRECTION_WEST:
          user_data->ant_coords.x--;
          break;
        }
        if (is_live)
          ge_grid_set_coord(user_data->temp_grid, coord, 0);
        else
          ge_grid_set_coord(user_data->temp_grid, coord, 255);
      }
      else {
        val = ge_grid_get_coord(grid, coord);
        ge_grid_set_coord(user_data->temp_grid, coord, val);
      }
    }
  }
  // Swap the results with the current grid
  ge_grid_copy_pixel_arr(grid, user_data->temp_grid);
  ge_grid_clear_pixel_arr(user_data->temp_grid);
}

int main(void)
{
  const size_t width = 100;
  const size_t height = 100;
  ge_grid_t* grid = ge_grid_create(width, height);
  // User data to track state, etc
  user_data_t user_data = {
      .last_update_time_s = 0,
      .temp_grid = ge_grid_create(width, height),
      .ant_coords = (ge_coord_t){49, 49},
      .orientation = DIRECTION_EAST,
  };
  // The EZ loop data
  ez_loop_data_t ez_loop_data = {
      .grid = grid,
      .palette = NULL,
      .user_data = &user_data,
      .loop_func = langton_loop_func,
      .event_func = NULL,
  };
  // RUN THE LOOP!
  const int result = ge_ez_loop(&ez_loop_data);
  ge_grid_free(grid);
  ge_grid_free(user_data.temp_grid);
  return result;
}
