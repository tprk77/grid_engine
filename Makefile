# Copyright (c) 2020 Tim Perkins

#########
# SETUP #
#########

SHELL := /bin/bash
.ONESHELL:
.SHELLFLAGS := -o errexit -o nounset -o pipefail -c
.DELETE_ON_ERROR:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules
ifeq ($(origin .RECIPEPREFIX), undefined)
  $(error Please use a version of Make supporting .RECIPEPREFIX)
endif
.RECIPEPREFIX = >

# On a Windows build this will be set to "t", otherwise it's blank
IS_WIN = $(if $(findstring Windows_NT,$(OS)),t,)

# HACK Adjust the vpath to find libraries on Windows and MSYS2. This is
# necessary because without it, the libraries being used as prerequisites (and
# translated by .LIBPATTERNS) weren't getting located properly.
ifeq ($(IS_WIN), t)
  MINGW_LIB_DIR := /mingw64/lib
  MINGW_BIN_DIR := /mingw64/bin
  vpath %.a $(MINGW_LIB_DIR)
  vpath %.dll.a $(MINGW_LIB_DIR)
  vpath %.dll $(MINGW_BIN_DIR)
endif

# RELEASE, DEBUG, and SANITIZE variables can be overriden on the command line,
# and should be specified as "t", "true", "T", "TRUE", or "1" for a true value.
# Any other value will be considered at a false value.
RELEASE ?= t
IS_RELEASE := $(filter t true T TRUE 1,$(RELEASE))
DEBUG ?= $(if $(IS_RELEASE),f,t)
IS_DEBUG := $(filter t true T TRUE 1,$(DEBUG))
SANITIZE ?= $(if $(IS_RELEASE),f,t)
IS_SANITIZE := $(filter t true T TRUE 1,$(SANITIZE))

##########
# COMMON #
##########

# This is the default target, and should always be first
all:

CP := cp
MKDIR := mkdir

DEBUG_FLAGS := $(if $(IS_DEBUG),-O0 -g,)
SANITIZE_FLAGS := $(if $(IS_SANITIZE),-fsanitize=address -fsanitize=leak -fsanitize=undefined,)
CFLAGS := $(filter %,-std=c11 -Wall -Wextra -Werror -fPIC $(DEBUG_FLAGS) $(SANITIZE_FLAGS) $(CFLAGS))
LDFLAGS := -static-libgcc

# Usage: $(call make-depend,src,obj,dep)
#
# This function assumes a capable version of gcc which supports the -MM, -MF,
# -MP, and -MT options. As an explaination, -MM will generate dependencies, -MF
# will set the output filename, -MP will generate empty rules for prerequisites
# (which avoid issues with deleted files), and -MT set the target for the rule.
#
define make-depend
  $(CC) -MM -MF $3 -MP -MT $2 $(CFLAGS) $1
endef

#######################
# GRID ENGINE LIBRARY #
#######################

GE_SRC_DIR := src
GE_INC_DIR := include
GE_BLD_DIR := build

CFLAGS += -I $(GE_INC_DIR)

