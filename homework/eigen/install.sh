#!/bin/bash

cd `dirname $0`

echo "Installing dependencies..."
apt-get install cmake
apt-get install g++

echo "Building..."
mkdir -p build/
cd build/
cmake ..
make
make install
cd ..

echo "Done!"