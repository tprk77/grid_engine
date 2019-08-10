// Copyright (c) 2019 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#include "grid_engine/utils.h"

#include <string.h>

#include "grid_engine/log.h"

static ptrdiff_t pd_abs(ptrdiff_t v);

size_t ge_utils_line_coords(ge_coord_t start_coord, ge_coord_t end_coord,
                            ge_coord_t* restrict output_coords, size_t max_num_coords)
{
  // See also: https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
  const ptrdiff_t diff_x = pd_abs(end_coord.x - start_coord.x);
  const ptrdiff_t diff_y = pd_abs(end_coord.y - start_coord.y);
  const ptrdiff_t step_x = (end_coord.x > start_coord.x ? 1 : -1);
  const ptrdiff_t step_y = (end_coord.y > start_coord.y ? 1 : -1);
  ptrdiff_t x = start_coord.x;
  ptrdiff_t y = start_coord.y;
  size_t num_coords = 0;
  if (max_num_coords > 0) {
    output_coords[num_coords++] = (ge_coord_t){x, y};
  }
  ptrdiff_t error = (diff_x >= diff_y ? diff_x : -diff_y) / 2;
  while (num_coords < max_num_coords && (x != end_coord.x || y != end_coord.y)) {
    ptrdiff_t prev_error = error;
    if (prev_error > -diff_x) {
      error -= diff_y;
      x += step_x;
    }
    if (prev_error < diff_y) {
      error += diff_x;
      y += step_y;
    }
    output_coords[num_coords++] = (ge_coord_t){x, y};
  }
  return num_coords;
}

