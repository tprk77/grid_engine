// Copyright (c) 2019 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#include "grid_engine/grid_engine.h"

int main(void)
{
  const size_t width = 256;
  const size_t height = 100;
  ge_grid_t* grid = ge_grid_create(width, height);
  ge_palette_t palette = GE_PALETTE_INFERNO;
  // Fill the grid with a gradient
  for (size_t x = 0; x < width; ++x) {
    for (size_t y = 0; y < height; ++y) {
      ge_grid_set_coord(grid, (ge_coord_t){x, y}, x);
    }
  }
  // The EZ loop data
  ez_loop_data_t ez_loop_data = {
      .grid = grid,
      .palette = &palette,
      .user_data = NULL,
      .loop_func = NULL,
      .event_func = NULL,
  };
  // RUN THE LOOP!
  const int result = ge_ez_loop(&ez_loop_data);
  ge_grid_free(grid);
  return result;
}
