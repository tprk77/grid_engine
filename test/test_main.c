// Copyright (c) 2019 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "grid_engine/grid_engine.h"

// Target 60 frames per second: 1000 ms / 60 loops = 17 ms / loop
static const uint32_t TARGET_LOOP_MS = 17;

int main(void)
{
  ge_init();

  uint8_t pixel_arr[100][100];

  ge_set_data(100, 100, (uint8_t*) pixel_arr);

  if (ge_create_window() != GE_OK) {
    fprintf(stderr, "Cannot create window\n");
    return 1;
  }

  fprintf(stderr, "Loop started!\n");

  // Copy some random memory
  uint8_t* uninited = malloc(100 * 100);
  memcpy(pixel_arr, uninited, 100 * 100);
  free(uninited);

  // Make an X pattern
  for (size_t j = 0; j < 100; ++j) {
    for (size_t i = 0; i < 100; ++i) {
      if (i == j || (99 - i) == j) {
        pixel_arr[j][i] = 255;
      }
    }
  }

  while (!ge_should_quit()) {
    const uint32_t loop_start_ms = ge_get_time_ms();
    ge_event_t event;
    while (ge_poll_events(&event)) {
      // TODO Handle events?
    }
    if (ge_redraw_window() != GE_OK) {
      fprintf(stderr, "Cannot draw window\n");
      return 1;
    }
    const uint32_t loop_end_ms = ge_get_time_ms();
    const uint32_t delta_time_ms = (loop_end_ms > loop_start_ms ? loop_end_ms - loop_start_ms : 0);
    const uint32_t delay_ms = (TARGET_LOOP_MS > delta_time_ms ? TARGET_LOOP_MS - delta_time_ms : 0);
    ge_sleep_ms(delay_ms);
  }

  if (ge_destroy_window() != GE_OK) {
    fprintf(stderr, "Cannot destroy window\n");
    return 1;
  }

  fprintf(stderr, "Loop done!\n");

  ge_quit();

  return 0;
}
