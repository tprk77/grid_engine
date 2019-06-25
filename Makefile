# Copyright (c) 2019 Tim Perkins

DEBUG := -O0 -g
CFLAGS := -std=c11 -Wall -Wextra -Werror -fPIC $(DEBUG)
INCLUDE := -Iinclude
LDFLAGS := -static-libgcc
SDL_LIBS := -lSDL2main -lSDL2
LIBS :=  -L$(CURDIR)/build  -Wl,-rpath=$(CURDIR)/build -lgrid_engine $(SDL_LIBS)

ifeq ($(OS), Windows_NT)
    LIB_OUT := build/grid_engine.dll
    LIB_IMPLIB := -Wl,--out-implib,build/libgrid_engine.a
    LIB_LIBS := $(SDL_LIBS)
else
    LIB_OUT := build/libgrid_engine.so
    LIB_IMPLIB :=
    LIB_LIBS :=
endif

all: lib tests

lib:
	mkdir -p build/tmp
	gcc $(CFLAGS) $(INCLUDE) -c -o build/tmp/engine.o src/engine.c
	gcc $(CFLAGS) $(INCLUDE) -c -o build/tmp/ez_loop.o src/ez_loop.c
	gcc -shared -o $(LIB_OUT) $(LIB_IMPLIB) build/tmp/engine.o build/tmp/ez_loop.o $(LIB_LIBS)

tests: lib
	mkdir -p build/tmp
	gcc $(CFLAGS) $(INCLUDE) -c -o build/tmp/test_main.o test/test_main.c
	gcc $(CFLAGS) $(INCLUDE) -c -o build/tmp/test_ez_main.o test/test_ez_main.c
	gcc $(LDFLAGS) -o build/test build/tmp/test_main.o $(LIBS)
	gcc $(LDFLAGS) -o build/test_ez build/tmp/test_ez_main.o $(LIBS)
	if [ -f "/mingw64/bin/SDL2.dll" ]; then cp "/mingw64/bin/SDL2.dll" -t build; fi

clean:
	-rm -rf build
