// Copyright (c) 2021 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#ifndef GE_BITSET_H_
#define GE_BITSET_H_

#include <stdbool.h>
#include <stddef.h>

typedef struct ge_bitset ge_bitset_t;

const size_t GE_BITSET_SEARCH_INIT;

ge_bitset_t* ge_bitset_create(size_t size);
void ge_bitset_free(ge_bitset_t* bitset);
void ge_bitset_set(ge_bitset_t* bitset, size_t index, bool value);
bool ge_bitset_get(const ge_bitset_t* bitset, size_t index);
size_t ge_bitset_search(const ge_bitset_t* bitset, size_t start_index);

#endif  // GE_BITSET_H_