bool ge_utils_get_glyph(char glyph, const ge_coord_t** restrict glyph_coords,
                        size_t* restrict glyph_size)
{
  *glyph_coords = NULL;
  *glyph_size = 0;
  switch (glyph) {
  case 'A':
    *glyph_coords = GE_GLYPH_A;
    *glyph_size = 28;
    break;
  case 'B':
    *glyph_coords = GE_GLYPH_B;
    *glyph_size = 33;
    break;
  case 'C':
    *glyph_coords = GE_GLYPH_C;
    *glyph_size = 22;
    break;
  case 'D':
    *glyph_coords = GE_GLYPH_D;
    *glyph_size = 30;
    break;
  case 'E':
    *glyph_coords = GE_GLYPH_E;
    *glyph_size = 30;
    break;
  case 'F':
    *glyph_coords = GE_GLYPH_F;
    *glyph_size = 26;
    break;
  case 'G':
    *glyph_coords = GE_GLYPH_G;
    *glyph_size = 26;
    break;
  case 'H':
    *glyph_coords = GE_GLYPH_H;
    *glyph_size = 30;
    break;
  case 'I':
    *glyph_coords = GE_GLYPH_I;
    *glyph_size = 18;
    break;
  case 'J':
    *glyph_coords = GE_GLYPH_J;
    *glyph_size = 22;
    break;
  case 'K':
    *glyph_coords = GE_GLYPH_K;
    *glyph_size = 30;
    break;
  case 'L':
    *glyph_coords = GE_GLYPH_L;
    *glyph_size = 24;
    break;
  case 'M':
    *glyph_coords = GE_GLYPH_M;
    *glyph_size = 37;
    break;
  case 'N':
    *glyph_coords = GE_GLYPH_N;
    *glyph_size = 34;
    break;
  case 'O':
    *glyph_coords = GE_GLYPH_O;
    *glyph_size = 26;
    break;
  case 'P':
    *glyph_coords = GE_GLYPH_P;
    *glyph_size = 27;
    break;
  case 'Q':
    *glyph_coords = GE_GLYPH_Q;
    *glyph_size = 28;
    break;
  case 'R':
    *glyph_coords = GE_GLYPH_R;
    *glyph_size = 32;
    break;
  case 'S':
    *glyph_coords = GE_GLYPH_S;
    *glyph_size = 25;
    break;
  case 'T':
    *glyph_coords = GE_GLYPH_T;
    *glyph_size = 22;
    break;
  case 'U':
    *glyph_coords = GE_GLYPH_U;
    *glyph_size = 30;
    break;
  case 'V':
    *glyph_coords = GE_GLYPH_V;
    *glyph_size = 26;
    break;
  case 'W':
    *glyph_coords = GE_GLYPH_W;
    *glyph_size = 34;
    break;
  case 'X':
    *glyph_coords = GE_GLYPH_X;
    *glyph_size = 26;
    break;
  case 'Y':
    *glyph_coords = GE_GLYPH_Y;
    *glyph_size = 24;
    break;
  case 'Z':
    *glyph_coords = GE_GLYPH_Z;
    *glyph_size = 30;
    break;
  case 'a':
    *glyph_coords = GE_GLYPH_a;
    *glyph_size = 20;
    break;
  case 'b':
    *glyph_coords = GE_GLYPH_b;
    *glyph_size = 25;
    break;
  case 'c':
    *glyph_coords = GE_GLYPH_c;
    *glyph_size = 18;
    break;
  case 'd':
    *glyph_coords = GE_GLYPH_d;
    *glyph_size = 25;
    break;
  case 'e':
    *glyph_coords = GE_GLYPH_e;
    *glyph_size = 20;
    break;
  case 'f':
    *glyph_coords = GE_GLYPH_f;
    *glyph_size = 21;
    break;
  case 'g':
    *glyph_coords = GE_GLYPH_g;
    *glyph_size = 25;
    break;
  case 'h':
    *glyph_coords = GE_GLYPH_h;
    *glyph_size = 27;
    break;
  case 'i':
    *glyph_coords = GE_GLYPH_i;
    *glyph_size = 15;
    break;
  case 'j':
    *glyph_coords = GE_GLYPH_j;
    *glyph_size = 20;
    break;
  case 'k':
    *glyph_coords = GE_GLYPH_k;
    *glyph_size = 26;
    break;
  case 'l':
    *glyph_coords = GE_GLYPH_l;
    *glyph_size = 17;
    break;
  case 'm':
    *glyph_coords = GE_GLYPH_m;
    *glyph_size = 27;
    break;
  case 'n':
    *glyph_coords = GE_GLYPH_n;
    *glyph_size = 21;
    break;
  case 'o':
    *glyph_coords = GE_GLYPH_o;
    *glyph_size = 20;
    break;
  case 'p':
    *glyph_coords = GE_GLYPH_p;
    *glyph_size = 24;
    break;
  case 'q':
    *glyph_coords = GE_GLYPH_q;
    *glyph_size = 24;
    break;
  case 'r':
    *glyph_coords = GE_GLYPH_r;
    *glyph_size = 20;
    break;
  case 's':
    *glyph_coords = GE_GLYPH_s;
    *glyph_size = 18;
    break;
  case 't':
    *glyph_coords = GE_GLYPH_t;
    *glyph_size = 17;
    break;
  case 'u':
    *glyph_coords = GE_GLYPH_u;
    *glyph_size = 21;
    break;
  case 'v':
    *glyph_coords = GE_GLYPH_v;
    *glyph_size = 18;
    break;
  case 'w':
    *glyph_coords = GE_GLYPH_w;
    *glyph_size = 27;
    break;
  case 'x':
    *glyph_coords = GE_GLYPH_x;
    *glyph_size = 19;
    break;
  case 'y':
    *glyph_coords = GE_GLYPH_y;
    *glyph_size = 24;
    break;
  case 'z':
    *glyph_coords = GE_GLYPH_z;
    *glyph_size = 20;
    break;
  case '0':
    *glyph_coords = GE_GLYPH_0;
    *glyph_size = 36;
    break;
  case '1':
    *glyph_coords = GE_GLYPH_1;
    *glyph_size = 19;
    break;
  case '2':
    *glyph_coords = GE_GLYPH_2;
    *glyph_size = 25;
    break;
  case '3':
    *glyph_coords = GE_GLYPH_3;
    *glyph_size = 23;
    break;
  case '4':
    *glyph_coords = GE_GLYPH_4;
    *glyph_size = 28;
    break;
  case '5':
    *glyph_coords = GE_GLYPH_5;
    *glyph_size = 25;
    break;
  case '6':
    *glyph_coords = GE_GLYPH_6;
    *glyph_size = 24;
    break;
  case '7':
    *glyph_coords = GE_GLYPH_7;
    *glyph_size = 20;
    break;
  case '8':
    *glyph_coords = GE_GLYPH_8;
    *glyph_size = 28;
    break;
  case '9':
    *glyph_coords = GE_GLYPH_9;
    *glyph_size = 24;
    break;
  default:
    // Do nothing
    break;
  }
  return (*glyph_coords != NULL);
}

size_t ge_utils_str_coords(const char* restrict str, ge_coord_t start_coord,
                           ge_coord_t* restrict output_coords, size_t max_num_coords)
{
  const char* ch = str;
  ptrdiff_t x = start_coord.x;
  const ptrdiff_t y = start_coord.y;
  size_t num_coords = 0;
  // Increment x by 9 because glyphs are 8 pixels wide
  for (; *ch != '\0' && num_coords < max_num_coords; ch++, x += 9) {
    if (*ch == ' ') {
      continue;
    }
    const ge_coord_t* glyph_coords = NULL;
    size_t glyph_size = 0;
    if (!ge_utils_get_glyph(*ch, &glyph_coords, &glyph_size)) {
      GE_LOG_ERROR("Unknow glyph: %c", *ch);
      break;
    }
    const ge_coord_t glyph_offset = (ge_coord_t){x, y};
    for (size_t ii = 0; ii < glyph_size && num_coords < max_num_coords; ++ii) {
      output_coords[num_coords++] = ge_coord_add(glyph_offset, glyph_coords[ii]);
    }
  }
  return num_coords;
}

