# Copyright (c) 2019 Tim Perkins

DEBUG := -O0 -g
CFLAGS := -std=c11 -Wall -Wextra -Werror -fPIC $(DEBUG)
INCLUDE := -Iinclude
LDFLAGS := -static-libgcc
SDL_LIBS := -lSDL2main -lSDL2
LIBS :=  -L$(CURDIR)/build  -Wl,-rpath=$(CURDIR)/build -lgrid_engine $(SDL_LIBS)

ifeq ($(OS), Windows_NT)
    LIB_GRID_ENGINE := build/grid_engine.dll
    LIB_IMPLIB := -Wl,--out-implib,build/libgrid_engine.a
    LIB_LIBS := $(SDL_LIBS)
else
    LIB_GRID_ENGINE := build/libgrid_engine.so
    LIB_IMPLIB :=
    LIB_LIBS :=
endif

SRCS := src/coord.c src/engine.c src/event.c src/ez_loop.c src/grid.c src/log.c
OBJS := $(patsubst src/%.c,build/tmp/%.o,$(SRCS))

all: $(LIB_GRID_ENGINE)

clean:
	-rm -rf build

build/tmp:
	mkdir -p $@

#######################
# GRID ENGINE LIBRARY #
#######################

build/tmp/%.o: src/%.c | build/tmp
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(LIB_GRID_ENGINE): $(OBJS)
	$(CC) -shared $^ $(LIB_LIBS) $(LIB_IMPLIB) -o $@
