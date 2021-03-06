// Copyright (c) 2021 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#include "grid_engine/coord_vec.h"

#include <stdlib.h>
#include <string.h>

#include "grid_engine/log.h"

typedef struct ge_coord_vec {
  size_t capacity;
  size_t size;
  ge_coord_t* coord_buffer;
} ge_coord_vec_t;

static const size_t GE_COORD_VEC_DEFAULT_CAPACITY = 64;
static const size_t GE_COORD_VEC_CAPACITY_2X_THRESH = 1024;
static const size_t GE_COORD_VEC_CAPACITY_INCREMENT = 1024;

static bool ge_coord_vec_grow_capacity(ge_coord_vec_t* coord_vec, size_t min_size);

static void abort_on_out_of_bounds(const ge_coord_vec_t* coord_vec, size_t index);
static size_t s_max(size_t a, size_t b);

ge_coord_vec_t* ge_coord_vec_create()
{
  ge_coord_vec_t* coord_vec = calloc(1, sizeof(ge_coord_vec_t));
  if (coord_vec == NULL) {
    return NULL;
  }
  coord_vec->capacity = GE_COORD_VEC_DEFAULT_CAPACITY;
  coord_vec->size = 0;
  coord_vec->coord_buffer = calloc(coord_vec->capacity, sizeof(ge_coord_t));
  if (coord_vec->coord_buffer == NULL) {
    free(coord_vec);
    return NULL;
  }
  return coord_vec;
}

ge_coord_vec_t* ge_coord_vec_copy(const ge_coord_vec_t* coord_vec)
{
  ge_coord_vec_t* copy_coord_vec = calloc(1, sizeof(ge_coord_vec_t));
  copy_coord_vec->capacity = coord_vec->size;
  copy_coord_vec->size = coord_vec->size;
  copy_coord_vec->coord_buffer = calloc(copy_coord_vec->capacity, sizeof(ge_coord_t));
  memcpy(copy_coord_vec->coord_buffer, coord_vec->coord_buffer,
         coord_vec->size * sizeof(ge_coord_t));
  return copy_coord_vec;
}

void ge_coord_vec_free(ge_coord_vec_t* coord_vec)
{
  if (coord_vec == NULL) {
    return;
  }
  free(coord_vec->coord_buffer);
  free(coord_vec);
}

size_t ge_coord_vec_size(const ge_coord_vec_t* coord_vec)
{
  return coord_vec->size;
}

size_t ge_coord_vec_capacity(const ge_coord_vec_t* coord_vec)
{
  return coord_vec->capacity;
}

void ge_coord_vec_set(ge_coord_vec_t* coord_vec, size_t index, ge_coord_t coord)
{
  abort_on_out_of_bounds(coord_vec, index);
  coord_vec->coord_buffer[index] = coord;
}

ge_coord_t ge_coord_vec_get(const ge_coord_vec_t* coord_vec, size_t index)
{
  abort_on_out_of_bounds(coord_vec, index);
  return coord_vec->coord_buffer[index];
}

bool ge_coord_vec_push_back(ge_coord_vec_t* coord_vec, ge_coord_t coord)
{
  if (coord_vec->size == coord_vec->capacity) {
    if (!ge_coord_vec_grow_capacity(coord_vec, 0)) {
      return false;
    }
  }
  coord_vec->coord_buffer[coord_vec->size] = coord;
  ++coord_vec->size;
  return true;
}

ge_coord_t ge_coord_vec_pop_back(ge_coord_vec_t* coord_vec)
{
  --coord_vec->size;
  return coord_vec->coord_buffer[coord_vec->size];
}

bool ge_coord_vec_resize(ge_coord_vec_t* coord_vec, size_t size)
{
  if (size > coord_vec->size) {
    if (!ge_coord_vec_grow_capacity(coord_vec, size)) {
      return false;
    }
    // Fill with invalid coords
    for (size_t ii = coord_vec->size; ii < size; ++ii) {
      coord_vec->coord_buffer[ii] = GE_INVALID_COORD;
    }
  }
  coord_vec->size = size;
  return true;
}

bool ge_coord_vec_reserve(ge_coord_vec_t* coord_vec, size_t capacity)
{
  if (capacity <= coord_vec->capacity) {
    return true;
  }
  ge_coord_t* const coord_buffer = realloc(coord_vec->coord_buffer, capacity * sizeof(ge_coord_t));
  if (coord_buffer == NULL) {
    return false;
  }
  coord_vec->capacity = capacity;
  coord_vec->coord_buffer = coord_buffer;
  return true;
}

ge_coord_t* ge_coord_vec_begin(ge_coord_vec_t* coord_vec)
{
  return coord_vec->coord_buffer;
}

ge_coord_t* ge_coord_vec_end(ge_coord_vec_t* coord_vec)
{
  return coord_vec->coord_buffer + coord_vec->size;
}

const ge_coord_t* ge_coord_vec_cbegin(const ge_coord_vec_t* coord_vec)
{
  return coord_vec->coord_buffer;
}

const ge_coord_t* ge_coord_vec_cend(const ge_coord_vec_t* coord_vec)
{
  return coord_vec->coord_buffer + coord_vec->size;
}

static bool ge_coord_vec_grow_capacity(ge_coord_vec_t* coord_vec, size_t min_size)
{
  const size_t capacity =  //
      s_max(coord_vec->capacity < GE_COORD_VEC_CAPACITY_2X_THRESH
                ? 2 * coord_vec->capacity
                : coord_vec->capacity + GE_COORD_VEC_CAPACITY_INCREMENT,
            min_size);
  ge_coord_t* const coord_buffer = realloc(coord_vec->coord_buffer, capacity * sizeof(ge_coord_t));
  if (coord_buffer == NULL) {
    return false;
  }
  coord_vec->capacity = capacity;
  coord_vec->coord_buffer = coord_buffer;
  return true;
}

static void abort_on_out_of_bounds(const ge_coord_vec_t* coord_vec, size_t index)
{
  if (index >= coord_vec->size) {
    GE_LOG_ERROR("Index is out of bounds! (%zu / %zu)", index, coord_vec->size);
    abort();
  }
}

static size_t s_max(size_t a, size_t b)
{
  return (a > b ? a : b);
}
