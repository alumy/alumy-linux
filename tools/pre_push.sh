#!/bin/bash

set -e

ROOTDIR=$PWD/../

echo "checking for gcc"
cd $ROOTDIR
./cmake_gen.sh gcc.cmake MinSizeRel 1 $ROOTDIR/build/release
cd $ROOTDIR/build
make -j32 && make test && make install

