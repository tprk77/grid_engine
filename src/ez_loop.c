// Copyright (c) 2019 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#include "grid_engine/ez_loop.h"

#include <stdio.h>

#include "grid_engine/log.h"

// Target 60 frames per second: 1000 ms / 60 loops = 17 ms / loop
static const uint32_t TARGET_LOOP_MS = 17;

int ge_ez_loop(const ez_loop_data_t* const ez_loop_data)
{
  ge_grid_t* const grid = ez_loop_data->grid;
  const ge_palette_t* const palette = ez_loop_data->palette;
  void* const user_data = ez_loop_data->user_data;
  const ge_ez_loop_func_t loop_func = ez_loop_data->loop_func;
  const ge_ez_event_func_t event_func = ez_loop_data->event_func;
  if (ge_init() != GE_OK) {
    GE_LOG_ERROR("Cannot initialize");
    return 1;
  }
  if (ge_set_grid(grid) != GE_OK) {
    GE_LOG_ERROR("Cannot set grid data");
    return 1;
  }
  if (ge_set_palette(palette) != GE_OK) {
    GE_LOG_ERROR("Cannot set palette");
    return 1;
  }
  ge_gfx_opts_t gfx_opts = GE_GFX_OPTS_DEFAULTS;
  gfx_opts.pixel_multiplier = ge_auto_detect_pixel_multiplier();
  if (ge_set_gfx_opts(&gfx_opts) != GE_OK) {
    GE_LOG_ERROR("Cannot set graphics options");
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
        event_func(grid, user_data, loop_start_ms, &event);
      }
    }
    if (loop_func != NULL) {
      loop_func(grid, user_data, loop_start_ms);
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
