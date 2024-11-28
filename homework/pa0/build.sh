#!/bin/bash

FILE_PATH=_build

if [ -d ${FILE_PATH} ]; then
	rm -rf ${FILE_PATH}
fi;


mkdir ${FILE_PATH}
cd ${FILE_PATH}
cmake ..
make all
mv Transformation ../exec
cd ..

