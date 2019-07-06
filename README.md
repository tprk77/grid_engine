# Grid Engine #

* Build simple cellular automatons!
* Not quite a game engine, but sort of similar.
* Simple and easy to use.
* Learn the C programming language!


## Table of Contents ##

1. [Intro](#intro)
2. [EZ API](#ez-api)
3. [Grid API](#grid-api)
4. [Examples](#examples)


## Intro ##

The goal of Grid Engine is to provide a simple and easy to use "engine" to build
grid oriented programs in the C programming language. In case you don't know
what a "grid oriented program" is, one example would cellular automatons such as
[Conway's Game of Life][conways_game_of_life]. But really, it could be any
program that uses a 2D grid as a display.

Grid Engine is meant to be useful on it's own, but it's also meant to double as
a fun way to learn more about the C programming language. I've written the
interface as a mostly idiomatic C API. I've tried to keep things simple, while
not shying away from more advanced topics for beginners like function pointers
and opaque `struct` pointers.


## EZ API ##

**`int ge_ez_loop(const ez_loop_data_t* ez_loop_data);`**

The [EZ Loop API][ez_loop.h] tries to take care of most of the details of
running the engine. In particular, the `ge_ez_loop` runs the main loop of the
game, and is meant as a `main` replacement. You just need to supply a `struct`
with some necessary data. The required data is a grid to operate on, optional
user data (more on that later), and either a loop function, an event handler, or
both. Here's a template for a minimal example:

```
#include "grid_engine/grid_engine.h"

void my_loop_func(ge_grid_t* grid, void* user_data, uint32_t time_ms)
{
  // TODO Periodic update function
}

void my_event_func(ge_grid_t* grid, void* user_data, uint32_t time_ms,
                   const ge_event_t* event)
{
  // TODO Handle events as the occur
}

int main(void)
{
  ge_grid_t* grid = ge_grid_create(100, 100);
  ez_loop_data_t ez_loop_data = {
      .grid = grid,
      .user_data = NULL,
      .loop_func = my_loop_func,
      .event_func = my_event_func,
  };
  return ge_ez_loop(&ez_loop_data);
}
```

The loop function and event function are what's known as callbacks. They're
called callbacks because we don't call them ourselves, we pass them to the
`ge_ez_loop` function to get "called back" later. In our case, the loop function
is called repeatedly for each loop update of the engine, which usually happens
at about 60 Hz. The event function is also called repeatedly for each event that
occurs in the engine. Multiple events can occur per loop update.

**User Data**

In C, it's common to supply callbacks with void pointers to user data. The user
data can be any `struct` pointer the user wants. It's passed alongside the
callback, when the callback is being set up. It's casted to a void pointer
because void pointers can point to any type. Later on, when the callback is
actually called, the user data is passed to the callback as an argument. This
allows the callback to use some non-global persistent storage. It just needs to
get casted back to it's original type. For example:

```
typedef struct user_data {
  int my_counter;
} user_data_t;

int main(void)
{
  ge_grid_t* grid = ge_grid_create(100, 100);
  user_data_t my_user_data = {
      .my_counter = 0;
  };
  ez_loop_data_t ez_loop_data = {
      .grid = grid,
      .user_data = &my_user_data,
      .loop_func = my_loop_func,
      .event_func = my_event_func,
  };
  return ge_ez_loop(&ez_loop_data);
}
```

Then in the callback:

```
void my_loop_func(ge_grid_t* grid, void* user_data, uint32_t time_ms)
{
  user_data_t* my_user_data = (user_data_t*) user_data;
  // Increment the counter!
  my_user_data->counter++;
  // ...
}
```


## Grid API ##

**`typedef struct ge_grid ge_grid_t;`**

The [Grid API][grid.h] centers around `ge_grid_t`, which is a `struct`
containing the width, height, and pixel array of a grid. The programmer cannot
create a `ge_grid_t` directly, they can only access the grid by an opaque
pointer and it's associated functions.

An [opaque pointer][opaque_pointer] is when the programmer is only given a
declaration of a data type, and not the actual definition. This allows the
programmer to pass the pointer around to other functions, but does not allow
direct access through a dereference. This is commonly done in C to protect
internal data. In the case of `ge_grid_t`, we can make sure that the width and
height stay constant for the life of the grid.

The coordinates of a grid follow the common computer graphics convention of
putting `(0, 0)` at the top-left corner. The other corner `(width, height)` is
at the bottom-right corner of the image. Said another way: Increasing X
corresponds to leftward movement, while decreasing X corresponds to rightward
movement. Increasing Y corresponds to downward movement, while decreasing Y
corresponds to upwards movement.

Pixels can have values ranging from 0 to 255. A value of zero corresponds to a
black pixel, while a value of 255 corresponds to a white pixel. Values in
between 0 and 255 are varying shades of gray. Colors not currently
supported, sorry!

**`ge_grid_t* ge_grid_create(size_t width, size_t height);`**<br>
**`void ge_grid_free(const ge_grid_t* grid);`**

These two functions are used to create and free (destroy) grids. When a grid is
created, a pixel array is automatically allocated for the given width and
height. All of the grids associated data is allocated on the heap, so it must be
freed later to avoid memory leaks.

When a grid is no longer needed, it should be freed. Freeing the grid also
automatically frees the pixel array.

**`size_t ge_grid_get_width(const ge_grid_t* grid);`**<br>
**`size_t ge_grid_get_height(const ge_grid_t* grid);`**

These two functions return the width and height of the grid, respectively. Note
that the width and height are not modifiable; you are not allowed to change the
size of a grid after creation.

**`const uint8_t* ge_grid_get_pixel_arr(const ge_grid_t* grid);`**<br>
**`uint8_t* ge_grid_get_pixel_arr_mut(ge_grid_t* grid);`**

These functions return a pointer to the pixel array data. The first function
returns a pointer to `const`, which means the data is read-only. The second
function, ending in `_mut` for mutable, returns a pointer to data that is
writable. In general, you shouldn't need to use these functions. Prefer to use
the getters and setters described below.

**`bool ge_grid_has_coord(const ge_grid_t* grid, ge_coord_t coord);`**

This function is used to test if a coordinate is contained by the grid. It will
return true if the X component is greater or equal to zero and less than width,
and the Y component is greater or equal to zero and less than height.

**`uint8_t ge_grid_get_coord(const ge_grid_t* grid, ge_coord_t coord);`**<br>
**`void ge_grid_set_coord(ge_grid_t* grid, ge_coord_t coord, uint8_t value);`**

These functions get and set the value of a pixel at a coordinate, respectively.
The grid must actually contain the coordinate. You can check for this using
`ge_grid_has_coord` if necessary. If the grid does not contain the coordinate,
the access will be out-of-bounds and the program will abort.

**`uint8_t ge_grid_get_coord_wrapped(const ge_grid_t* grid, ge_coord_t offset);`**<br>
**`void ge_grid_set_coord_wrapped(ge_grid_t* grid, ge_coord_t offset, uint8_t value);`**

These functions get and set the value of a pixel at a wrapped coordinate,
respectively. They are similar to the above functions, but the grid DOES NOT
need to contain the coordinates. Instead, the coordinates will wrap around the
extents of the grid so they always result in a valid access.

Wrapping works in the following way: Basically, when a coordinate goes off the
top edge of the grid, it wraps back to the bottom edge, and vice versa. When a
coordinate goes off the left edge, it wraps back to the right edge, and vice
versa. Coordinates can be arbitrarily large or small, wrapping will always bring
them back within the grid, even if it would require multiple wraps.

**`void ge_grid_copy_pixel_arr(ge_grid_t* grid, const ge_grid_t* other);`**<br>
**`void ge_grid_clear_pixel_arr(ge_grid_t* grid);`**

The fist function can be used to copy the pixel array of one grid to another.
The grids must have exactly the same width and height, or else an out-of-bounds
access will occur and the program will abort.

The second function simply clears the grid by setting all pixels to zero.

**`ge_neighbor_res_t ge_grid_get_neighbors(const ge_grid_t* grid, ge_coord_t coord);`**<br>
**`ge_neighbor_res_t ge_grid_get_neighbors_wrapped(const ge_grid_t* grid, ge_coord_t coord);`**

```
typedef struct ge_neighbor_res {
  size_t num_neighbors;
  ge_coord_t neighbors[GE_MAX_NUM_NEIGHBORS];
} ge_neighbor_res_t;
```

These functions return the coordinates of the neighbors of a pixel. The
neighbors of a pixel are defined to be the eight pixels surrounding it. The
first function is aware of the extents of the grid, and will not return
coordinates outside of the grid. For example, a pixel in the corner of the grid
will only have three neighbors. The second function will wrap around the edges
of the grid and will always return all eight neighbors.


## Examples ##

Here's Conway's Game of Life implemented as a test. The full source is
[available here!][test_conway.c]

First we include the header and define our user data:

```
#include "grid_engine/grid_engine.h"

typedef struct user_data {
  size_t last_update_time_s;
  ge_grid_t* temp_grid;
} user_data_t;
```

The user data includes the time step of the last update, so we only update the
grid once per second. It also includes a secondary grid, which we will use as a
temporary workspace when finding the cells for the next time step.

Then we define the typical Game of Life rules:

```
bool gol_cell_live(bool is_live, size_t num_live_neighbors)
{
  if (is_live) {
    if (num_live_neighbors < 2 || num_live_neighbors > 3) {
      return false;
    }
    else {
      return true;
    }
  }
  else {
    if (num_live_neighbors == 3) {
      return true;
    }
    else {
      return false;
    }
  }
}
```

Next we will define the loop update function. We start by casting the user data
from a void pointer back to the real type:

```
void conway_loop_func(ge_grid_t* restrict grid, void* restrict user_data_, uint32_t time_ms)
{
  // Cast the user data back to the right type
  user_data_t* user_data = (user_data_t*) user_data_;
```

We use the time step to check if we should actually perform the update. We
only want to update once a second, so we find the time step in seconds, and then
compare that to the last value. If they're the same, we return early:

```
  // Check the time so we only update once a second
  const uint64_t time_s = time_ms / 1000;
  if (time_s == user_data->last_update_time_s) {
    return;
  }
  user_data->last_update_time_s = time_s;
```

Here we start the loop to iterate over all pixels in the grid:

```
  // Get width, height, etc
  const size_t width = ge_grid_get_width(grid);
  const size_t height = ge_grid_get_height(grid);
  // Compute the new grid from the old grid
  for (size_t jj = 0; jj < height; ++jj) {
    for (size_t ii = 0; ii < width; ++ii) {
      const ge_coord_t coord = (ge_coord_t){ii, jj};
```

We check if the cell (pixel) is alive by seeing if the value is non-zero. We
then get the neighboring coordinates of the cell, wrapping around the edges of
the grid, and count the number of live neighboring cells:

```
      const bool is_live = (ge_grid_get_coord(grid, coord) != 0);
      const ge_neighbor_res_t neighbor_res = ge_grid_get_neighbors_wrapped(grid, coord);
      size_t num_live_neighbors = 0;
      for (size_t kk = 0; kk < neighbor_res.num_neighbors; ++kk) {
        if (ge_grid_get_coord(grid, neighbor_res.neighbors[kk])) {
          ++num_live_neighbors;
        }
      }
```

With that information, we can apply the Game of Life rules. If the cell is
alive, we give it a value of 255. If it's not alive, we set it to 0. We don't
want to modify the original grid while we are looping over it, so we set the
result in the temporary grid. We do this over and over until the loop ends:

```
      // Apply the Game Of Life rules
      if (gol_cell_live(is_live, num_live_neighbors)) {
        ge_grid_set_coord(user_data->temp_grid, coord, 255);
      }
      else {
        ge_grid_set_coord(user_data->temp_grid, coord, 0);
      }
    }
  }
```

Once the loop ends, we copy the temporary grid to the original grid:

```
  // Swap the results with the current grid
  ge_grid_copy_pixel_arr(grid, user_data->temp_grid);
  ge_grid_clear_pixel_arr(user_data->temp_grid);
}
```

The main loop is very straightforward and just sets up the loop:

```
int main(void)
{
  ge_grid_t* grid = ge_grid_create(100, 100);
  // Make an X pattern
  for (size_t jj = 0; jj < 100; ++jj) {
    for (size_t ii = 0; ii < 100; ++ii) {
      if (ii == jj || (99 - ii) == jj) {
        ge_grid_set_coord(grid, (ge_coord_t){ii, jj}, 255);
      }
    }
  }
  // User data to track state, etc
  user_data_t user_data = {
      .last_update_time_s = 0,
      .temp_grid = ge_grid_create(100, 100),
  };
  // The EZ loop data
  ez_loop_data_t ez_loop_data = {
      .grid = grid,
      .user_data = &user_data,
      .loop_func = conway_loop_func,
      .event_func = NULL,
  };
  // RUN THE LOOP!
  return ge_ez_loop(&ez_loop_data);
}
```

The full source is [available here!][test_conway.c]


<!-- REFERENCE -->

[conways_game_of_life]: https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life
[ez_loop.h]: include/grid_engine/ez_loop.h
[grid.h]: include/grid_engine/grid.h
[opaque_pointer]: https://en.wikipedia.org/wiki/Opaque_pointer
[test_conway.c]: test/test_conway.c


<!-- Local Variables: -->
<!-- fill-column: 80 -->
<!-- End: -->
