#!/usr/bin/env -S bash

SOMEWHAT_REALDIR="$(realpath "$0")"
FILENAME="${SOMEWHAT_REALDIR##*/}"
PATHTO="${SOMEWHAT_REALDIR/$FILENAME/}"

function runInLocal() {
    if [[ "$(pwd)/" != "$PATHTO" ]]; then
        (cd "$PATHTO" && bash "$FILENAME")
        exit $?
    fi

    return 0
}

function buildAndRun() {
    make o3 && \
    ./buildO3/OpenGL/glfwExample

    return $?
}

runInLocal && buildAndRun
exit $?