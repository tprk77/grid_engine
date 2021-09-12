// Copyright (c) 2021 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#include "grid_engine/nbrs.h"

#include <stdlib.h>

#include "grid_engine/log.h"

static bool coord_inside(ge_coord_t coord, size_t width, size_t height);

const ge_nbrs_t GE_NBRS_DEFAULTS = GE_NBRS_DEFAULTS_K;

ge_nbrs_t ge_nbrs_from_coord(ge_coord_t coord)
{
  ge_nbrs_t nbrs = GE_NBRS_DEFAULTS;
  for (size_t ii = 0; ii < GE_NUM_DIRS; ++ii) {
    nbrs.nbrs[ii] = ge_coord_add(coord, ge_dir_get_offset(ii));
  }
  return nbrs;
}

ge_nbrs_t ge_nbrs_from_coord_inside(ge_coord_t coord, size_t width, size_t height)
{
  ge_nbrs_t nbrs = GE_NBRS_DEFAULTS;
  for (size_t ii = 0; ii < GE_NUM_DIRS; ++ii) {
    ge_coord_t nbr_coord = ge_coord_add(coord, ge_dir_get_offset(ii));
    if (!coord_inside(nbr_coord, width, height)) {
      continue;
    }
    nbrs.nbrs[ii] = nbr_coord;
  }
  return nbrs;
}

ge_nbrs_t ge_nbrs_from_coord_wrapped(ge_coord_t coord, size_t width, size_t height)
{
  ge_nbrs_t nbrs = GE_NBRS_DEFAULTS;
  for (size_t ii = 0; ii < GE_NUM_DIRS; ++ii) {
    nbrs.nbrs[ii] = ge_coord_wrap(ge_coord_add(coord, ge_dir_get_offset(ii)), width, height);
  }
  return nbrs;
}

bool ge_nbrs_has_nbr(const ge_nbrs_t* nbrs, ge_dir_t dir)
{
  if (dir == GE_DIR_NONE || dir == GE_DIR_ALL) {
    const ge_coord_t* const begin_coord = nbrs->nbrs;
    const ge_coord_t* const end_coord = nbrs->nbrs + GE_NUM_DIRS;
    for (const ge_coord_t* nbr_coord = begin_coord; nbr_coord < end_coord; ++nbr_coord) {
      const bool has_nbr = !ge_coord_is_invalid(*nbr_coord);
      if ((dir == GE_DIR_NONE && has_nbr) || (dir == GE_DIR_ALL && !has_nbr)) {
        return false;
      }
    }
    return true;
  }
  return !ge_coord_is_invalid(nbrs->nbrs[dir]);
}

ge_coord_t ge_nbrs_get_nbr(const ge_nbrs_t* nbrs, ge_dir_t dir)
{
  if (dir == GE_DIR_NONE || dir == GE_DIR_ALL) {
    GE_LOG_ERROR("Cannot get neighbor with special dirction ALL or NONE!");
    abort();
  }
  if (ge_coord_is_invalid(nbrs->nbrs[dir])) {
    GE_LOG_ERROR("Neighbor does not exist for dir");
    abort();
  }
  return nbrs->nbrs[dir];
}

ge_dir_t ge_nbrs_next_dir(const ge_nbrs_t* nbrs, ge_dir_t prev_dir)
{
  size_t ii = (prev_dir != GE_DIR_NONE ? prev_dir + 1 : 0);
  while (ii < GE_NUM_DIRS && ge_coord_is_invalid(nbrs->nbrs[ii])) {
    ++ii;
  }
  return (ii != GE_NUM_DIRS ? (ge_dir_t) ii : GE_DIR_NONE);
}

static bool coord_inside(ge_coord_t coord, size_t width, size_t height)
{
  return (coord.x >= 0 && coord.x < (ptrdiff_t) width && coord.y >= 0
          && coord.y < (ptrdiff_t) height);
}
