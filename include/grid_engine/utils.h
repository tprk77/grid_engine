// Copyright (c) 2019 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#ifndef GE_UTILS_H_
#define GE_UTILS_H_

#include <stddef.h>

#include "grid_engine/coord.h"

size_t ge_utils_line_coords(ge_coord_t start_coord, ge_coord_t end_coord,
                            ge_coord_t* restrict output_coords, size_t max_num_coords);

#endif  // GE_UTILS_H_
