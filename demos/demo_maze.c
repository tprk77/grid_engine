// Copyright (c) 2021 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#include "grid_engine/grid_engine.h"

#include <stdlib.h>

typedef struct maze_grid {
  ge_grid_t* logic_grid;
  ge_grid_t* render_grid;
} maze_grid_t;

#define NUM_MAZE_CONVALS 4

typedef enum maze_conval {
  MAZE_CONVAL_NORTH = 0,
  MAZE_CONVAL_EAST,
  MAZE_CONVAL_SOUTH,
  MAZE_CONVAL_WEST,
  MAZE_CONVAL_NONE,  // Special value to indicate no connections
  MAZE_CONVAL_ALL,   // Special value to indicate all connections
} maze_conval_t;

static const maze_conval_t MAZE_CONVALS[NUM_MAZE_CONVALS] = {
    MAZE_CONVAL_NORTH,
    MAZE_CONVAL_EAST,
    MAZE_CONVAL_SOUTH,
    MAZE_CONVAL_WEST,
};

#define MAZE_CONVAL_BIT_MASK ((uint8_t) 0x0F)
#define MAZE_CONVAL_BIT_OFFSET ((uint8_t) 0)
#define MAZE_CONVAL_NORTH_BIT ((uint8_t)((1 << MAZE_CONVAL_NORTH) << MAZE_CONVAL_BIT_OFFSET))
#define MAZE_CONVAL_EAST_BIT ((uint8_t)((1 << MAZE_CONVAL_EAST) << MAZE_CONVAL_BIT_OFFSET))
#define MAZE_CONVAL_SOUTH_BIT ((uint8_t)((1 << MAZE_CONVAL_SOUTH) << MAZE_CONVAL_BIT_OFFSET))
#define MAZE_CONVAL_WEST_BIT ((uint8_t)((1 << MAZE_CONVAL_WEST) << MAZE_CONVAL_BIT_OFFSET))

static const uint8_t MAZE_CONVAL_TO_BITS[NUM_MAZE_CONVALS] = {
    MAZE_CONVAL_NORTH_BIT,
    MAZE_CONVAL_EAST_BIT,
    MAZE_CONVAL_SOUTH_BIT,
    MAZE_CONVAL_WEST_BIT,
};

static const maze_conval_t MAZE_CONVAL_TO_OPPOSITE[NUM_MAZE_CONVALS] = {
    MAZE_CONVAL_SOUTH,
    MAZE_CONVAL_WEST,
    MAZE_CONVAL_NORTH,
    MAZE_CONVAL_EAST,
};

static const ge_direction_t MAZE_CONVAL_TO_DIRECTION[NUM_MAZE_CONVALS] = {
    GE_DIRECTION_NORTH,
    GE_DIRECTION_EAST,
    GE_DIRECTION_SOUTH,
    GE_DIRECTION_WEST,
};

#define NUM_MAZE_PATHVALS 4

typedef enum maze_pathval {
  MAZE_PATHVAL_UNVISITED = 0,
  MAZE_PATHVAL_PATHED,
  MAZE_PATHVAL_EDGE,
  MAZE_PATHVAL_VISITED,
} maze_pathval_t;

#define MAZE_PATHVAL_BIT_MASK ((uint8_t) 0x30)
#define MAZE_PATHVAL_BIT_OFFSET ((uint8_t) 4)
#define MAZE_PATHVAL_UNVISITED_BITS ((uint8_t) MAZE_PATHVAL_UNVISITED << MAZE_PATHVAL_BIT_OFFSET)
#define MAZE_PATHVAL_PATHED_BITS ((uint8_t)(MAZE_PATHVAL_PATHED << MAZE_PATHVAL_BIT_OFFSET))
#define MAZE_PATHVAL_EDGE_BITS ((uint8_t)(MAZE_PATHVAL_EDGE << MAZE_PATHVAL_BIT_OFFSET))
#define MAZE_PATHVAL_VISITED_BITS ((uint8_t)(MAZE_PATHVAL_VISITED << MAZE_PATHVAL_BIT_OFFSET))

static const uint8_t MAZE_PATHVAL_TO_BITS[NUM_MAZE_PATHVALS] = {
    MAZE_PATHVAL_UNVISITED_BITS,
    MAZE_PATHVAL_PATHED_BITS,
    MAZE_PATHVAL_EDGE_BITS,
    MAZE_PATHVAL_VISITED_BITS,
};

uint8_t maze_value_set_connection(uint8_t value, maze_conval_t conval)
{
  if (conval == MAZE_CONVAL_NONE) {
    return value & ~MAZE_CONVAL_BIT_MASK;
  }
  if (conval == MAZE_CONVAL_ALL) {
    return value | MAZE_CONVAL_BIT_MASK;
  }
  return (value & ~MAZE_CONVAL_BIT_MASK) | MAZE_CONVAL_TO_BITS[conval];
}

