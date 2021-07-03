// Copyright (c) 2019 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#ifndef GE_UTILS_H_
#define GE_UTILS_H_

#include <stddef.h>

#include "grid_engine/coord.h"

typedef struct ge_coord_vec ge_coord_vec_t;

ge_coord_vec_t* ge_coord_vec_create();
ge_coord_vec_t* ge_coord_vec_copy(const ge_coord_vec_t* coord_vec);
void ge_coord_vec_free(ge_coord_vec_t* coord_vec);
size_t ge_coord_vec_size(const ge_coord_vec_t* coord_vec);
size_t ge_coord_vec_capacity(const ge_coord_vec_t* coord_vec);
void ge_coord_vec_set(ge_coord_vec_t* coord_vec, size_t index, ge_coord_t coord);
ge_coord_t ge_coord_vec_get(const ge_coord_vec_t* coord_vec, size_t index);
bool ge_coord_vec_push_back(ge_coord_vec_t* coord_vec, ge_coord_t coord);
ge_coord_t ge_coord_vec_pop_back(ge_coord_vec_t* coord_vec);
bool ge_coord_vec_resize(ge_coord_vec_t* coord_vec, size_t size);
bool ge_coord_vec_reserve(ge_coord_vec_t* coord_vec, size_t capacity);

/**
 * Return the set of coords forming a line between `start_coord` and
 * `end_coord`, using Bresenham's algorithm.
 */
size_t ge_utils_line_coords(ge_coord_t start_coord, ge_coord_t end_coord, ge_coord_t* output_coords,
                            size_t max_num_coords);

#endif  // GE_UTILS_H_
