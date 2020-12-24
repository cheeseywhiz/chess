#!/bin/bash
set -Eeuxo pipefail
rm -rf compile_commands.json build
mkdir build
cd build
cmake ..
make -j5
