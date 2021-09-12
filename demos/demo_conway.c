// Copyright (c) 2019 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#include <stdlib.h>

#include "grid_engine/grid_engine.h"

typedef struct user_data {
  size_t last_update_time_s;
  ge_grid_t* temp_grid;
} user_data_t;

bool gol_cell_live(bool is_live, size_t num_live_nbrs)
{
  if (is_live) {
    if (num_live_nbrs < 2 || num_live_nbrs > 3) {
      return false;
    }
    else {
      return true;
    }
  }
  else {
    if (num_live_nbrs == 3) {
      return true;
    }
    else {
      return false;
    }
  }
}

void conway_loop_func(ge_grid_t* grid, void* user_data_, uint32_t time_ms)
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
      const ge_nbrs_t nbrs = ge_grid_get_nbrs_wrapped(grid, coord);
      size_t num_live_nbrs = 0;
      ge_dir_t nbr_dir = GE_DIR_NONE;
      while ((nbr_dir = ge_nbrs_next_dir(&nbrs, nbr_dir)) != GE_DIR_NONE) {
        const ge_coord_t nbr_coord = ge_nbrs_get_nbr(&nbrs, nbr_dir);
        if (ge_grid_get_coord(grid, nbr_coord) != 0) {
          ++num_live_nbrs;
        }
      }
      // Apply the Game Of Life rules
      if (gol_cell_live(is_live, num_live_nbrs)) {
        ge_grid_set_coord(user_data->temp_grid, coord, 255);
      }
      else {
        ge_grid_set_coord(user_data->temp_grid, coord, 0);
      }
    }
  }
  // Swap the results with the current grid
  ge_grid_copy_pixel_arr(grid, user_data->temp_grid);
  ge_grid_clear_pixel_arr(user_data->temp_grid);
}

void draw_glider(ge_grid_t* grid, ge_coord_t origin_coord)
{
  const ge_coord_t glider_offsets[5] = {
      (ge_coord_t){2, 0}, (ge_coord_t){2, 1}, (ge_coord_t){2, 2},
      (ge_coord_t){1, 2}, (ge_coord_t){0, 1},
  };
  for (size_t ii = 0; ii < 5; ++ii) {
    ge_grid_set_coord_wrapped(grid, ge_coord_add(origin_coord, glider_offsets[ii]), 255);
  }
}

int main(void)
{
  const size_t width = 200;
  const size_t height = 100;
  ge_grid_t* grid = ge_grid_create(width, height);
  // Make a glider pattern
  for (size_t x = 0; x < width; x += 10) {
    for (size_t y = 0; y < height; y += 10) {
      draw_glider(grid, (ge_coord_t){x, y});
    }
  }
  // Draw some random stuff
  for (size_t ii = 0; ii < 1000; ++ii) {
    const size_t x = rand() % width;
    const size_t y = rand() % height;
    ge_grid_set_coord(grid, (ge_coord_t){x, y}, 255);
  }
  // User data to track state, etc
  user_data_t user_data = {
      .last_update_time_s = 0,
      .temp_grid = ge_grid_create(width, height),
  };
  // The EZ loop data
  ez_loop_data_t ez_loop_data = {
      .grid = grid,
      .palette = NULL,
      .user_data = &user_data,
      .loop_func = conway_loop_func,
      .event_func = NULL,
  };
  // RUN THE LOOP!
  const int result = ge_ez_loop(&ez_loop_data);
  ge_grid_free(grid);
  ge_grid_free(user_data.temp_grid);
  return result;
}
