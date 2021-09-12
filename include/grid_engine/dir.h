// Copyright (c) 2021 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#ifndef GE_DIRS_H_
#define GE_DIRS_H_

#include "grid_engine/coord.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GE_NUM_DIRS 8

typedef enum ge_dir {
  GE_DIR_NORTH = 0,
  GE_DIR_NORTHEAST,
  GE_DIR_EAST,
  GE_DIR_SOUTHEAST,
  GE_DIR_SOUTH,
  GE_DIR_SOUTHWEST,
  GE_DIR_WEST,
  GE_DIR_NORTHWEST,
  GE_DIR_NONE,  // Special value to indicate no direction
  GE_DIR_ALL,   // Special value to indicate all directions
} ge_dir_t;

// Take advantage of the enum layout to make these as simple as possible
#define GE_FOR_ALL_DIRS(DIR) for (ge_dir_t DIR = 0; DIR < GE_NUM_DIRS; ++DIR)
#define GE_FOR_NESW_DIRS(DIR) for (ge_dir_t DIR = 0; DIR < GE_NUM_DIRS; DIR += 2)

ge_dir_t ge_dir_get_opposite(ge_dir_t dir);
ge_coord_t ge_dir_get_offset(ge_dir_t dir);

#ifdef __cplusplus
}
#endif

#endif  // GE_DIRS_H_
