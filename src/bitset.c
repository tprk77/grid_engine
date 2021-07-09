// Copyright (c) 2021 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#include "grid_engine/bitset.h"

#include <stdlib.h>

#include "grid_engine/log.h"

typedef struct ge_bitset {
  size_t size;
  size_t num_bytes;
  uint8_t* bytes;
} ge_bitset_t;

static void abort_on_out_of_bounds(const ge_bitset_t* bitset, size_t index);

ge_bitset_t* ge_bitset_create(size_t size)
{
  ge_bitset_t* bitset = calloc(1, sizeof(ge_bitset_t));
  if (bitset == NULL) {
    return NULL;
  }
  bitset->size = size;
  bitset->num_bytes = size / 8 + (size % 8 != 0 ? 1 : 0);
  bitset->bytes = calloc(bitset->num_bytes, sizeof(uint8_t));
  if (bitset->bytes == NULL) {
    free(bitset);
    return NULL;
  }
  return bitset;
}

void ge_bitset_free(ge_bitset_t* bitset)
{
  if (bitset == NULL) {
    return;
  }
  free(bitset->bytes);
  free(bitset);
}

void ge_bitset_set(ge_bitset_t* bitset, size_t index, bool value)
{
  abort_on_out_of_bounds(bitset, index);
  const size_t byte_index = index / 8;
  const size_t bit_index = index % 8;
  if (value) {
    bitset->bytes[byte_index] |= ((uint8_t)(1 << bit_index));
  }
  else {
    bitset->bytes[byte_index] &= ~((uint8_t)(1 << bit_index));
  }
}

bool ge_bitset_get(const ge_bitset_t* bitset, size_t index)
{
  abort_on_out_of_bounds(bitset, index);
  const size_t byte_index = index / 8;
  const size_t bit_index = index % 8;
  return ((bitset->bytes[byte_index] & ((uint8_t)(1 << bit_index))) != 0);
}

size_t ge_bitset_search(const ge_bitset_t* bitset, size_t start_index)
{
  static const uint8_t ZERO_BIT_MASKS[8] = {
      0xFF, 0xFE, 0xFC, 0xF8, 0xF0, 0xE0, 0xC0, 0x80,
  };
  static const uint8_t ONE_BIT_MASKS[8] = {
      0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80,
  };
  if (start_index != GE_BITSET_SEARCH_INIT) {
    abort_on_out_of_bounds(bitset, start_index);
  }
  start_index = (start_index != GE_BITSET_SEARCH_INIT ? start_index + 1 : 0);
  size_t byte_index = start_index / 8;
  size_t bit_index = start_index % 8;
  for (; byte_index < bitset->num_bytes; ++byte_index, bit_index = 0) {
    // Check if this is a zeroed byte and skip it
    if ((bitset->bytes[byte_index] & ZERO_BIT_MASKS[bit_index]) == 0) {
      continue;
    }
    // Or else check the remaining bits in the byte
    for (; bit_index < 8; ++bit_index) {
      if ((bitset->bytes[byte_index] & ONE_BIT_MASKS[bit_index]) != 0) {
        return 8 * byte_index + bit_index;
      }
    }
  }
  return GE_BITSET_SEARCH_INIT;
}

static void abort_on_out_of_bounds(const ge_bitset_t* bitset, size_t index)
{
  if (index >= bitset->size) {
    GE_LOG_ERROR("Index is out of bounds! (%zu / %zu)", index, bitset->size);
    abort();
  }
}
