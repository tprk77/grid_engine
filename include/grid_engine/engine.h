// Copyright (c) 2019 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#ifndef GE_ENGINE_H_
#define GE_ENGINE_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "grid_engine/error.h"
#include "grid_engine/event.h"
#include "grid_engine/grid.h"
#include "grid_engine/palette.h"

#ifdef __cplusplus
extern "C" {
#endif

// TODO Other graphics options like fullscreen
typedef struct ge_gfx_opts {
  size_t pixel_multiplier;
  const char* window_name;
} ge_gfx_opts_t;

#define GE_GFX_OPTS_DEFAULTS_K                           \
  {                                                      \
    .pixel_multiplier = 8, .window_name = "Grid Engine", \
  }

extern const ge_gfx_opts_t GE_GFX_OPTS_DEFAULTS;

ge_error_t ge_init(void);
void ge_quit(void);
ge_error_t ge_set_grid(const ge_grid_t* grid);
ge_error_t ge_set_palette(const ge_palette_t* palette);
ge_error_t ge_set_gfx_opts(const ge_gfx_opts_t* gfx_opts);
size_t ge_auto_detect_pixel_multiplier(void);
ge_error_t ge_create_window(void);
ge_error_t ge_destroy_window(void);
ge_error_t ge_redraw_window(void);
bool ge_poll_events(ge_event_t* event);
bool ge_should_quit(void);
uint32_t ge_get_time_ms(void);
void ge_sleep_ms(uint32_t duration_ms);

#ifdef __cplusplus
}
#endif

#endif  // GE_ENGINE_H_
