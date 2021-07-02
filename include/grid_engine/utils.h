// Copyright (c) 2019 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#ifndef GE_UTILS_H_
#define GE_UTILS_H_

#include <stddef.h>

#include "grid_engine/coord.h"

size_t ge_utils_line_coords(ge_coord_t start_coord, ge_coord_t end_coord, ge_coord_t* output_coords,
                            size_t max_num_coords);

/**
 * Get some glyph coords, if they exist.
 *
 * Glyphs are always 8x8 and monospaced (that happened to be the font I picked). Supported
 * characters are 'A' to 'Z', 'a' to 'z', and '0' to '9'.
 */
bool ge_utils_get_glyph(char glyph, const ge_coord_t** glyph_coords, size_t* glyph_size);

/**
 * Get an array of glyphs corresponding to a string.
 */
size_t ge_utils_str_coords(const char* str, ge_coord_t start_coord, ge_coord_t* output_coords,
                           size_t max_num_coords);

#endif  // GE_UTILS_H_
