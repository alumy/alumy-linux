#!/bin/bash

set -e

TOOLCHAIN_DIR=$PWD/cmake/toolchain
TOOLCHAIN=$1
BUILD_TYPE=$2

# Delete build directory
if [ -d build ]; then
	rm -r build
fi

# Make build directory
mkdir build && cd build

# add options '-LAH' if you want see all variables
cmake3 \
	-DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN_DIR/$TOOLCHAIN \
	-DCMAKE_BUILD_TYPE=$BUILD_TYPE \
	..
make all && make install

exit 0
