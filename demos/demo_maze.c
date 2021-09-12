// Copyright (c) 2021 Tim Perkins

// Licensed under an MIT style license, see LICENSE.md for details.
// You are free to copy and modify this code. Happy hacking!

#include <stdlib.h>

#include "grid_engine/grid_engine.h"
#include "grid_engine/mz_grid.h"

static const uint8_t MAZE_RENDER_VALUE_LOW = 50;
static const uint8_t MAZE_RENDER_VALUE_HIGH = 150;

void recursive_backtracker(ge_mz_grid_t* grid)
{
  const size_t width = ge_mz_grid_get_width(grid);
  const size_t height = ge_mz_grid_get_height(grid);
  // Pick a random point to start
  const ge_coord_t start_coord = {rand() % width, rand() % height};
  // Set up the stack for backtracking
  const size_t con_stack_capacity = width * height;
  uint8_t* const con_stack = calloc(con_stack_capacity, sizeof(uint8_t));
  size_t con_stack_size = 0;
  // Initialize the stack with the first element
  ge_coord_t coord = start_coord;
  con_stack[con_stack_size++] = GE_MZ_CON_NORTH;
  // Work through the entire stack
  while (con_stack_size != 0) {
    // Check neighbors for unvisited connections
    const ge_nbrs_t nbrs = ge_mz_grid_get_nbrs(grid, coord);
    ge_mz_con_t unvisited_cons[GE_MZ_NUM_CONS];
    size_t num_unvisited = 0;
    GE_MZ_FOR_ALL_CONS (con) {
      const ge_dir_t dir = ge_mz_con_get_dir(con);
      // Invalid coord are treated as visited
      if (!ge_nbrs_has_nbr(&nbrs, dir)) {
        continue;
      }
      // Push the connection if the cell is unvisisted, e.g., has no connections
      const ge_coord_t nbr_coord = ge_nbrs_get_nbr(&nbrs, dir);
      if (ge_mz_value_has_con(ge_mz_grid_get_coord(grid, nbr_coord), GE_MZ_CON_NONE)) {
        unvisited_cons[num_unvisited++] = con;
      }
    }
    // If all cells have been visited, pop the stack to backtrack
    if (num_unvisited == 0) {
      const ge_mz_con_t pop_con = con_stack[--con_stack_size];
      const ge_coord_t pop_offset = ge_dir_get_offset(ge_mz_con_get_dir(pop_con));
      coord = ge_coord_sub(coord, pop_offset);
      continue;
    }
    // Otherwise, pick a random direction, push onto the stack
    const ge_mz_con_t unvisited_con = unvisited_cons[rand() % num_unvisited];
    const uint8_t prv_value = ge_mz_grid_get_coord(grid, coord);
    const uint8_t nxt_value = ge_mz_value_add_con(prv_value, unvisited_con);
    ge_mz_grid_set_coord(grid, coord, nxt_value);
    const ge_coord_t unvisited_offset = ge_dir_get_offset(ge_mz_con_get_dir(unvisited_con));
    coord = ge_coord_add(coord, unvisited_offset);
    con_stack[con_stack_size++] = unvisited_con;
  }
  // Cleanup once done
  free(con_stack);
}

