#!/bin/bash

# Copyright (c) 2019 Tim Perkins

set -o errexit
set -o nounset
set -o pipefail
IFS=$'\n\t'

readonly C_FILENAME_REGEX='.*\.[ch]$'
readonly C_LOCKFILE_REGEX='.*\.#.+\.[ch]$'
readonly DIRS='"src" "include" "test"'

readonly SCRIPT_DIR="$(dirname "$(readlink -f "$0")")"

readonly lockfiles="$(cd "${SCRIPT_DIR}" && eval "find ${DIRS} -regex '${C_LOCKFILE_REGEX}'")"
if [ -n "${lockfiles}" ]; then
    echo "Emacs Lockfiles detected! Save your files!" 2>&1
    exit 1
fi

(cd "${SCRIPT_DIR}" && eval "find ${DIRS} -regex '${C_FILENAME_REGEX}'" | xargs clang-format -i)

exit 0
