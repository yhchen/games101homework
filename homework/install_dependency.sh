#!/bin/bash

FILE_PATH=_build

cd `dirname $0`
cd ./eigen

echo "Installing dependencies..."
sudo apt-get install cmake
sudo apt-get install g++

echo "Building..."
mkdir -p ${FILE_PATH}/
cd ${FILE_PATH}/
cmake ..
make
sudo make install
cd ../..

echo "Done!"


