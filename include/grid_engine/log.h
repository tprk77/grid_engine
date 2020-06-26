// Copyright (c) 2019 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#ifndef GE_LOG_H_
#define GE_LOG_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef enum ge_log_level {
  GE_LOG_LEVEL_ERROR,
  GE_LOG_LEVEL_WARN,
  GE_LOG_LEVEL_INFO,
  GE_LOG_LEVEL_DEBUG,
} ge_log_level_t;

void ge_log(ge_log_level_t log_level, const char* format, ...);

#define GE_LOG_ERROR(...) ge_log(GE_LOG_LEVEL_ERROR, __VA_ARGS__)
#define GE_LOG_WARN(...) ge_log(GE_LOG_LEVEL_WARN, __VA_ARGS__)
#define GE_LOG_INFO(...) ge_log(GE_LOG_LEVEL_INFO, __VA_ARGS__)
#define GE_LOG_DEBUG(...) ge_log(GE_LOG_LEVEL_DEBUG, __VA_ARGS__)

#endif  // GE_LOG_H_
