#!/usr/bin/env sh

set -evx

mkdir -p test/lib

cd test/lib
if [ ! -d "googletest" ]; then
    git clone https://github.com/google/googletest.git googletest
    rm -rf googletest/.git
fi
