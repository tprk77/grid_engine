// Copyright (c) 2019 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#ifndef GE_ERROR_H_
#define GE_ERROR_H_

typedef enum ge_error {
  GE_OK = 0,
  GE_ERROR_BAD_GRID_SIZE,
  GE_ERROR_BAD_GRID_MEMORY,
  GE_ERROR_ALREADY_HAS_WINDOW,
  GE_ERROR_NO_WINDOW,
  GE_ERROR_BAD_DISPLAY,
} ge_error_t;

#endif  // GE_ERROR_H_
