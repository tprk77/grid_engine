// Copyright (c) 2019 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#ifndef GE_ENGINE_H_
#define GE_ENGINE_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "grid_engine/error.h"

// TODO Other graphics options like fullscreen
typedef struct ge_gfx_opts {
  size_t pixel_multiplier;
  const char* window_name;
} ge_gfx_opts_t;

// TODO Implement events
typedef enum ge_event_type {
  GE_EVENT_NONE = 0,
  GE_EVENT_KEYDOWN,
  GE_EVENT_KEYUP,
} ge_event_type_t;

// TODO Implement events
typedef struct ge_event {
  ge_event_type_t type;
} ge_event_t;

// clang-format off
#define GE_GFX_OPTS_DEFAULTS_K { \
    .pixel_multiplier = 8, \
    .window_name = "Grid Engine", \
}
// clang-format on

extern const ge_gfx_opts_t GE_GFX_OPTS_DEFAULTS;

void ge_init(void);
void ge_quit(void);
ge_error_t ge_set_data(size_t width, size_t height, const uint8_t* restrict pixel_arr);
ge_error_t ge_set_gfx_opts(const ge_gfx_opts_t* restrict gfx_opts);
ge_error_t ge_create_window();
ge_error_t ge_destroy_window();
ge_error_t ge_redraw_window();
bool ge_poll_events(ge_event_t* restrict event);
bool ge_should_quit(void);
uint32_t ge_get_time_ms(void);
void ge_sleep_ms(uint32_t duration_ms);

#endif  // GE_ENGINE_H_
