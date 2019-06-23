# Copyright (c) 2019 Tim Perkins

DEBUG := -O0 -g
CFLAGS := -std=c11 -Wall -Wextra -Werror -fPIC $(DEBUG)
INCLUDE := -Iinclude
# On windows we should use -lmingw32
LDFLAGS := -static-libgcc
SDL_LIBS := -lSDL2main -lSDL2
LIBS :=  -L$(CURDIR)/build  -Wl,-rpath=$(CURDIR)/build -lgrid_engine $(SDL_LIBS)

all: lib tests

lib:
	mkdir -p build/tmp
	gcc $(CFLAGS) $(INCLUDE) -c -o build/tmp/engine.o src/engine.c
	gcc $(CFLAGS) $(INCLUDE) -c -o build/tmp/grid.o src/grid.c
	gcc $(CFLAGS) $(INCLUDE) -c -o build/tmp/grid_engine_ez.o src/grid_engine_ez.c
	gcc -shared -o build/libgrid_engine.so build/tmp/engine.o build/tmp/grid.o build/tmp/grid_engine_ez.o
tests: lib
	mkdir -p build/tmp
	gcc $(CFLAGS) $(INCLUDE) -c -o build/tmp/test_main.o test/test_main.c
	gcc $(CFLAGS) $(INCLUDE) -c -o build/tmp/test_ez_main.o test/test_ez_main.c
	gcc $(LDFLAGS) -o build/test build/tmp/test_main.o $(LIBS)
	gcc $(LDFLAGS) -o build/test_ez build/tmp/test_ez_main.o $(LIBS)

clean:
	-rm -rf build
