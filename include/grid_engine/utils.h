// Copyright (c) 2019 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#ifndef GE_UTILS_H_
#define GE_UTILS_H_

#include <stddef.h>

#include "grid_engine/coord.h"

size_t ge_utils_line_coords(ge_coord_t start_coord, ge_coord_t end_coord,
                            ge_coord_t* restrict output_coords, size_t max_num_coords);

/**
 * Get some glyph coords, if they exist.
 *
 * Glyphs are always 8x8 and monospaced (that happened to be the font I picked). Supported
 * characters are 'A' to 'Z', 'a' to 'z', and '0' to '9'.
 */
bool ge_utils_get_glyph(char glyph, const ge_coord_t** restrict glyph_coords,
                        size_t* restrict glyph_size);

/*
 * The many, many glyphs:
 */

const ge_coord_t GE_GLYPH_A[28];
const ge_coord_t GE_GLYPH_B[33];
const ge_coord_t GE_GLYPH_C[22];
const ge_coord_t GE_GLYPH_D[30];
const ge_coord_t GE_GLYPH_E[30];
const ge_coord_t GE_GLYPH_F[26];
const ge_coord_t GE_GLYPH_G[26];
const ge_coord_t GE_GLYPH_H[30];
const ge_coord_t GE_GLYPH_I[18];
const ge_coord_t GE_GLYPH_J[22];
const ge_coord_t GE_GLYPH_K[30];
const ge_coord_t GE_GLYPH_L[24];
const ge_coord_t GE_GLYPH_M[37];
const ge_coord_t GE_GLYPH_N[34];
const ge_coord_t GE_GLYPH_O[26];
const ge_coord_t GE_GLYPH_P[27];
const ge_coord_t GE_GLYPH_Q[28];
const ge_coord_t GE_GLYPH_R[32];
const ge_coord_t GE_GLYPH_S[25];
const ge_coord_t GE_GLYPH_T[22];
const ge_coord_t GE_GLYPH_U[30];
const ge_coord_t GE_GLYPH_V[26];
const ge_coord_t GE_GLYPH_W[34];
const ge_coord_t GE_GLYPH_X[26];
const ge_coord_t GE_GLYPH_Y[24];
const ge_coord_t GE_GLYPH_Z[30];
const ge_coord_t GE_GLYPH_a[20];
const ge_coord_t GE_GLYPH_b[25];
const ge_coord_t GE_GLYPH_c[18];
const ge_coord_t GE_GLYPH_d[25];
const ge_coord_t GE_GLYPH_e[20];
const ge_coord_t GE_GLYPH_f[21];
const ge_coord_t GE_GLYPH_g[25];
const ge_coord_t GE_GLYPH_h[27];
const ge_coord_t GE_GLYPH_i[15];
const ge_coord_t GE_GLYPH_j[20];
const ge_coord_t GE_GLYPH_k[26];
const ge_coord_t GE_GLYPH_l[17];
const ge_coord_t GE_GLYPH_m[27];
const ge_coord_t GE_GLYPH_n[21];
const ge_coord_t GE_GLYPH_o[20];
const ge_coord_t GE_GLYPH_p[24];
const ge_coord_t GE_GLYPH_q[24];
const ge_coord_t GE_GLYPH_r[20];
const ge_coord_t GE_GLYPH_s[18];
const ge_coord_t GE_GLYPH_t[17];
const ge_coord_t GE_GLYPH_u[21];
const ge_coord_t GE_GLYPH_v[18];
const ge_coord_t GE_GLYPH_w[27];
const ge_coord_t GE_GLYPH_x[19];
const ge_coord_t GE_GLYPH_y[24];
const ge_coord_t GE_GLYPH_z[20];
const ge_coord_t GE_GLYPH_0[36];
const ge_coord_t GE_GLYPH_1[19];
const ge_coord_t GE_GLYPH_2[25];
const ge_coord_t GE_GLYPH_3[23];
const ge_coord_t GE_GLYPH_4[28];
const ge_coord_t GE_GLYPH_5[25];
const ge_coord_t GE_GLYPH_6[24];
const ge_coord_t GE_GLYPH_7[20];
const ge_coord_t GE_GLYPH_8[28];
const ge_coord_t GE_GLYPH_9[24];

#endif  // GE_UTILS_H_
