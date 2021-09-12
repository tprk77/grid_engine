// Copyright (c) 2021 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#include "grid_engine/dir.h"

#include <stdlib.h>

#include "grid_engine/log.h"

static const ge_dir_t GE_DIR_OPPOSITES[GE_NUM_DIRS] = {
    GE_DIR_SOUTH, GE_DIR_SOUTHWEST, GE_DIR_WEST, GE_DIR_NORTHWEST,
    GE_DIR_NORTH, GE_DIR_NORTHEAST, GE_DIR_EAST, GE_DIR_SOUTHEAST,
};

static const ge_coord_t GE_DIR_OFFSETS[GE_NUM_DIRS] = {
    {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1},
};

ge_dir_t ge_dir_get_opposite(ge_dir_t dir)
{
  if (dir == GE_DIR_NONE || dir == GE_DIR_ALL) {
    GE_LOG_ERROR("Cannot get opposite for special direction ALL or NONE!");
    abort();
  }
  return GE_DIR_OPPOSITES[dir];
}

ge_coord_t ge_dir_get_offset(ge_dir_t dir)
{
  if (dir == GE_DIR_NONE || dir == GE_DIR_ALL) {
    GE_LOG_ERROR("Cannot get offset for special direction ALL or NONE!");
    abort();
  }
  return GE_DIR_OFFSETS[dir];
}