uint8_t maze_value_add_connection(uint8_t value, maze_conval_t conval)
{
  if (conval == MAZE_CONVAL_NONE) {
    return value;
  }
  if (conval == MAZE_CONVAL_ALL) {
    return value | MAZE_CONVAL_BIT_MASK;
  }
  return value | MAZE_CONVAL_TO_BITS[conval];
}

uint8_t maze_value_remove_connection(uint8_t value, maze_conval_t conval)
{
  if (conval == MAZE_CONVAL_NONE) {
    return value;
  }
  if (conval == MAZE_CONVAL_ALL) {
    return value & ~MAZE_CONVAL_BIT_MASK;
  }
  return value & ~MAZE_CONVAL_TO_BITS[conval];
}

bool maze_value_has_connection(uint8_t value, maze_conval_t conval)
{
  if (conval == MAZE_CONVAL_NONE) {
    return (value & MAZE_CONVAL_BIT_MASK) == 0;
  }
  if (conval == MAZE_CONVAL_ALL) {
    return (value & MAZE_CONVAL_BIT_MASK) == MAZE_CONVAL_BIT_MASK;
  }
  return value & MAZE_CONVAL_TO_BITS[conval];
}

uint8_t maze_value_set_path(uint8_t value, maze_pathval_t pathval)
{
  return (value & ~MAZE_PATHVAL_BIT_MASK) | MAZE_PATHVAL_TO_BITS[pathval];
}

bool maze_value_is_path(uint8_t value, maze_pathval_t pathval)
{
  return (value & MAZE_PATHVAL_BIT_MASK) == MAZE_PATHVAL_TO_BITS[pathval];
}

maze_grid_t* maze_grid_create(size_t width, size_t height)
{
  maze_grid_t* mgrid = calloc(1, sizeof(maze_grid_t));
  if (mgrid == NULL) {
    return NULL;
  }
  mgrid->logic_grid = ge_grid_create(width, height);
  if (mgrid->logic_grid == NULL) {
    free(mgrid);
    return NULL;
  }
  const size_t render_width = width * 2 + 1;
  const size_t render_height = height * 2 + 1;
  mgrid->render_grid = ge_grid_create(render_width, render_height);
  if (mgrid->render_grid == NULL) {
    ge_grid_free(mgrid->logic_grid);
    free(mgrid);
    return NULL;
  }
  // Render the cells of the maze (assuming all unconnected)
  for (size_t jj = 1; jj < render_height; jj += 2) {
    for (size_t ii = 1; ii < render_width; ii += 2) {
      ge_grid_set_coord(mgrid->render_grid, (ge_coord_t){ii, jj}, 255);
    }
  }
  return mgrid;
}

void maze_grid_free(maze_grid_t* mgrid)
{
  if (mgrid == NULL) {
    return;
  }
  ge_grid_free(mgrid->logic_grid);
  ge_grid_free(mgrid->render_grid);
  free(mgrid);
}

size_t maze_grid_get_width(const maze_grid_t* mgrid)
{
  return ge_grid_get_width(mgrid->logic_grid);
}

size_t maze_grid_get_height(const maze_grid_t* mgrid)
{
  return ge_grid_get_height(mgrid->logic_grid);
}

bool maze_grid_has_coord(const maze_grid_t* mgrid, ge_coord_t coord)
{
  return ge_grid_has_coord(mgrid->logic_grid, coord);
}

uint8_t maze_grid_get_coord(const maze_grid_t* mgrid, ge_coord_t coord)
{
  return ge_grid_get_coord(mgrid->logic_grid, coord);
}

void maze_grid_set_coord(maze_grid_t* mgrid, ge_coord_t coord, uint8_t value)
{
  // Update the logical grid
  const uint8_t prv_value = ge_grid_get_coord(mgrid->logic_grid, coord);
  ge_grid_set_coord(mgrid->logic_grid, coord, value);
  for (size_t ii = 0; ii < NUM_MAZE_CONVALS; ++ii) {
    const maze_conval_t conval = MAZE_CONVALS[ii];
    const bool prv_conval_exists = maze_value_has_connection(prv_value, conval);
    const bool cur_conval_exists = maze_value_has_connection(value, conval);
    const bool added_conval = !prv_conval_exists && cur_conval_exists;
    const bool removed_conval = prv_conval_exists && !cur_conval_exists;
    if (!added_conval && !removed_conval) {
      continue;
    }
    const ge_coord_t nbr_offset = ge_direction_get_offset(MAZE_CONVAL_TO_DIRECTION[conval]);
    const ge_coord_t nbr_coord = ge_coord_add(coord, nbr_offset);
    if (!ge_grid_has_coord(mgrid->logic_grid, nbr_coord)) {
      continue;
    }
    const uint8_t nbr_value = ge_grid_get_coord(mgrid->logic_grid, nbr_coord);
    const maze_conval_t ops_conval = MAZE_CONVAL_TO_OPPOSITE[conval];
    const uint8_t nxt_value = (added_conval ? maze_value_add_connection(nbr_value, ops_conval)
                                            : maze_value_remove_connection(nbr_value, ops_conval));
    ge_grid_set_coord(mgrid->logic_grid, nbr_coord, nxt_value);
  }
  // Update the rendered grid
  const ge_coord_t render_coord = {2 * coord.x + 1, 2 * coord.y + 1};
  for (size_t ii = 0; ii < NUM_MAZE_CONVALS; ++ii) {
    const maze_conval_t conval = MAZE_CONVALS[ii];
    const ge_coord_t nbr_offset = ge_direction_get_offset(MAZE_CONVAL_TO_DIRECTION[conval]);
    const ge_coord_t nbr_coord = ge_coord_add(render_coord, nbr_offset);
    const uint8_t nbr_value = (maze_value_has_connection(value, conval) ? 255 : 0);
    ge_grid_set_coord(mgrid->render_grid, nbr_coord, nbr_value);
  }
}