static ptrdiff_t pd_abs(ptrdiff_t v)
{
  return (v > 0 ? v : -v);
}

/*
 * The many, many glyphs:
 */

const ge_coord_t GE_GLYPH_A[28] = {
    {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5},
    {1, 6}, {2, 0}, {2, 1}, {2, 4}, {3, 0}, {3, 1}, {3, 4}, {4, 1}, {4, 2}, {4, 3},
    {4, 4}, {4, 5}, {4, 6}, {5, 2}, {5, 3}, {5, 4}, {5, 5}, {5, 6},
};

const ge_coord_t GE_GLYPH_B[33] = {
    {0, 0}, {0, 6}, {1, 0}, {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6}, {2, 0}, {2, 1},
    {2, 2}, {2, 3}, {2, 4}, {2, 5}, {2, 6}, {3, 0}, {3, 3}, {3, 6}, {4, 0}, {4, 3}, {4, 6},
    {5, 0}, {5, 1}, {5, 2}, {5, 3}, {5, 4}, {5, 5}, {5, 6}, {6, 1}, {6, 2}, {6, 4}, {6, 5},
};

const ge_coord_t GE_GLYPH_C[22] = {
    {0, 2}, {0, 3}, {0, 4}, {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {2, 0}, {2, 1}, {2, 5},
    {2, 6}, {3, 0}, {3, 6}, {4, 0}, {4, 6}, {5, 0}, {5, 1}, {5, 5}, {5, 6}, {6, 1}, {6, 5},
};

const ge_coord_t GE_GLYPH_D[30] = {
    {0, 0}, {0, 6}, {1, 0}, {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6}, {2, 0},
    {2, 1}, {2, 2}, {2, 3}, {2, 4}, {2, 5}, {2, 6}, {3, 0}, {3, 6}, {4, 0}, {4, 1},
    {4, 5}, {4, 6}, {5, 1}, {5, 2}, {5, 3}, {5, 4}, {5, 5}, {6, 2}, {6, 3}, {6, 4},
};

const ge_coord_t GE_GLYPH_E[30] = {
    {0, 0}, {0, 6}, {1, 0}, {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6}, {2, 0},
    {2, 1}, {2, 2}, {2, 3}, {2, 4}, {2, 5}, {2, 6}, {3, 0}, {3, 3}, {3, 6}, {4, 0},
    {4, 2}, {4, 3}, {4, 4}, {4, 6}, {5, 0}, {5, 6}, {6, 0}, {6, 1}, {6, 5}, {6, 6},
};

const ge_coord_t GE_GLYPH_F[26] = {
    {0, 0}, {0, 6}, {1, 0}, {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6},
    {2, 0}, {2, 1}, {2, 2}, {2, 3}, {2, 4}, {2, 5}, {2, 6}, {3, 0}, {3, 3},
    {3, 6}, {4, 0}, {4, 2}, {4, 3}, {4, 4}, {5, 0}, {6, 0}, {6, 1},
};

const ge_coord_t GE_GLYPH_G[26] = {
    {0, 2}, {0, 3}, {0, 4}, {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {2, 0},
    {2, 1}, {2, 5}, {2, 6}, {3, 0}, {3, 6}, {4, 0}, {4, 4}, {4, 6}, {5, 0},
    {5, 1}, {5, 4}, {5, 5}, {5, 6}, {6, 1}, {6, 4}, {6, 5}, {6, 6},
};

const ge_coord_t GE_GLYPH_H[30] = {
    {0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {1, 0}, {1, 1}, {1, 2},
    {1, 3}, {1, 4}, {1, 5}, {1, 6}, {2, 3}, {3, 3}, {4, 0}, {4, 1}, {4, 2}, {4, 3},
    {4, 4}, {4, 5}, {4, 6}, {5, 0}, {5, 1}, {5, 2}, {5, 3}, {5, 4}, {5, 5}, {5, 6},
};

const ge_coord_t GE_GLYPH_I[18] = {
    {1, 0}, {1, 6}, {2, 0}, {2, 1}, {2, 2}, {2, 3}, {2, 4}, {2, 5}, {2, 6},
    {3, 0}, {3, 1}, {3, 2}, {3, 3}, {3, 4}, {3, 5}, {3, 6}, {4, 0}, {4, 6},
};

const ge_coord_t GE_GLYPH_J[22] = {
    {0, 4}, {0, 5}, {1, 4}, {1, 5}, {1, 6}, {2, 6}, {3, 0}, {3, 6}, {4, 0}, {4, 1}, {4, 2},
    {4, 3}, {4, 4}, {4, 5}, {4, 6}, {5, 0}, {5, 1}, {5, 2}, {5, 3}, {5, 4}, {5, 5}, {6, 0},
};

const ge_coord_t GE_GLYPH_K[30] = {
    {0, 0}, {0, 6}, {1, 0}, {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6}, {2, 0},
    {2, 1}, {2, 2}, {2, 3}, {2, 4}, {2, 5}, {2, 6}, {3, 3}, {4, 2}, {4, 3}, {4, 4},
    {5, 0}, {5, 1}, {5, 2}, {5, 4}, {5, 5}, {5, 6}, {6, 0}, {6, 1}, {6, 5}, {6, 6},
};

const ge_coord_t GE_GLYPH_L[24] = {
    {0, 0}, {0, 6}, {1, 0}, {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6}, {2, 0}, {2, 1}, {2, 2},
    {2, 3}, {2, 4}, {2, 5}, {2, 6}, {3, 0}, {3, 6}, {4, 6}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6},
};

const ge_coord_t GE_GLYPH_M[37] = {
    {0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {1, 0}, {1, 1}, {1, 2},
    {1, 3}, {1, 4}, {1, 5}, {1, 6}, {2, 1}, {2, 2}, {2, 3}, {3, 2}, {3, 3}, {3, 4},
    {4, 1}, {4, 2}, {4, 3}, {5, 0}, {5, 1}, {5, 2}, {5, 3}, {5, 4}, {5, 5}, {5, 6},
    {6, 0}, {6, 1}, {6, 2}, {6, 3}, {6, 4}, {6, 5}, {6, 6},
};

const ge_coord_t GE_GLYPH_N[34] = {
    {0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {1, 0}, {1, 1}, {1, 2}, {1, 3}, {1, 4},
    {1, 5}, {1, 6}, {2, 1}, {2, 2}, {3, 2}, {3, 3}, {4, 3}, {4, 4}, {5, 0}, {5, 1}, {5, 2}, {5, 3},
    {5, 4}, {5, 5}, {5, 6}, {6, 0}, {6, 1}, {6, 2}, {6, 3}, {6, 4}, {6, 5}, {6, 6},
};

const ge_coord_t GE_GLYPH_O[26] = {
    {0, 2}, {0, 3}, {0, 4}, {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {2, 0},
    {2, 1}, {2, 5}, {2, 6}, {3, 0}, {3, 6}, {4, 0}, {4, 1}, {4, 5}, {4, 6},
    {5, 1}, {5, 2}, {5, 3}, {5, 4}, {5, 5}, {6, 2}, {6, 3}, {6, 4},
};

const ge_coord_t GE_GLYPH_P[27] = {
    {0, 0}, {0, 6}, {1, 0}, {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6},
    {2, 0}, {2, 1}, {2, 2}, {2, 3}, {2, 4}, {2, 5}, {2, 6}, {3, 0}, {3, 3},
    {3, 6}, {4, 0}, {4, 3}, {5, 0}, {5, 1}, {5, 2}, {5, 3}, {6, 1}, {6, 2},
};

const ge_coord_t GE_GLYPH_Q[28] = {
    {0, 1}, {0, 2}, {0, 3}, {0, 4}, {1, 0}, {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5},
    {2, 0}, {2, 5}, {3, 0}, {3, 4}, {3, 5}, {3, 6}, {4, 0}, {4, 1}, {4, 2}, {4, 3},
    {4, 4}, {4, 5}, {4, 6}, {5, 1}, {5, 2}, {5, 3}, {5, 4}, {5, 6},
};

const ge_coord_t GE_GLYPH_R[32] = {
    {0, 0}, {0, 6}, {1, 0}, {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6}, {2, 0}, {2, 1},
    {2, 2}, {2, 3}, {2, 4}, {2, 5}, {2, 6}, {3, 0}, {3, 3}, {4, 0}, {4, 3}, {4, 4}, {5, 0},
    {5, 1}, {5, 2}, {5, 3}, {5, 4}, {5, 5}, {5, 6}, {6, 1}, {6, 2}, {6, 5}, {6, 6},
};

const ge_coord_t GE_GLYPH_S[25] = {
    {0, 1}, {0, 2}, {0, 5}, {1, 0}, {1, 1}, {1, 2}, {1, 3}, {1, 5}, {1, 6},
    {2, 0}, {2, 2}, {2, 3}, {2, 6}, {3, 0}, {3, 3}, {3, 4}, {3, 6}, {4, 0},
    {4, 1}, {4, 4}, {4, 5}, {4, 6}, {5, 1}, {5, 4}, {5, 5},
};

const ge_coord_t GE_GLYPH_T[22] = {
    {0, 0}, {0, 1}, {1, 0}, {1, 6}, {2, 0}, {2, 1}, {2, 2}, {2, 3}, {2, 4}, {2, 5}, {2, 6},
    {3, 0}, {3, 1}, {3, 2}, {3, 3}, {3, 4}, {3, 5}, {3, 6}, {4, 0}, {4, 6}, {5, 0}, {5, 1},
};

const ge_coord_t GE_GLYPH_U[30] = {
    {0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {1, 0}, {1, 1}, {1, 2},
    {1, 3}, {1, 4}, {1, 5}, {1, 6}, {2, 6}, {3, 6}, {4, 0}, {4, 1}, {4, 2}, {4, 3},
    {4, 4}, {4, 5}, {4, 6}, {5, 0}, {5, 1}, {5, 2}, {5, 3}, {5, 4}, {5, 5}, {5, 6},
};

const ge_coord_t GE_GLYPH_V[26] = {
    {0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {1, 0}, {1, 1}, {1, 2}, {1, 3},
    {1, 4}, {1, 5}, {2, 5}, {2, 6}, {3, 5}, {3, 6}, {4, 0}, {4, 1}, {4, 2},
    {4, 3}, {4, 4}, {4, 5}, {5, 0}, {5, 1}, {5, 2}, {5, 3}, {5, 4},
};

const ge_coord_t GE_GLYPH_W[34] = {
    {0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {1, 0}, {1, 1}, {1, 2}, {1, 3}, {1, 4},
    {1, 5}, {1, 6}, {2, 4}, {2, 5}, {3, 3}, {3, 4}, {4, 4}, {4, 5}, {5, 0}, {5, 1}, {5, 2}, {5, 3},
    {5, 4}, {5, 5}, {5, 6}, {6, 0}, {6, 1}, {6, 2}, {6, 3}, {6, 4}, {6, 5}, {6, 6},
};

const ge_coord_t GE_GLYPH_X[26] = {
    {0, 0}, {0, 1}, {0, 6}, {1, 0}, {1, 1}, {1, 2}, {1, 5}, {1, 6}, {2, 2},
    {2, 3}, {2, 4}, {2, 5}, {3, 3}, {3, 4}, {4, 2}, {4, 3}, {4, 4}, {4, 5},
    {5, 0}, {5, 1}, {5, 2}, {5, 5}, {5, 6}, {6, 0}, {6, 1}, {6, 6},
};

const ge_coord_t GE_GLYPH_Y[24] = {
    {0, 0}, {0, 1}, {0, 2}, {1, 0}, {1, 1}, {1, 2}, {1, 3}, {1, 6}, {2, 3}, {2, 4}, {2, 5}, {2, 6},
    {3, 3}, {3, 4}, {3, 5}, {3, 6}, {4, 0}, {4, 1}, {4, 2}, {4, 3}, {4, 6}, {5, 0}, {5, 1}, {5, 2},
};

const ge_coord_t GE_GLYPH_Z[30] = {
    {0, 0}, {0, 1}, {0, 2}, {0, 6}, {1, 0}, {1, 1}, {1, 5}, {1, 6}, {2, 0}, {2, 4},
    {2, 5}, {2, 6}, {3, 0}, {3, 3}, {3, 4}, {3, 6}, {4, 0}, {4, 2}, {4, 3}, {4, 6},
    {5, 0}, {5, 1}, {5, 2}, {5, 5}, {5, 6}, {6, 0}, {6, 1}, {6, 4}, {6, 5}, {6, 6},
};

const ge_coord_t GE_GLYPH_a[20] = {
    {0, 5}, {1, 2}, {1, 4}, {1, 5}, {1, 6}, {2, 2}, {2, 4}, {2, 6}, {3, 2}, {3, 4},
    {3, 6}, {4, 2}, {4, 3}, {4, 4}, {4, 5}, {5, 3}, {5, 4}, {5, 5}, {5, 6}, {6, 6},
};

const ge_coord_t GE_GLYPH_b[25] = {
    {0, 0}, {0, 6}, {1, 0}, {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6},
    {2, 0}, {2, 1}, {2, 2}, {2, 3}, {2, 4}, {2, 5}, {3, 3}, {3, 6}, {4, 3},
    {4, 6}, {5, 3}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5},
};

const ge_coord_t GE_GLYPH_c[18] = {
    {0, 3}, {0, 4}, {0, 5}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6}, {2, 2},
    {2, 6}, {3, 2}, {3, 6}, {4, 2}, {4, 3}, {4, 5}, {4, 6}, {5, 3}, {5, 5},
};

const ge_coord_t GE_GLYPH_d[25] = {
    {0, 4}, {0, 5}, {1, 3}, {1, 4}, {1, 5}, {1, 6}, {2, 3}, {2, 6}, {3, 0},
    {3, 3}, {3, 6}, {4, 0}, {4, 1}, {4, 2}, {4, 3}, {4, 4}, {4, 5}, {5, 0},
    {5, 1}, {5, 2}, {5, 3}, {5, 4}, {5, 5}, {5, 6}, {6, 6},
};

const ge_coord_t GE_GLYPH_e[20] = {
    {0, 3}, {0, 4}, {0, 5}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6}, {2, 2}, {2, 4},
    {2, 6}, {3, 2}, {3, 4}, {3, 6}, {4, 2}, {4, 3}, {4, 4}, {4, 6}, {5, 3}, {5, 4},
};

const ge_coord_t GE_GLYPH_f[21] = {
    {0, 3}, {0, 6}, {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6}, {2, 0}, {2, 1}, {2, 2},
    {2, 3}, {2, 4}, {2, 5}, {2, 6}, {3, 0}, {3, 3}, {3, 6}, {4, 0}, {4, 1}, {5, 1},
};

const ge_coord_t GE_GLYPH_g[25] = {
    {0, 3}, {0, 4}, {0, 7}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 7}, {2, 2},
    {2, 5}, {2, 7}, {3, 2}, {3, 5}, {3, 7}, {4, 3}, {4, 4}, {4, 5}, {4, 6},
    {4, 7}, {5, 2}, {5, 3}, {5, 4}, {5, 5}, {5, 6}, {6, 2},
};

const ge_coord_t GE_GLYPH_h[27] = {
    {0, 0}, {0, 6}, {1, 0}, {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6},
    {2, 0}, {2, 1}, {2, 2}, {2, 3}, {2, 4}, {2, 5}, {2, 6}, {3, 3}, {4, 2},
    {5, 2}, {5, 3}, {5, 4}, {5, 5}, {5, 6}, {6, 3}, {6, 4}, {6, 5}, {6, 6},
};

const ge_coord_t GE_GLYPH_i[15] = {
    {1, 2}, {1, 6}, {2, 0}, {2, 2}, {2, 3}, {2, 4}, {2, 5}, {2, 6},
    {3, 0}, {3, 2}, {3, 3}, {3, 4}, {3, 5}, {3, 6}, {4, 6},
};

const ge_coord_t GE_GLYPH_j[20] = {
    {0, 5}, {0, 6}, {1, 5}, {1, 6}, {1, 7}, {2, 7}, {3, 7}, {4, 0}, {4, 2}, {4, 3},
    {4, 4}, {4, 5}, {4, 6}, {4, 7}, {5, 0}, {5, 2}, {5, 3}, {5, 4}, {5, 5}, {5, 6},
};

const ge_coord_t GE_GLYPH_k[26] = {
    {0, 0}, {0, 6}, {1, 0}, {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6},
    {2, 0}, {2, 1}, {2, 2}, {2, 3}, {2, 4}, {2, 5}, {2, 6}, {3, 4}, {4, 3},
    {4, 4}, {4, 5}, {5, 2}, {5, 3}, {5, 5}, {5, 6}, {6, 2}, {6, 6},
};

const ge_coord_t GE_GLYPH_l[17] = {
    {1, 0}, {1, 6}, {2, 0}, {2, 1}, {2, 2}, {2, 3}, {2, 4}, {2, 5}, {2, 6},
    {3, 0}, {3, 1}, {3, 2}, {3, 3}, {3, 4}, {3, 5}, {3, 6}, {4, 6},
};

const ge_coord_t GE_GLYPH_m[27] = {
    {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {1, 2}, {1, 3}, {1, 4}, {1, 5},
    {1, 6}, {2, 3}, {2, 4}, {3, 3}, {3, 4}, {3, 5}, {4, 2}, {4, 3}, {4, 4},
    {5, 2}, {5, 3}, {5, 4}, {5, 5}, {5, 6}, {6, 3}, {6, 4}, {6, 5}, {6, 6},
};

const ge_coord_t GE_GLYPH_n[21] = {
    {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6}, {2, 2},
    {3, 2}, {4, 2}, {4, 3}, {4, 4}, {4, 5}, {4, 6}, {5, 3}, {5, 4}, {5, 5}, {5, 6},
};

const ge_coord_t GE_GLYPH_o[20] = {
    {0, 3}, {0, 4}, {0, 5}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6}, {2, 2}, {2, 6},
    {3, 2}, {3, 6}, {4, 2}, {4, 3}, {4, 4}, {4, 5}, {4, 6}, {5, 3}, {5, 4}, {5, 5},
};

const ge_coord_t GE_GLYPH_p[24] = {
    {0, 2}, {0, 7}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6}, {1, 7}, {2, 3}, {2, 4}, {2, 5}, {2, 6},
    {2, 7}, {3, 2}, {3, 5}, {3, 7}, {4, 2}, {4, 5}, {5, 2}, {5, 3}, {5, 4}, {5, 5}, {6, 3}, {6, 4},
};

const ge_coord_t GE_GLYPH_q[24] = {
    {0, 3}, {0, 4}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {2, 2}, {2, 5}, {3, 2}, {3, 5}, {3, 7}, {4, 3},
    {4, 4}, {4, 5}, {4, 6}, {4, 7}, {5, 2}, {5, 3}, {5, 4}, {5, 5}, {5, 6}, {5, 7}, {6, 2}, {6, 7},
};

const ge_coord_t GE_GLYPH_r[20] = {
    {0, 2}, {0, 6}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6}, {2, 3}, {2, 4}, {2, 5},
    {2, 6}, {3, 2}, {3, 3}, {3, 6}, {4, 2}, {5, 2}, {5, 3}, {5, 4}, {6, 3}, {6, 4},
};

const ge_coord_t GE_GLYPH_s[18] = {
    {0, 3}, {0, 6}, {1, 2}, {1, 3}, {1, 4}, {1, 6}, {2, 2}, {2, 4}, {2, 6},
    {3, 2}, {3, 4}, {3, 6}, {4, 2}, {4, 4}, {4, 5}, {4, 6}, {5, 2}, {5, 5},
};

const ge_coord_t GE_GLYPH_t[17] = {
    {1, 2}, {2, 1}, {2, 2}, {2, 3}, {2, 4}, {2, 5}, {3, 0}, {3, 1}, {3, 2},
    {3, 3}, {3, 4}, {3, 5}, {3, 6}, {4, 2}, {4, 6}, {5, 2}, {5, 5},
};

const ge_coord_t GE_GLYPH_u[21] = {
    {0, 2}, {0, 3}, {0, 4}, {0, 5}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6}, {2, 6}, {3, 6},
    {4, 2}, {4, 3}, {4, 4}, {4, 5}, {5, 2}, {5, 3}, {5, 4}, {5, 5}, {5, 6}, {6, 6},
};

const ge_coord_t GE_GLYPH_v[18] = {
    {0, 2}, {0, 3}, {0, 4}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {2, 5}, {2, 6},
    {3, 5}, {3, 6}, {4, 2}, {4, 3}, {4, 4}, {4, 5}, {5, 2}, {5, 3}, {5, 4},
};

const ge_coord_t GE_GLYPH_w[27] = {
    {0, 2}, {0, 3}, {0, 4}, {0, 5}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6},
    {2, 4}, {2, 5}, {2, 6}, {3, 3}, {3, 4}, {3, 5}, {4, 4}, {4, 5}, {4, 6},
    {5, 2}, {5, 3}, {5, 4}, {5, 5}, {5, 6}, {6, 2}, {6, 3}, {6, 4}, {6, 5},
};

