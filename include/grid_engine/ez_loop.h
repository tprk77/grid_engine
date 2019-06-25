// Copyright (c) 2019 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#ifndef GE_EZ_LOOP_H_
#define GE_EZ_LOOP_H_

#include "grid_engine/engine.h"

/**
 * The "EZ" update function type. See `ge_ez_loop` for usage.
 */
typedef void (*ge_ez_loop_func_t)(size_t width, size_t height, uint8_t* restrict pixel_arr,
                                  void* user_data, uint32_t time_ms);

/**
 * Run an "EZ" version of the grid engine. You initialize the loop with the
 * width, height, a pointer to the pixel array, and a pointer to the user data.
 * (The user data may be a pointer to anything the user wants, they just need to
 * cast it to a void pointer.)
 *
 * The final argument, the update function, will be called periodically by the
 * loop. It's responsible for updating the pixel array in whatever way the user
 * sees fit. The update function receives the width, height, pointer to the
 * pixel array, and pointer to the user data, the same values that were passed
 * to `ge_ez_loop`. Additionally, the update function is also passed the current
 * time step in milliseconds, which can be used in pixel calculations.
 */
int ge_ez_loop(size_t width, size_t height, uint8_t* restrict pixel_arr, void* user_data,
               ge_ez_loop_func_t loop_func);

#endif  // GE_EZ_LOOP_H_
