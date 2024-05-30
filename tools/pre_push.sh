#!/bin/bash

set -e

ROOTDIR=$PWD/../

echo "checking for gcc"
cd $ROOTDIR
./cmake_gen.sh gcc.cmake MinSizeRel 1 $ROOTDIR/build/install
cd $ROOTDIR/build
make -j32 && make test && make install

echo "checking for arm-linux-gnueabi-gcc"
cd $ROOTDIR
./cmake_gen.sh arm-linux-gnueabi.cmake MinSizeRel 1 $ROOTDIR/build/install
cd $ROOTDIR/build
make -j32 && make install

echo "checking for arm-buildroot-linux-musleabi-gcc"
cd $ROOTDIR
./cmake_gen.sh arm-buildroot-linux-musleabi.cmake MinSizeRel 1 $ROOTDIR/build/install
cd $ROOTDIR/build
make -j32 && make install



