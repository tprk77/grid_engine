// Copyright (c) 2019 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#include <stdio.h>
#include <stdlib.h>

#include "grid_engine/grid_engine.h"

#define WIDTH 100
#define HEIGHT 50

void test_loop_func(size_t width, size_t height, uint8_t* restrict pixel_arr_, void* user_data,
                    uint32_t time_ms)
{
  // User data is unused
  (void) user_data;
  // Cast the pixel_arr back to an easy to use type (optional)
  uint8_t(*pixel_arr)[WIDTH] = (uint8_t(*)[WIDTH]) pixel_arr_;
  // Calculate the values!
  uint64_t time_s = time_ms / 1000;
  bool odd_second = time_s % 2 != 0;
  for (size_t j = 0; j < height; ++j) {
    for (size_t i = 0; i < width; ++i) {
      bool on_pixel = ((i % 2 == 0) ^ (j % 2 == 0)) ^ odd_second;
      pixel_arr[j][i] = (on_pixel ? 255.0f * (1.0f - (i / 100.0f)) : 0);
    }
  }
}

int main(void)
{
  // Memory is in row-major order
  uint8_t pixel_arr[HEIGHT][WIDTH];

  return ge_ez_loop(WIDTH, HEIGHT, (uint8_t*) pixel_arr, NULL, test_loop_func);
}
