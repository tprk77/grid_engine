// Copyright (c) 2019 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#include "grid_engine/log.h"

#include "SDL2/SDL.h"

void ge_log(ge_log_level_t log_level, const char* restrict format, ...)
{
  SDL_LogPriority sdl_priority;
  switch (log_level) {
  case GE_LOG_LEVEL_ERROR:
    sdl_priority = SDL_LOG_PRIORITY_ERROR;
    break;
  case GE_LOG_LEVEL_WARN:
    sdl_priority = SDL_LOG_PRIORITY_WARN;
    break;
  case GE_LOG_LEVEL_INFO:
    sdl_priority = SDL_LOG_PRIORITY_INFO;
    break;
  case GE_LOG_LEVEL_DEBUG:
    sdl_priority = SDL_LOG_PRIORITY_DEBUG;
    break;
  }
  va_list vlist;
  va_start(vlist, format);
  SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, sdl_priority, format, vlist);
  va_end(vlist);
}
