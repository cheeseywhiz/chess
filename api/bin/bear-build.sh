#!/bin/bash
set -Eeuxo pipefail
rm -rf compile_commands.json build
mkdir build
db.sh reset
cd build
cmake ..
make -j5
