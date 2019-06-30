// Copyright (c) 2019 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#include "grid_engine/ez_loop.h"

#include <stdio.h>

#include "grid_engine/log.h"

// Target 60 frames per second: 1000 ms / 60 loops = 17 ms / loop
static const uint32_t TARGET_LOOP_MS = 17;

int ge_ez_loop(const ez_loop_data_t* const restrict ez_loop_data)
{
  const size_t width = ez_loop_data->width;
  const size_t height = ez_loop_data->height;
  uint8_t* const restrict pixel_arr = ez_loop_data->pixel_arr;
  void* const restrict user_data = ez_loop_data->user_data;
  const ge_ez_loop_func_t loop_func = ez_loop_data->loop_func;
  const ge_ez_event_func_t event_func = ez_loop_data->event_func;
  if (ge_init() != GE_OK) {
    GE_LOG_ERROR("Cannot initialize");
    return 1;
  }
  if (ge_set_data(width, height, pixel_arr) != GE_OK) {
    GE_LOG_ERROR("Cannot set grid data");
    return 1;
  }
  if (ge_create_window() != GE_OK) {
    GE_LOG_ERROR("Cannot create window");
    return 1;
  }
  while (!ge_should_quit()) {
    const uint32_t loop_start_ms = ge_get_time_ms();
    ge_event_t event;
    while (ge_poll_events(&event)) {
      if (event_func != NULL) {
        event_func(&event, user_data, loop_start_ms);
      }
    }
    if (loop_func != NULL) {
      loop_func(width, height, pixel_arr, user_data, loop_start_ms);
    }
    if (ge_redraw_window() != GE_OK) {
      GE_LOG_ERROR("Cannot draw window");
      return 1;
    }
    const uint32_t loop_end_ms = ge_get_time_ms();
    const uint32_t delta_time_ms = (loop_end_ms > loop_start_ms ? loop_end_ms - loop_start_ms : 0);
    const uint32_t delay_ms = (TARGET_LOOP_MS > delta_time_ms ? TARGET_LOOP_MS - delta_time_ms : 0);
    ge_sleep_ms(delay_ms);
  }
  if (ge_destroy_window() != GE_OK) {
    GE_LOG_ERROR("Cannot destroy window");
    return 1;
  }
  ge_quit();
  return 0;
}
