// Copyright (c) 2019 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#ifndef GE_ERROR_H_
#define GE_ERROR_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum ge_error {
  GE_OK = 0,
  GE_ERROR_ALREADY_INITED,
  GE_ERROR_ENGINE_INIT,
  GE_ERROR_NOT_INITED,
  GE_ERROR_NO_GRID_SET,
  GE_ERROR_ALREADY_HAS_WINDOW,
  GE_ERROR_CREATE_WINDOW,
  GE_ERROR_NO_WINDOW,
  GE_ERROR_DRAWING,
} ge_error_t;

#ifdef __cplusplus
}
#endif

#endif  // GE_ERROR_H_
