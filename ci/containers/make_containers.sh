#!/bin/bash

# Copyright (c) 2020 Tim Perkins

set -o errexit
set -o nounset
set -o pipefail
IFS=$'\n\t'

readonly SCRIPT_DIR="$(dirname "$(readlink -f "$0")")"

docker build \
    -t tprk77/gridengineci:ubuntu1804 \
    - < "${SCRIPT_DIR}/Ubuntu1804Dockerfile"

docker build \
    -t tprk77/gridengineci:ubuntu2004 \
    - < "${SCRIPT_DIR}/Ubuntu2004Dockerfile"