const ge_coord_t GE_GLYPH_x[19] = {
    {0, 2}, {0, 6}, {1, 2}, {1, 3}, {1, 5}, {1, 6}, {2, 3}, {2, 4}, {2, 5}, {3, 4},
    {4, 3}, {4, 4}, {4, 5}, {5, 2}, {5, 3}, {5, 5}, {5, 6}, {6, 2}, {6, 6},
};

const ge_coord_t GE_GLYPH_y[24] = {
    {0, 2}, {0, 3}, {0, 4}, {0, 7}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 7}, {2, 5}, {2, 7}, {3, 5},
    {3, 7}, {4, 2}, {4, 3}, {4, 4}, {4, 5}, {4, 6}, {4, 7}, {5, 2}, {5, 3}, {5, 4}, {5, 5}, {5, 6},
};

const ge_coord_t GE_GLYPH_z[20] = {
    {0, 2}, {0, 3}, {0, 6}, {1, 2}, {1, 5}, {1, 6}, {2, 2}, {2, 4}, {2, 5}, {2, 6},
    {3, 2}, {3, 3}, {3, 4}, {3, 6}, {4, 2}, {4, 3}, {4, 6}, {5, 2}, {5, 5}, {5, 6},
};

const ge_coord_t GE_GLYPH_0[36] = {
    {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {1, 0}, {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6},
    {2, 0}, {2, 4}, {2, 5}, {2, 6}, {3, 0}, {3, 3}, {3, 4}, {3, 6}, {4, 0}, {4, 2}, {4, 3}, {4, 6},
    {5, 0}, {5, 1}, {5, 2}, {5, 3}, {5, 4}, {5, 5}, {5, 6}, {6, 1}, {6, 2}, {6, 3}, {6, 4}, {6, 5},
};

const ge_coord_t GE_GLYPH_1[19] = {
    {0, 6}, {1, 1}, {1, 6}, {2, 0}, {2, 1}, {2, 2}, {2, 3}, {2, 4}, {2, 5}, {2, 6},
    {3, 0}, {3, 1}, {3, 2}, {3, 3}, {3, 4}, {3, 5}, {3, 6}, {4, 6}, {5, 6},
};

const ge_coord_t GE_GLYPH_2[25] = {
    {0, 1}, {0, 5}, {0, 6}, {1, 0}, {1, 1}, {1, 4}, {1, 5}, {1, 6}, {2, 0},
    {2, 3}, {2, 4}, {2, 6}, {3, 0}, {3, 3}, {3, 6}, {4, 0}, {4, 1}, {4, 2},
    {4, 3}, {4, 5}, {4, 6}, {5, 1}, {5, 2}, {5, 5}, {5, 6},
};

const ge_coord_t GE_GLYPH_3[23] = {
    {0, 1}, {0, 5}, {1, 0}, {1, 1}, {1, 5}, {1, 6}, {2, 0}, {2, 3}, {2, 6}, {3, 0}, {3, 3}, {3, 6},
    {4, 0}, {4, 1}, {4, 2}, {4, 3}, {4, 4}, {4, 5}, {4, 6}, {5, 1}, {5, 2}, {5, 4}, {5, 5},
};

const ge_coord_t GE_GLYPH_4[28] = {
    {0, 3}, {0, 4}, {1, 2}, {1, 3}, {1, 4}, {2, 1}, {2, 2}, {2, 4}, {3, 0}, {3, 1},
    {3, 4}, {3, 6}, {4, 0}, {4, 1}, {4, 2}, {4, 3}, {4, 4}, {4, 5}, {4, 6}, {5, 0},
    {5, 1}, {5, 2}, {5, 3}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 6},
};

