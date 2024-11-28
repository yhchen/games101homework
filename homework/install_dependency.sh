#!/bin/bash

FILE_PATH=_build

cd `dirname $0`
cd ./eigen

echo "Installing dependencies..."
sudo apt-get install -y cmake g++ git

echo "Installing OpenCV..."
sudo apt-get install -y build-essential libgtk-3-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev libv4l-dev libxvidcore-dev libx264-dev openexr libatlas-base-dev libopenexr-dev libgstreamer-plugins-base1.0-dev libgstreamer1.0-dev python3-dev python3-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libdc1394-dev gfortran
sudo apt-get install -y libopencv-dev

echo "Building..."
mkdir -p ${FILE_PATH}/
cd ${FILE_PATH}/
cmake ..
make -j8
sudo make install
cd ../..

echo "Done!"


