// Copyright (c) 2021 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#ifndef GE_IMG_H_
#define GE_IMG_H_

#include "grid_engine/grid.h"

#ifdef __cplusplus
extern "C" {
#endif

ge_grid_t* ge_img_load(const char* filename);

#ifdef __cplusplus
}
#endif

#endif  // GE_IMG_H_
