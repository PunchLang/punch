#!/usr/bin/env sh

set -evx

mkdir -p external

cd lib
if [ ! -d "googletest" ]; then
    git clone https://github.com/google/googletest.git googletest
    rm -rf googletest/.git
fi