GE_SRCS := $(wildcard $(GE_SRC_DIR)/*.c)
GE_OBJS := $(patsubst $(GE_SRC_DIR)/%.c,$(GE_BLD_DIR)/%.o,$(GE_SRCS))
GE_DEPS := $(patsubst $(GE_BLD_DIR)/%.o,$(GE_BLD_DIR)/%.d,$(GE_OBJS))

# The library is either an .so or .dll. The import library is always a .dll.a,
# but should only exist on a Windows build.
GE_LIB_EXT := $(if $(IS_WIN),dll,so)
GE_LIB_GE := $(GE_BLD_DIR)/libgrid_engine.$(GE_LIB_EXT)
GE_IMPLIB_GE := $(if $(IS_WIN),$(GE_BLD_DIR)/libgrid_engine.dll.a,)

# If the import library exists, then construct the linker argument
GE_WIN_OUT_IMPLIB := -Wl,--out-implib,$(GE_IMPLIB_GE)
GE_OUT_IMPLIB := $(if $(GE_IMPLIB_GE),$(GE_WIN_OUT_IMPLIB),)

SDL_LIBS := -lSDL2

$(GE_BLD_DIR):
> $(MKDIR) -p $@

# Note that this will always generate a dependency file (.d) alongside the
# object file, so one will not exist without the other.
$(GE_BLD_DIR)/%.o: $(GE_SRC_DIR)/%.c | $(GE_BLD_DIR)
> $(call make-depend,$<,$@,$(subst .o,.d,$@))
> $(CC) $(CFLAGS) -c $< -o $@

$(GE_LIB_GE): $(GE_OBJS) $(SDL_LIBS) | $(GE_BLD_DIR)
> $(CC) $(CFLAGS) $(LDFLAGS) -shared $^ -o $@ $(GE_OUT_IMPLIB)

# Include the dependency files, but only if we're not cleaning out the build.
# Also if the dependency is missing, just ignore that error. If the dependency
# doesn't exist, neither does the object file.
ifneq ($(MAKECMDGOALS), clean)
  -include $(GE_DEPS)
endif

.PHONY: all
all: $(GE_LIB_GE)

#####################
# GRID ENGINE DEMOS #
#####################

GE_DEMO_DIR := demos

GE_DCNW_SRCS := $(GE_DEMO_DIR)/demo_conway.c
GE_DCNW_OBJS := $(patsubst $(GE_DEMO_DIR)/%.c,$(GE_BLD_DIR)/%.o,$(GE_DCNW_SRCS))
GE_DCNW_DEPS := $(patsubst $(GE_BLD_DIR)/%.o,$(GE_BLD_DIR)/%.d,$(GE_DCNW_OBJS))

GE_DEMO_CONWAY := $(GE_BLD_DIR)/demo_conway

GE_DLNG_SRCS := $(GE_DEMO_DIR)/demo_langton.c
GE_DLNG_OBJS := $(patsubst $(GE_DEMO_DIR)/%.c,$(GE_BLD_DIR)/%.o,$(GE_DLNG_SRCS))
GE_DLNG_DEPS := $(patsubst $(GE_BLD_DIR)/%.o,$(GE_BLD_DIR)/%.d,$(GE_DLNG_OBJS))

GE_DEMO_LANGTON := $(GE_BLD_DIR)/demo_langton

GE_DPNG_SRCS := $(GE_DEMO_DIR)/demo_pong.c
GE_DPNG_OBJS := $(patsubst $(GE_DEMO_DIR)/%.c,$(GE_BLD_DIR)/%.o,$(GE_DPNG_SRCS))
GE_DPNG_DEPS := $(patsubst $(GE_BLD_DIR)/%.o,$(GE_BLD_DIR)/%.d,$(GE_DPNG_OBJS))

GE_DEMO_PONG := $(GE_BLD_DIR)/demo_pong

GE_DPLT_SRCS := $(GE_DEMO_DIR)/demo_palette.c
GE_DPLT_OBJS := $(patsubst $(GE_DEMO_DIR)/%.c,$(GE_BLD_DIR)/%.o,$(GE_DPLT_SRCS))
GE_DPLT_DEPS := $(patsubst $(GE_BLD_DIR)/%.o,$(GE_BLD_DIR)/%.d,$(GE_DPLT_OBJS))

GE_DEMO_PALETTE := $(GE_BLD_DIR)/demo_palette

GE_DMZE_SRCS := $(GE_DEMO_DIR)/demo_maze.c
GE_DMZE_OBJS := $(patsubst $(GE_DEMO_DIR)/%.c,$(GE_BLD_DIR)/%.o,$(GE_DMZE_SRCS))
GE_DMZE_DEPS := $(patsubst $(GE_BLD_DIR)/%.o,$(GE_BLD_DIR)/%.d,$(GE_DMZE_OBJS))

GE_DEMO_MAZE := $(GE_BLD_DIR)/demo_maze

# HACK Any DLLs we want to copy to the build directory
GE_CP_DLL := $(if $(IS_WIN),$(GE_BLD_DIR)/SDL2.dll,)

$(GE_BLD_DIR)/%.o: $(GE_DEMO_DIR)/%.c | $(GE_BLD_DIR)
> $(call make-depend,$<,$@,$(subst .o,.d,$@))
> $(CC) $(CFLAGS) -c $< -o $@

$(GE_DEMO_CONWAY): $(GE_DCNW_OBJS) $(GE_LIB_GE) $(SDL_LIBS) | $(GE_BLD_DIR) $(GE_CP_DLL)
> $(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(GE_DEMO_LANGTON): $(GE_DLNG_OBJS) $(GE_LIB_GE) $(SDL_LIBS) | $(GE_BLD_DIR) $(GE_CP_DLL)
> $(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(GE_DEMO_PONG): $(GE_DPNG_OBJS) $(GE_LIB_GE) $(SDL_LIBS) | $(GE_BLD_DIR) $(GE_CP_DLL)
> $(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(GE_DEMO_PALETTE): $(GE_DPLT_OBJS) $(GE_LIB_GE) $(SDL_LIBS) | $(GE_BLD_DIR) $(GE_CP_DLL)
> $(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(GE_DEMO_MAZE): $(GE_DMZE_OBJS) $(GE_LIB_GE) $(SDL_LIBS) | $(GE_BLD_DIR) $(GE_CP_DLL)
> $(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

# Copy DLLs on Windows, using VPATH to locate them
$(GE_BLD_DIR)/%.dll: %.dll | $(GE_BLD_DIR)
> $(CP) $< $@

ifneq ($(MAKECMDGOALS), clean)
  -include $(GE_DCNW_DEPS)
  -include $(GE_DLNG_DEPS)
  -include $(GE_DPNG_DEPS)
  -include $(GE_DPLT_DEPS)
  -include $(GE_DMZE_DEPS)
endif

.PHONY: demos
demos: $(GE_DEMO_CONWAY) $(GE_DEMO_LANGTON) $(GE_DEMO_PONG) $(GE_DEMO_PALETTE) $(GE_DEMO_MAZE)

#########
# OTHER #
#########

.PHONY: clean
clean:
> -$(RM) -r build
