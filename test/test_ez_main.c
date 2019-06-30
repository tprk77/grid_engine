// Copyright (c) 2019 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#include <stdio.h>
#include <stdlib.h>

#include "grid_engine/grid_engine.h"

#define WIDTH 100
#define HEIGHT 50

typedef struct user_data {
  size_t x;
  size_t y;
} user_data_t;

void test_loop_func(size_t width, size_t height, uint8_t* restrict pixel_arr_,
                    void* restrict user_data_, uint32_t time_ms)
{
  // Cast the userdata back to the right type
  user_data_t* user_data = (user_data_t*) user_data_;
  // Cast the pixel_arr back to an easy to use type (optional)
  uint8_t(*pixel_arr)[WIDTH] = (uint8_t(*)[WIDTH]) pixel_arr_;
  // Calculate the values!
  const uint64_t time_s = time_ms / 1000;
  const bool odd_second = time_s % 2 != 0;
  for (size_t j = 0; j < height; ++j) {
    for (size_t i = 0; i < width; ++i) {
      const bool on_pixel = ((i % 2 == 0) ^ (j % 2 == 0)) ^ odd_second;
      pixel_arr[j][i] = (on_pixel ? 255.0f * (1.0f - (i / 100.0f)) : 0);
    }
  }
  // Invert the user pixel
  const size_t user_x = user_data->x;
  const size_t user_y = user_data->y;
  const bool user_pixel = !(((user_x % 2 == 0) ^ (user_y % 2 == 0)) ^ odd_second);
  pixel_arr[user_y][user_x] = (user_pixel ? 255.0f * (1.0f - (user_x / 100.0f)) : 0);
}

void test_event_func(const ge_event_t* restrict event, void* restrict user_data_, uint32_t time_ms)
{
  // Time is unused here
  (void) time_ms;
  // Cast the userdata back to the right type
  user_data_t* user_data = (user_data_t*) user_data_;
  if (event->type == GE_EVENT_KEYDOWN) {
    if (event->keydown_data.keycode == GE_KEYCODE_UP) {
      user_data->y = (user_data->y == 0 ? HEIGHT : (user_data->y - 1) % HEIGHT);
    }
    else if (event->keydown_data.keycode == GE_KEYCODE_DOWN) {
      user_data->y = (user_data->y + 1) % HEIGHT;
    }
    else if (event->keydown_data.keycode == GE_KEYCODE_LEFT) {
      user_data->x = (user_data->x == 0 ? WIDTH : (user_data->x - 1) % WIDTH);
    }
    else if (event->keydown_data.keycode == GE_KEYCODE_RIGHT) {
      user_data->x = (user_data->x + 1) % WIDTH;
    }
  }
}

int main(void)
{
  // Memory is in row-major order
  uint8_t pixel_arr[HEIGHT][WIDTH];
  // Userdata to track the inverted pixel
  user_data_t user_data = {.x = WIDTH / 2, .y = HEIGHT / 2};
  // The EZ loop data
  ez_loop_data_t ez_loop_data = {
      .width = WIDTH,
      .height = HEIGHT,
      .pixel_arr = (uint8_t*) &pixel_arr,
      .user_data = &user_data,
      .loop_func = test_loop_func,
      .event_func = test_event_func,
  };
  // RUN THE LOOP!
  return ge_ez_loop(&ez_loop_data);
}