const ge_coord_t GE_GLYPH_5[25] = {
    {0, 0}, {0, 1}, {0, 2}, {0, 5}, {1, 0}, {1, 1}, {1, 2}, {1, 5}, {1, 6},
    {2, 0}, {2, 2}, {2, 6}, {3, 0}, {3, 2}, {3, 6}, {4, 0}, {4, 2}, {4, 3},
    {4, 4}, {4, 5}, {4, 6}, {5, 0}, {5, 3}, {5, 4}, {5, 5},
};

const ge_coord_t GE_GLYPH_6[24] = {
    {0, 2}, {0, 3}, {0, 4}, {0, 5}, {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6}, {2, 0}, {2, 1},
    {2, 3}, {2, 6}, {3, 0}, {3, 3}, {3, 6}, {4, 0}, {4, 3}, {4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5},
};

const ge_coord_t GE_GLYPH_7[20] = {
    {0, 0}, {0, 1}, {1, 0}, {1, 1}, {2, 0}, {2, 4}, {2, 5}, {2, 6}, {3, 0}, {3, 3},
    {3, 4}, {3, 5}, {3, 6}, {4, 0}, {4, 1}, {4, 2}, {4, 3}, {5, 0}, {5, 1}, {5, 2},
};

const ge_coord_t GE_GLYPH_8[28] = {
    {0, 1}, {0, 2}, {0, 4}, {0, 5}, {1, 0}, {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5},
    {1, 6}, {2, 0}, {2, 3}, {2, 6}, {3, 0}, {3, 3}, {3, 6}, {4, 0}, {4, 1}, {4, 2},
    {4, 3}, {4, 4}, {4, 5}, {4, 6}, {5, 1}, {5, 2}, {5, 4}, {5, 5},
};

const ge_coord_t GE_GLYPH_9[24] = {
    {0, 1}, {0, 2}, {1, 0}, {1, 1}, {1, 2}, {1, 3}, {1, 6}, {2, 0}, {2, 3}, {2, 6}, {3, 0}, {3, 3},
    {3, 5}, {3, 6}, {4, 0}, {4, 1}, {4, 2}, {4, 3}, {4, 4}, {4, 5}, {5, 1}, {5, 2}, {5, 3}, {5, 4},
};