void dijkstra_distances(ge_mz_grid_t* grid, ge_coord_t start_coord, size_t* dist_grid)
{
  // It's assumed that dist_grid is already allocated to the right size
  const size_t width = ge_mz_grid_get_width(grid);
  const size_t height = ge_mz_grid_get_height(grid);
  // Fill distance grid with max values (approximating infinity)
  for (size_t jj = 0; jj < height; ++jj) {
    for (size_t ii = 0; ii < width; ++ii) {
      dist_grid[width * jj + ii] = SIZE_MAX;
    }
  }
  // Set the starting coord as an edge with distance zero
  ge_mz_grid_set_coord_set_path(grid, start_coord, GE_MZ_PATH_EDGE);
  dist_grid[width * start_coord.y + start_coord.x] = 0;
  // We are done when the edge set is empty
  while (true) {
    // Find the lowest distance (and check for an empty edge set)
    size_t min_dist = SIZE_MAX;
    size_t min_dist_index = SIZE_MAX;
    ge_coord_t edge_coord = GE_INVALID_COORD;
    while (!ge_coord_is_invalid(edge_coord = ge_mz_grid_next_edge_coord(grid, edge_coord))) {
      const size_t coord_index = width * edge_coord.y + edge_coord.x;
      const size_t coord_dist = dist_grid[coord_index];
      if (coord_dist < min_dist) {
        min_dist = coord_dist;
        min_dist_index = coord_index;
      }
    }
    // Bail on an empty edge set
    if (min_dist_index == SIZE_MAX) {
      break;
    }
    // Expand the cell with the minimum distance
    const ge_coord_t cur_coord = (ge_coord_t){min_dist_index % width, min_dist_index / width};
    const size_t cur_dist = min_dist;
    ge_mz_grid_set_coord_set_path(grid, cur_coord, GE_MZ_PATH_VISITED);
    // Check for unvisited neighbors
    const ge_nbrs_t nbrs = ge_mz_grid_get_nbrs_connected(grid, cur_coord);
    ge_dir_t nbr_dir = GE_DIR_NONE;
    while ((nbr_dir = ge_nbrs_next_dir(&nbrs, nbr_dir)) != GE_DIR_NONE) {
      const ge_coord_t nbr_coord = ge_nbrs_get_nbr(&nbrs, nbr_dir);
      // Only process unvisited and edge neighbors
      const uint8_t nbr_value = ge_mz_grid_get_coord(grid, nbr_coord);
      if (!ge_mz_value_is_path(nbr_value, GE_MZ_PATH_UNVISITED)
          && !ge_mz_value_is_path(nbr_value, GE_MZ_PATH_EDGE)) {
        continue;
      }
      // Update distance for the neighbor cell
      size_t* const nbr_dist = dist_grid + width * nbr_coord.y + nbr_coord.x;
      if (*nbr_dist > cur_dist + 1) {
        *nbr_dist = cur_dist + 1;
      }
      // Set unvisited nodes to now be edges
      if (ge_mz_value_is_path(nbr_value, GE_MZ_PATH_UNVISITED)) {
        ge_mz_grid_set_coord(grid, nbr_coord, ge_mz_value_set_path(nbr_value, GE_MZ_PATH_EDGE));
      }
    }
  }
}

void render_distances(ge_mz_grid_t* grid, size_t* dist_grid)
{
  // It's assumed that dist_grid is already allocated to the right size
  const size_t width = ge_mz_grid_get_width(grid);
  const size_t height = ge_mz_grid_get_height(grid);
  // Find the maximum distance to scale everything to
  size_t max_dist = 0;
  for (size_t jj = 0; jj < height; ++jj) {
    for (size_t ii = 0; ii < width; ++ii) {
      const size_t dist = dist_grid[width * jj + ii];
      max_dist = (dist > max_dist ? dist : max_dist);
    }
  }
  // Color the distances according to the palette
  ge_grid_t* const render_grid = ge_mz_grid_get_render_grid(grid);
  for (size_t jj = 0; jj < height; ++jj) {
    for (size_t ii = 0; ii < width; ++ii) {
      const ge_coord_t coord = {ii, jj};
      const size_t dist_value =
          (MAZE_RENDER_VALUE_HIGH * dist_grid[width * jj + ii]) / max_dist + MAZE_RENDER_VALUE_LOW;
      const ge_coord_t render_coord = {2 * coord.x + 1, 2 * coord.y + 1};
      ge_grid_set_coord(render_grid, render_coord, dist_value);
      const uint8_t value = ge_mz_grid_get_coord(grid, coord);
      if (ge_mz_value_has_con(value, GE_MZ_CON_EAST)) {
        const ge_coord_t render_coord_east = {2 * coord.x + 2, 2 * coord.y + 1};
        ge_grid_set_coord(render_grid, render_coord_east, dist_value);
      }
      if (ge_mz_value_has_con(value, GE_MZ_CON_SOUTH)) {
        const ge_coord_t render_coord_south = {2 * coord.x + 1, 2 * coord.y + 2};
        ge_grid_set_coord(render_grid, render_coord_south, dist_value);
      }
    }
  }
}

int main(void)
{
  const size_t width = 100;
  const size_t height = 100;
  ge_mz_grid_t* const grid = ge_mz_grid_create(width, height);
  // Do maze generation
  recursive_backtracker(grid);
  // Find distances and render them
  size_t* const dist_grid = calloc(width * height, sizeof(size_t));
  GE_LOG_INFO("Finding distances...");
  dijkstra_distances(grid, (ge_coord_t){0, 0}, dist_grid);
  GE_LOG_INFO("Rendering distances...");
  render_distances(grid, dist_grid);
  // The EZ loop data
  ez_loop_data_t ez_loop_data = {
      .grid = ge_mz_grid_get_render_grid(grid),
      .palette = &GE_PALETTE_INFERNO,
      .user_data = NULL,
      .loop_func = NULL,
      .event_func = NULL,
  };
  // RUN THE LOOP!
  const int result = ge_ez_loop(&ez_loop_data);
  free(dist_grid);
  ge_mz_grid_free(grid);
  return result;
}
