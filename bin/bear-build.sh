#!/bin/bash
set -Eeuxo pipefail
rm -rf build
mkdir build
cd build
cmake ..
make -j5
