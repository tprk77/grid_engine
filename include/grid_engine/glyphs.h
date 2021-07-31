// Copyright (c) 2019 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#ifndef GE_GLYPHS_H_
#define GE_GLYPHS_H_

#include "grid_engine/coord.h"
#include "grid_engine/coord_vec.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const ge_coord_t GE_GLYPH_A[28];
extern const ge_coord_t GE_GLYPH_B[33];
extern const ge_coord_t GE_GLYPH_C[22];
extern const ge_coord_t GE_GLYPH_D[30];
extern const ge_coord_t GE_GLYPH_E[30];
extern const ge_coord_t GE_GLYPH_F[26];
extern const ge_coord_t GE_GLYPH_G[26];
extern const ge_coord_t GE_GLYPH_H[30];
extern const ge_coord_t GE_GLYPH_I[18];
extern const ge_coord_t GE_GLYPH_J[22];
extern const ge_coord_t GE_GLYPH_K[30];
extern const ge_coord_t GE_GLYPH_L[24];
extern const ge_coord_t GE_GLYPH_M[37];
extern const ge_coord_t GE_GLYPH_N[34];
extern const ge_coord_t GE_GLYPH_O[26];
extern const ge_coord_t GE_GLYPH_P[27];
extern const ge_coord_t GE_GLYPH_Q[28];
extern const ge_coord_t GE_GLYPH_R[32];
extern const ge_coord_t GE_GLYPH_S[25];
extern const ge_coord_t GE_GLYPH_T[22];
extern const ge_coord_t GE_GLYPH_U[30];
extern const ge_coord_t GE_GLYPH_V[26];
extern const ge_coord_t GE_GLYPH_W[34];
extern const ge_coord_t GE_GLYPH_X[26];
extern const ge_coord_t GE_GLYPH_Y[24];
extern const ge_coord_t GE_GLYPH_Z[30];
extern const ge_coord_t GE_GLYPH_a[20];
extern const ge_coord_t GE_GLYPH_b[25];
extern const ge_coord_t GE_GLYPH_c[18];
extern const ge_coord_t GE_GLYPH_d[25];
extern const ge_coord_t GE_GLYPH_e[20];
extern const ge_coord_t GE_GLYPH_f[21];
extern const ge_coord_t GE_GLYPH_g[25];
extern const ge_coord_t GE_GLYPH_h[27];
extern const ge_coord_t GE_GLYPH_i[15];
extern const ge_coord_t GE_GLYPH_j[20];
extern const ge_coord_t GE_GLYPH_k[26];
extern const ge_coord_t GE_GLYPH_l[17];
extern const ge_coord_t GE_GLYPH_m[27];
extern const ge_coord_t GE_GLYPH_n[21];
extern const ge_coord_t GE_GLYPH_o[20];
extern const ge_coord_t GE_GLYPH_p[24];
extern const ge_coord_t GE_GLYPH_q[24];
extern const ge_coord_t GE_GLYPH_r[20];
extern const ge_coord_t GE_GLYPH_s[18];
extern const ge_coord_t GE_GLYPH_t[17];
extern const ge_coord_t GE_GLYPH_u[21];
extern const ge_coord_t GE_GLYPH_v[18];
extern const ge_coord_t GE_GLYPH_w[27];
extern const ge_coord_t GE_GLYPH_x[19];
extern const ge_coord_t GE_GLYPH_y[24];
extern const ge_coord_t GE_GLYPH_z[20];
extern const ge_coord_t GE_GLYPH_0[36];
extern const ge_coord_t GE_GLYPH_1[19];
extern const ge_coord_t GE_GLYPH_2[25];
extern const ge_coord_t GE_GLYPH_3[23];
extern const ge_coord_t GE_GLYPH_4[28];
extern const ge_coord_t GE_GLYPH_5[25];
extern const ge_coord_t GE_GLYPH_6[24];
extern const ge_coord_t GE_GLYPH_7[20];
extern const ge_coord_t GE_GLYPH_8[28];
extern const ge_coord_t GE_GLYPH_9[24];

/**
 * Get some glyph coords, if they exist.
 *
 * Glyphs are always 8x8 and monospaced (that happened to be the font I picked). Supported
 * characters are 'A' to 'Z', 'a' to 'z', and '0' to '9'.
 */
bool ge_glyph_get(char glyph, const ge_coord_t** glyph_coords, size_t* glyph_size);

/**
 * Get a newly allocated vector of coords corresponding to a string of glyphs.
 */
ge_coord_vec_t* ge_glyph_get_str_coords(const char* str, ge_coord_t start_coord);

#ifdef __cplusplus
}
#endif

#endif  // GE_GLYPHS_H_
