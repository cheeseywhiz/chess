#!/bin/bash
set -Eeuxo pipefail

function main() {
    mkdir -p deps
    export CXXFLAGS="-O0 -g3"
    clone
    build
}

function clone() {
    pushd deps
    git clone https://github.com/an-tao/drogon.git
    pushd drogon
    git checkout "v1.2.0"
    git submodule update --init
    popd
    popd
}

function build() {
    pushd deps
    local DEPS="$(dirs | awk '{ print $1}')"
    pushd drogon
    rm -rf build
    mkdir build
    pushd build
    cmake -DCMAKE_INSTALL_PREFIX="${DEPS}" ..
    local N=$(( $(nproc) + 1 ))
    make -j"${N}"
    make install
    popd
    popd
    popd
}

main
