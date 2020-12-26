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
    git clone https://github.com/cheeseywhiz/drogon.git
    pushd drogon
    git config pull.rebase true
    git config credential.helper store
    git checkout fixes
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
