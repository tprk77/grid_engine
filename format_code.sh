#!/bin/bash

# Copyright (c) 2019 Tim Perkins

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

set -o errexit
set -o nounset
set -o pipefail
IFS=$'\n\t'

# These regexes must match the entire path
readonly SRC_FILENAME_REGEX='.*\.[ch](pp)?$'
readonly SRC_LOCKFILE_REGEX='.*\.#.+\.[ch](pp)?$'
readonly IGNORE_REGEX='^(subprojects|\.ccls-cache)/.*'

readonly SCRIPT_DIR="$(dirname "$(readlink -f "$0")")"
readonly GIT_ROOT="$(cd "${SCRIPT_DIR}" && git rev-parse --show-toplevel)"

function show_usage {
    cat <<EOF 1>&2
Usage: $(basename "$0") [-a | --all] [-d | --diff] [-u | --upstream]
            [-n | --preview] [-c | --clang] [-t | --time]
            [-h | --help]

Formats source code for C (*.h, *.c), C++ (*.hpp, *.cpp), etc.

    -a | --all      Format all source files (default)
    -d | --diff     Format only changed source files, added and modified
    -u | --upstream Format only changed source files from upstream
    -n | --preview  Display paths of source files to be formatted
    -c | --clang    Display clang-format command and exit
    -t | --time     Display the run time of the script
    -h | --help     Display this help message

Always remember to format your code! ;-)

EOF
}

# Display the run time of the script on exit
show_run_time=0
readonly script_start_time="$(date +%s%6N)"
trap display_run_time EXIT
function display_run_time {
    # Do not show on errors, or when the option was not set
    err="$?"
    if [ "${err}" -ne 0 -o "${show_run_time}" -eq 0 ]; then
        return
    fi
    # Calculate the run time and display it
    readonly script_end_time="$(date +%s%6N)"
    readonly delta_time="$(("${script_end_time}" - "${script_start_time}"))"
    readonly delta_time_sec="$(echo | awk "{printf \"%0.3f\", ${delta_time}/1000000}")"
    readonly ESC_BOLD="$(tput bold)"
    readonly ESC_NORM="$(tput sgr0)"
    echo -e "${ESC_BOLD}${delta_time_sec} secs${ESC_NORM}" 1>&2
    show_run_time=0
}

function detect_clang_format {
    readonly CLANG_FORMAT_COMMANDS=(
        "clang-format-13"
        "clang-format-12"
        "clang-format-11"
        "clang-format-10"
        "clang-format"
    )
    # Echo the first clang-format command that exists
    for clang_format_cmd in "${CLANG_FORMAT_COMMANDS[@]}"; do
        if command -v "${clang_format_cmd}" &>/dev/null; then
            echo "${clang_format_cmd}"
            return
        fi
    done
}

# Detect the clang-format command
readonly CLANG_FORMAT_COMMAND="$(detect_clang_format)"

# Default options
format_mode="ALL"
show_preview=0
show_clang=0

# Convert long options to short options, preserving order
for arg in "${@}"; do
    case "${arg}" in
        "--all" ) set -- "${@}" "-a" ;;
        "--diff" ) set -- "${@}" "-d" ;;
        "--upstream" ) set -- "${@}" "-u" ;;
        "--preview" ) set -- "${@}" "-n" ;;
        "--clang" ) set -- "${@}" "-c" ;;
        "--time" ) set -- "${@}" "-t" ;;
        "--help" ) set -- "${@}" "-h" ;;
        * ) set -- "${@}" "${arg}" ;;
    esac
    shift
done

# Parse short options using getopts
while getopts "aduncth" arg &>/dev/null; do
    case "${arg}" in
        "a" ) format_mode="ALL" ;;
        "d" ) format_mode="DIFF" ;;
        "u" ) format_mode="UPSTREAM" ;;
        "n" ) show_preview=1 ;;
        "c" ) show_clang=1 ;;
        "t" ) show_run_time=1 ;;
        "h" ) show_usage ; show_run_time=0 ; exit 0 ;;
        "?" ) show_usage ; exit 1 ;;
    esac
done

# Shift positional arguments into place
shift "$(("${OPTIND}" - 1))"

# There are no positional arguments
if [ "${#}" -gt 0 ]; then
    show_usage
    exit 1
fi

# Bail if clang-format was not found
if [ -z "${CLANG_FORMAT_COMMAND}" ]; then
    echo "Could not detect clang-format command!" 1>&2
    exit 1
fi

# Show clang-format command and exit if the option was set
if [ "${show_clang}" -ne 0 ]; then
    command -v "${CLANG_FORMAT_COMMAND}"
    exit 0
fi

# Always work in the Git repo root directory
cd "${GIT_ROOT}"

# Bail if Emacs lockfiles are detected
readonly lockfiles="$(find . -regextype egrep -regex "${SRC_LOCKFILE_REGEX}" \
    | sed -e 's;^\./;;' | grep -E -v -e "${IGNORE_REGEX}" || true)"
if [ -n "${lockfiles}" ]; then
    echo "Emacs Lockfiles detected! Save your files!" 1>&2
    exit 1
fi

# Get the files to be formatted
format_files=""
if [ "${format_mode}" = "ALL" ]; then
    format_files="$(find . -regextype egrep -regex "${SRC_FILENAME_REGEX}" \
        | sed -e 's;^\./;;' | grep -E -v -e "${IGNORE_REGEX}" || true)"
elif [ "${format_mode}" = "DIFF" ]; then
    format_files="$(git diff --diff-filter=d --name-only HEAD \
        | grep -E -e "${SRC_FILENAME_REGEX}" \
        | grep -E -v -e "${IGNORE_REGEX}" || true)"
elif [ "${format_mode}" = "UPSTREAM" ]; then
    if ! git name-rev '@{u}' &>/dev/null; then
        echo "No upstream set! Cannot format files modified from upstream!" 1>&2
        exit 1
    fi
    format_files="$(git diff --diff-filter=d --name-only '@{u}' \
        | grep -E -e "${SRC_FILENAME_REGEX}" \
        | grep -E -v -e "${IGNORE_REGEX}" || true)"
fi

# Show preview and exit if the option was set
if [ "${show_preview}" -ne 0 ]; then
    echo "${format_files}" | LC_ALL="C.UTF-8" sort
    exit 0
fi

# Format all the necessary files
if [ -n "${format_files}" ]; then
    echo "${format_files}" | xargs -P 4 "${CLANG_FORMAT_COMMAND}" -i
fi

exit 0
