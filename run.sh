#!/bin/bash
# SPDX-FileCopyrightText: 2024 Wiktor Kwapisiewicz <wiktor@metacode.biz>
# SPDX-License-Identifier: Apache-2.0

set -eaxo pipefail

cargo build

LD_LIBRARY_PATH=${CARGO_TARGET_DIR:-target}/debug
gcc sample.c -L $LD_LIBRARY_PATH -l openpgp_cards -o sample.o
./sample.o
