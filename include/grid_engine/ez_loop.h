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
                                  void* restrict user_data, uint32_t time_ms);

/**
 * The "EZ" event handler function type. See `ge_ez_loop` for usage.
 */
typedef void (*ge_ez_event_func_t)(const ge_event_t* restrict event, void* restrict user_data,
                                   uint32_t time_ms);

/**
 * Run an "EZ" version of the grid engine. You initialize the loop with the
 * width, height, a pointer to the pixel array, and a pointer to the user data.
 * (The user data may be a pointer to anything the user wants, they just need to
 * cast it to a void pointer.)
 *
 * The second to last argument, the update function, will be called periodically
 * by the loop. It's responsible for updating the pixel array in whatever way
 * the user sees fit. The update function receives the width, height, pointer to
 * the pixel array, and the pointer to the user data, the same values that were
 * passed to `ge_ez_loop`. Additionally, the update function is also passed the
 * current time step in milliseconds, which can be used in pixel calculations.
 *
 * The last argument, the event handler function, is optional and will be called
 * for each event that occurs. It's responsible for handling and reacting to
 * events in whatever way the user sees fit. Events are things like keypresses,
 * etc, see "event.h" for details. The event handler function receives the
 * pointer to the event, pointer to the user data, and the current time step in
 * milliseconds, just like the update function.
 */
int ge_ez_loop(size_t width, size_t height, uint8_t* restrict pixel_arr, void* restrict user_data,
               ge_ez_loop_func_t loop_func, ge_ez_event_func_t event_func);

#endif  // GE_EZ_LOOP_H_
