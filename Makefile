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
GE_OBJS := $(patsubst $(GE_SRC_DIR)/%.c,$(GE_BLD_DIR)/tmp/%.o,$(GE_SRCS))
GE_DEPS := $(patsubst $(GE_BLD_DIR)/tmp/%.o,$(GE_BLD_DIR)/tmp/%.d,$(GE_OBJS))

# The library is either an .so or .dll. The import library is always a .dll.a,
# but should only exist on a Windows build.
GE_LIB_EXT := $(if $(IS_WIN),dll,so)
GE_LIB_GE := $(GE_BLD_DIR)/libgrid_engine.$(GE_LIB_EXT)
GE_IMPLIB_GE := $(if $(IS_WIN),$(GE_BLD_DIR)/libgrid_engine.dll.a,)

# If the import library exists, then construct the linker argument
GE_WIN_OUT_IMPLIB := -Wl,--out-implib,$(GE_IMPLIB_GE)
GE_OUT_IMPLIB := $(if $(GE_IMPLIB_GE),$(GE_WIN_OUT_IMPLIB),)

SDL_LIBS := -lSDL2_image -lSDL2

# Also create build/tmp directory which is used for objects, etc
$(GE_BLD_DIR):
> $(MKDIR) -p $@/tmp

# Note that this will always generate a dependency file (.d) alongside the
# object file, so one will not exist without the other.
$(GE_BLD_DIR)/tmp/%.o: $(GE_SRC_DIR)/%.c | $(GE_BLD_DIR)
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

GE_DEMO_SRCS := $(wildcard $(GE_DEMO_DIR)/demo_*.c)
GE_DEMO_OBJS := $(patsubst $(GE_DEMO_DIR)/%.c,$(GE_BLD_DIR)/tmp/%.o,$(GE_DEMO_SRCS))
GE_DEMO_DEPS := $(patsubst $(GE_BLD_DIR)/tmp/%.o,$(GE_BLD_DIR)/tmp/%.d,$(GE_DEMO_OBJS))

GE_DEMOS := $(patsubst $(GE_BLD_DIR)/tmp/%.o,$(GE_BLD_DIR)/%,$(GE_DEMO_OBJS))

# This rule is slightly different then the above to match the demo sources
$(GE_BLD_DIR)/tmp/demo_%.o: $(GE_DEMO_DIR)/demo_%.c | $(GE_BLD_DIR)
> $(call make-depend,$<,$@,$(subst .o,.d,$@))
> $(CC) $(CFLAGS) -c $< -o $@

# HACK Any DLLs we want to copy to the build directory
GE_CP_DLL := $(if $(IS_WIN),$(GE_BLD_DIR)/SDL2.dll,)

# Copy DLLs on Windows, using VPATH to locate them
$(GE_BLD_DIR)/%.dll: %.dll | $(GE_BLD_DIR)
> $(CP) $< $@

# Usage: $(call get-demo-target-for-eval,demo)
#
# This will result in a rule to build a demo executable which must then be
# evaluated. Note that $$(...) will expand to $(...), which can then expanded in
# the final eval when the rule is instantiated.
define get-demo-target-for-eval
$1: $$(patsubst $$(GE_BLD_DIR)/demo_%,$$(GE_BLD_DIR)/tmp/demo_%.o,$1) \
        $$(GE_LIB_GE) | $$(GE_BLD_DIR) $$(GE_CP_DLL)
> $$(CC) $$(CFLAGS) $$(LDFLAGS) $$^ -o $$@
endef

# For each demo target, instantiate the rule by evaluating it
$(foreach demo,$(GE_DEMOS),$(eval $(call get-demo-target-for-eval,$(demo))))

ifneq ($(MAKECMDGOALS), clean)
  -include $(GE_DEMO_DEPS)
endif

.PHONY: demos
demos: $(GE_DEMOS)

#########
# OTHER #
#########

.PHONY: clean
clean:
> -$(RM) -r build
