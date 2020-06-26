# Copyright (c) 2019 Tim Perkins

SHELL := /bin/bash
.ONESHELL:
.SHELLFLAGS := -o errexit -o nounset -o pipefail -c
.DELETE_ON_ERROR:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

DEBUG := -O0 -g
CFLAGS := -std=c11 -Wall -Wextra -Werror -fPIC $(DEBUG)
INCLUDE := -Iinclude
LDFLAGS := -static-libgcc

#######################
# GRID ENGINE LIBRARY #
#######################

SRCS := src/coord.c src/engine.c src/event.c src/ez_loop.c src/grid.c src/log.c src/utils.c
OBJS := $(patsubst src/%.c,build/tmp/%.o,$(SRCS))

SDL_LIBS := -lSDL2main -lSDL2
ifeq ($(OS), Windows_NT)
  LIB_GRID_ENGINE := build/grid_engine.dll
  LIB_GE_LIBS := $(SDL_LIBS)
  LIB_GE_IMPLIB := -Wl,--out-implib,build/libgrid_engine.a
else
  LIB_GRID_ENGINE := build/libgrid_engine.so
  LIB_GE_LIBS :=
  LIB_GE_IMPLIB :=
endif

all: $(LIB_GRID_ENGINE)

build/tmp:
	mkdir -p $@

build/tmp/%.o: src/%.c | build/tmp
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(LIB_GRID_ENGINE): $(OBJS)
	$(CC) -shared $^ $(LIB_GE_LIBS) $(LIB_GE_IMPLIB) -o $@

#####################
# GRID ENGINE TESTS #
#####################

TESTS := build/test_conway build/test_pong build/test_langton
TEST_LIBS := -L$(CURDIR)/build -Wl,-rpath=$(CURDIR)/build -lgrid_engine $(SDL_LIBS)

ifeq ($(OS), Windows_NT)
  COPY_DLLS := build/SDL2.dll
else
  COPY_DLLS :=
endif

tests: $(LIB_GRID_ENGINE) $(TESTS)

build/tmp/%.o: test/%.c | build/tmp
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

build/test_%: build/tmp/test_%.o $(LIB_GRID_ENGINE) | $(COPY_DLLS)
	$(CC) $(LDFLAGS) $< $(TEST_LIBS) -o $@

build/SDL2.dll: /mingw64/bin/SDL2.dll
	cp $< $@

#########
# OTHER #
#########

clean:
	-rm -rf build

# List of all fake targets
.PHONY: all tests clean
