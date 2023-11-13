#!/bin/bash

set -eaxo pipefail

cargo build

LD_LIBRARY_PATH=${CARGO_TARGET_DIR:-target}/debug
gcc sample.c -L $LD_LIBRARY_PATH -l openpgp_cards -o sample.o
./sample.o
