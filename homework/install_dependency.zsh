#!/bin/zsh

FILE_PATH=_build

cd `dirname $0`
cd ./eigen

echo "Installing dependencies..."
brew install cmake git

echo "Installing pkg config"
brew install pkg-config

echo "Installing OpenCV..."
brew install opencv

echo "Building..."
mkdir -p ${FILE_PATH}/
cd ${FILE_PATH}/
cmake ..
make -j8
sudo make install
cd ../..

echo "Done!"


