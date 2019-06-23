// Copyright (c) 2019 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#ifndef GE_ENGINE_H_
#define GE_ENGINE_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef enum ge_event_type {
  GE_EVENT_NONE = 0,
  GE_EVENT_KEYDOWN,
  GE_EVENT_KEYUP,
  // TODO?
} ge_event_type_t;

typedef struct ge_event {
  ge_event_type_t type;
  // TODO?
} ge_event_t;

void ge_init(void);
void ge_quit(void);
bool ge_poll_events(ge_event_t* restrict event);
bool ge_should_quit(void);
uint32_t ge_get_time_ms(void);
void ge_sleep_ms(uint32_t duration_ms);

#endif  // GE_ENGINE_H_
