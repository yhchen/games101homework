#!/bin/bash

FILE_PATH=./code/_build

if [ -d ${FILE_PATH} ]; then
	rm -rf ${FILE_PATH}
fi;


mkdir ${FILE_PATH}
cd ${FILE_PATH}
cmake ..
make all -j8
mv Rasterizer ../exec
cd ..

