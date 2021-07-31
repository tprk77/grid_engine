// Copyright (c) 2020 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#ifndef GE_PALETTE_H_
#define GE_PALETTE_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Color represented as the addition of red, blue, and green values.
 */
typedef struct ge_color {
  uint8_t red;
  uint8_t blue;
  uint8_t green;
} ge_color_t;

/**
 * Palette mapping pixel values to colors.
 */
typedef struct ge_palette {
  ge_color_t colormap[256];
} ge_palette_t;

/**
 * An "Inferno" palette, based on the popular Matplotlib colormap.
 *
 * See also: https://matplotlib.org/examples/color/colormaps_reference.html
 */
extern const ge_palette_t GE_PALETTE_INFERNO;

#ifdef __cplusplus
}
#endif

#endif  // GE_PALETTE_H_
