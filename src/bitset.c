// Copyright (c) 2021 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#include "grid_engine/bitset.h"

#include <stdlib.h>

#include "grid_engine/log.h"

typedef struct ge_bitset {
  size_t size;
  size_t num_values;
  uint64_t* values;
} ge_bitset_t;

static void abort_on_out_of_bounds(const ge_bitset_t* bitset, size_t index);
static size_t get_first_bit_index(uint64_t value);

ge_bitset_t* ge_bitset_create(size_t size)
{
  ge_bitset_t* bitset = calloc(1, sizeof(ge_bitset_t));
  if (bitset == NULL) {
    return NULL;
  }
  bitset->size = size;
  bitset->num_values = size / 64 + (size % 64 != 0 ? 1 : 0);
  bitset->values = calloc(bitset->num_values, sizeof(uint64_t));
  if (bitset->values == NULL) {
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
  free(bitset->values);
  free(bitset);
}

void ge_bitset_set(ge_bitset_t* bitset, size_t index, bool value)
{
  abort_on_out_of_bounds(bitset, index);
  const size_t value_index = index / 64;
  const size_t bit_index = index % 64;
  if (value) {
    bitset->values[value_index] |= (INT64_C(1) << bit_index);
  }
  else {
    bitset->values[value_index] &= ~(INT64_C(1) << bit_index);
  }
}

bool ge_bitset_get(const ge_bitset_t* bitset, size_t index)
{
  abort_on_out_of_bounds(bitset, index);
  const size_t value_index = index / 64;
  const size_t bit_index = index % 64;
  return ((bitset->values[value_index] & (INT64_C(1) << bit_index)) != 0);
}

size_t ge_bitset_search(const ge_bitset_t* bitset, size_t start_index)
{
  // Complexity will be O(N), with N being the size of the set. Fancy bit
  // checking is really only helping the coefficient. So this data structure is
  // not so great for searching.
  if (start_index != GE_BITSET_SEARCH_INIT) {
    abort_on_out_of_bounds(bitset, start_index);
  }
  start_index = (start_index != GE_BITSET_SEARCH_INIT ? start_index + 1 : 0);
  size_t value_index = start_index / 64;
  const size_t start_bit_index = start_index % 64;
  // Mask the starting int for the start index
  const uint64_t start_value =
      bitset->values[value_index] & (0xFFFFFFFFFFFFFFFF << start_bit_index);
  // Check the starting int separately because of the masking
  if (start_value != 0) {
    return 64 * value_index + get_first_bit_index(start_value);
  }
  // Loop skipping zeroed values, apply bit pattern to non-zeroed values
  for (++value_index; value_index < bitset->num_values; ++value_index) {
    const uint64_t cur_value = bitset->values[value_index];
    if (cur_value != 0) {
      return 64 * value_index + get_first_bit_index(cur_value);
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

static size_t get_first_bit_index(uint64_t value)
{
  size_t first_bit_index = 0;
  first_bit_index += ((value & 0x00000000FFFFFFFF) != 0 ? 0 : 32);
  value &= ((value & 0x00000000FFFFFFFF) != 0 ? 0x00000000FFFFFFFF : 0xFFFFFFFFFFFFFFFF);
  first_bit_index += ((value & 0x0000FFFF0000FFFF) != 0 ? 0 : 16);
  value &= ((value & 0x0000FFFF0000FFFF) != 0 ? 0x0000FFFF0000FFFF : 0xFFFFFFFFFFFFFFFF);
  first_bit_index += ((value & 0x00FF00FF00FF00FF) != 0 ? 0 : 8);
  value &= ((value & 0x00FF00FF00FF00FF) != 0 ? 0x00FF00FF00FF00FF : 0xFFFFFFFFFFFFFFFF);
  first_bit_index += ((value & 0x0F0F0F0F0F0F0F0F) != 0 ? 0 : 4);
  value &= ((value & 0x0F0F0F0F0F0F0F0F) != 0 ? 0x0F0F0F0F0F0F0F0F : 0xFFFFFFFFFFFFFFFF);
  first_bit_index += ((value & 0x3333333333333333) != 0 ? 0 : 2);
  value &= ((value & 0x3333333333333333) != 0 ? 0x3333333333333333 : 0xFFFFFFFFFFFFFFFF);
  first_bit_index += ((value & 0x5555555555555555) != 0 ? 0 : 1);
  return first_bit_index;
}
