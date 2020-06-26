// Copyright (c) 2019 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#ifndef GE_EZ_LOOP_H_
#define GE_EZ_LOOP_H_

#include "grid_engine/engine.h"

/**
 * The "EZ" update function type. See `ge_ez_loop` for usage.
 */
typedef void (*ge_ez_loop_func_t)(ge_grid_t* grid, void* user_data, uint32_t time_ms);

/**
 * The "EZ" event handler function type. See `ge_ez_loop` for usage.
 */
typedef void (*ge_ez_event_func_t)(ge_grid_t* grid, void* user_data, uint32_t time_ms,
                                   const ge_event_t* event);

/**
 * The data required to run the EZ loop. See `ge_ez_loop` for usage.
 */
typedef struct ez_loop_data {
  ge_grid_t* grid;
  void* user_data;
  ge_ez_loop_func_t loop_func;
  ge_ez_event_func_t event_func;
} ez_loop_data_t;

/**
 * Run an "EZ" version of the grid engine. You initialize the loop with the
 * width, height, a pointer to the pixel array, and a pointer to the user data.
 * (The user data may be a pointer to anything the user wants, they just need to
 * cast it to a void pointer.) The EZ loop takes care of the rest!
 *
 * The loop function will be called periodically by the loop. It's responsible
 * for updating the pixel array in whatever way the user sees fit. The loop
 * function receives the pointer to the grid, the pointer to the user data, and
 * the current time step in milliseconds. The grid and user data are the same
 * values that were passed to `ge_ez_loop`. The time step can be used in pixel
 * calculations, to find a constant rate of change.
 *
 * The event handler function is optional and will be called for each event that
 * occurs. It's responsible for handling and reacting to events in whatever way
 * the user sees fit. Events are things like keypresses, etc, see "event.h" for
 * details. The event handler function receives the pointer to the grid, the
 * pointer to the user data, and the current time step in milliseconds (just
 * like the loop function), as well as the pointer to the event.
 */
int ge_ez_loop(const ez_loop_data_t* ez_loop_data);

#endif  // GE_EZ_LOOP_H_