uint8_t maze_grid_get_coord_wrapped(const maze_grid_t* mgrid, ge_coord_t coord)
{
  return ge_grid_get_coord_wrapped(mgrid->logic_grid, coord);
}

void maze_grid_set_coord_wrapped(maze_grid_t* mgrid, ge_coord_t coord, uint8_t value)
{
  // Reuse the code above for setting a coord
  coord = ge_coord_wrap(coord, maze_grid_get_width(mgrid), maze_grid_get_height(mgrid));
  maze_grid_set_coord(mgrid, coord, value);
}

void maze_grid_recursive_backtracker(maze_grid_t* mgrid)
{
  const size_t width = maze_grid_get_width(mgrid);
  const size_t height = maze_grid_get_height(mgrid);
  // Pick a random point to start
  const ge_coord_t start_coord = {rand() % width, rand() % height};
  // Set up the stack for backtracking
  const size_t conval_stack_capacity = width * height;
  uint8_t* const conval_stack = calloc(conval_stack_capacity, sizeof(uint8_t));
  size_t conval_stack_size = 0;
  // Initialize the stack with the first element
  ge_coord_t coord = start_coord;
  conval_stack[conval_stack_size++] = MAZE_CONVAL_NORTH;
  // Work through the entire stack
  while (conval_stack_size != 0) {
    // Check neighbors for unvisited connections
    const ge_neighbors_t nbrs = ge_grid_get_neighbors(mgrid->logic_grid, coord);
    maze_conval_t unvisited_convals[NUM_MAZE_CONVALS];
    size_t num_unvisited = 0;
    for (size_t ii = 0; ii < NUM_MAZE_CONVALS; ++ii) {
      const maze_conval_t conval = MAZE_CONVALS[ii];
      const ge_direction_t direction = MAZE_CONVAL_TO_DIRECTION[conval];
      // Invalid coord are treated as visited
      if (!ge_neighbors_has_neighbor(&nbrs, direction)) {
        continue;
      }
      // Push the connection if the cell is unvisisted, e.g., has no connections
      const ge_coord_t nbr_coord = ge_neighbors_get_neighbor(&nbrs, direction);
      if (maze_value_has_connection(maze_grid_get_coord(mgrid, nbr_coord), MAZE_CONVAL_NONE)) {
        unvisited_convals[num_unvisited++] = conval;
      }
    }
    // If all cells have been visited, pop the stack to backtrack
    if (num_unvisited == 0) {
      const maze_conval_t pop_conval = conval_stack[--conval_stack_size];
      const ge_coord_t pop_offset = ge_direction_get_offset(MAZE_CONVAL_TO_DIRECTION[pop_conval]);
      coord = ge_coord_sub(coord, pop_offset);
      continue;
    }
    // Otherwise, pick a random direction, push onto the stack
    const maze_conval_t unvisited_conval = unvisited_convals[rand() % num_unvisited];
    const uint8_t prv_value = maze_grid_get_coord(mgrid, coord);
    const uint8_t nxt_value = maze_value_add_connection(prv_value, unvisited_conval);
    maze_grid_set_coord(mgrid, coord, nxt_value);
    const ge_coord_t unvisited_offset =
        ge_direction_get_offset(MAZE_CONVAL_TO_DIRECTION[unvisited_conval]);
    coord = ge_coord_add(coord, unvisited_offset);
    conval_stack[conval_stack_size++] = unvisited_conval;
  }
  // Cleanup once done
  free(conval_stack);
}

int main(void)
{
  const size_t width = 100;
  const size_t height = 100;
  maze_grid_t* const mgrid = maze_grid_create(width, height);
  // Do maze generation
  maze_grid_recursive_backtracker(mgrid);
  // The EZ loop data
  ez_loop_data_t ez_loop_data = {
      .grid = mgrid->render_grid,
      .palette = NULL,
      .user_data = NULL,
      .loop_func = NULL,
      .event_func = NULL,
  };
  // RUN THE LOOP!
  const int result = ge_ez_loop(&ez_loop_data);
  maze_grid_free(mgrid);
  return result;
}
