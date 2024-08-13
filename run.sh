#!/bin/bash
SCRIPT_DIR=$(realpath $(dirname $0))
EXEC_FILE="main"
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${SCRIPT_DIR}
${SCRIPT_DIR}/${EXEC_FILE} "$@"
