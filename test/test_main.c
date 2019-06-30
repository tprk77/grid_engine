// Copyright (c) 2019 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "grid_engine/grid_engine.h"

// These functions assume a fixed grid size of 100 x 100
void ushift_columns(uint8_t pixel_arr[100][100], uint8_t pixel_buff[100]);
void dshift_columns(uint8_t pixel_arr[100][100], uint8_t pixel_buff[100]);
void lshift_columns(uint8_t pixel_arr[100][100], uint8_t pixel_buff[100]);
void rshift_columns(uint8_t pixel_arr[100][100], uint8_t pixel_buff[100]);

// Target 60 frames per second: 1000 ms / 60 loops = 17 ms / loop
static const uint32_t TARGET_LOOP_MS = 17;

int main(void)
{
  if (ge_init() != GE_OK) {
    GE_LOG_ERROR("Cannot initialize!");
    return 1;
  }

  uint8_t pixel_arr[100][100];
  uint8_t pixel_buff[100];

  if (ge_set_data(100, 100, (uint8_t*) pixel_arr) != GE_OK) {
    GE_LOG_ERROR("Cannot set grid data!");
    return 1;
  }

  if (ge_create_window() != GE_OK) {
    GE_LOG_ERROR("Cannot create window");
    return 1;
  }

  GE_LOG_INFO("Loop started!");
  GE_LOG_DEBUG("Hello, hello!");

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
      if (event.type == GE_EVENT_KEYDOWN) {
        GE_LOG_INFO("KEYDOWN: %s", ge_keycode_to_str(event.keydown_data.keycode));
      }
      else if (event.type == GE_EVENT_KEYUP) {
        GE_LOG_INFO("KEYUP: %s", ge_keycode_to_str(event.keydown_data.keycode));
      }
      if (event.type == GE_EVENT_KEYDOWN) {
        if (event.keydown_data.keycode == GE_KEYCODE_UP) {
          ushift_columns(pixel_arr, pixel_buff);
        }
        else if (event.keydown_data.keycode == GE_KEYCODE_DOWN) {
          dshift_columns(pixel_arr, pixel_buff);
        }
        else if (event.keydown_data.keycode == GE_KEYCODE_LEFT) {
          lshift_columns(pixel_arr, pixel_buff);
        }
        else if (event.keydown_data.keycode == GE_KEYCODE_RIGHT) {
          rshift_columns(pixel_arr, pixel_buff);
        }
      }
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

  GE_LOG_INFO("Loop done!");

  if (ge_destroy_window() != GE_OK) {
    GE_LOG_ERROR("Cannot destroy window");
    return 1;
  }

  ge_quit();

  return 0;
}

void ushift_columns(uint8_t pixel_arr[100][100], uint8_t pixel_buff[100])
{
  // Copy to first row to buffer
  for (size_t ii = 0; ii < 100; ++ii) {
    pixel_buff[ii] = pixel_arr[0][ii];
  }
  // Copy all rows to the one before
  for (size_t jj = 1; jj < 100; ++jj) {
    for (size_t ii = 0; ii < 100; ++ii) {
      pixel_arr[jj - 1][ii] = pixel_arr[jj][ii];
    }
  }
  // Copy buffer to last row
  for (size_t ii = 0; ii < 100; ++ii) {
    pixel_arr[99][ii] = pixel_buff[ii];
  }
}

void dshift_columns(uint8_t pixel_arr[100][100], uint8_t pixel_buff[100])
{
  // Copy to last row to buffer
  for (size_t ii = 0; ii < 100; ++ii) {
    pixel_buff[ii] = pixel_arr[99][ii];
  }
  // Copy all rows to the one before
  for (size_t jj = 1; jj < 100; ++jj) {
    for (size_t ii = 0; ii < 100; ++ii) {
      pixel_arr[100 - jj][ii] = pixel_arr[100 - jj - 1][ii];
    }
  }
  // Copy buffer to last row
  for (size_t ii = 0; ii < 100; ++ii) {
    pixel_arr[0][ii] = pixel_buff[ii];
  }
}

void lshift_columns(uint8_t pixel_arr[100][100], uint8_t pixel_buff[100])
{
  // Copy to first column to buffer
  for (size_t jj = 0; jj < 100; ++jj) {
    pixel_buff[jj] = pixel_arr[jj][0];
  }
  // Copy all columns to the one before
  for (size_t jj = 0; jj < 100; ++jj) {
    for (size_t ii = 1; ii < 100; ++ii) {
      pixel_arr[jj][ii - 1] = pixel_arr[jj][ii];
    }
  }
  // Copy buffer to last column
  for (size_t jj = 0; jj < 100; ++jj) {
    pixel_arr[jj][99] = pixel_buff[jj];
  }
}

void rshift_columns(uint8_t pixel_arr[100][100], uint8_t pixel_buff[100])
{
  // Copy to last column to buffer
  for (size_t jj = 0; jj < 100; ++jj) {
    pixel_buff[jj] = pixel_arr[jj][99];
  }
  // Copy all columns to the one after
  for (size_t jj = 0; jj < 100; ++jj) {
    for (size_t ii = 1; ii < 100; ++ii) {
      pixel_arr[jj][100 - ii] = pixel_arr[jj][100 - ii - 1];
    }
  }
  // Copy buffer to first column
  for (size_t jj = 0; jj < 100; ++jj) {
    pixel_arr[jj][0] = pixel_buff[jj];
  }
}
