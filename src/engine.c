// Copyright (c) 2019 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#include "grid_engine/engine.h"

#include "SDL2/SDL.h"

static const ge_event_t GE_EVENT_DEFAULTS = {GE_EVENT_NONE};

static void log_line(const char* restrict format, ...);

static bool ge_should_quit_flag = false;

void ge_init(void)
{
  log_line("Grid engine initializing!\n");
  SDL_Init(SDL_INIT_VIDEO);
}

void ge_quit(void)
{
  log_line("Grid engine is exiting!\n");
  SDL_Quit();
}

bool ge_poll_events(ge_event_t* restrict event)
{
  *event = GE_EVENT_DEFAULTS;
  SDL_Event sdl_event;
  bool got_event = SDL_PollEvent(&sdl_event);
  if (!got_event) {
    return false;
  }
  if (sdl_event.type == SDL_QUIT) {
    log_line("Going to quit!\n");
    ge_should_quit_flag = true;
  }
  return false;
}

bool ge_should_quit(void)
{
  return ge_should_quit_flag;
}

uint32_t ge_get_time_ms(void)
{
  return SDL_GetTicks();
}

void ge_sleep_ms(uint32_t duration_ms)
{
  SDL_Delay(duration_ms);
}

static void log_line(const char* restrict format, ...)
{
  va_list vlist;
  va_start(vlist, format);
  vfprintf(stderr, format, vlist);
  va_end(vlist);
}
