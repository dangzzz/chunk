#!/bin/bash

chunktype="chunk_append"

if [ $1 == "a" ]
then
    chunktype="chunk_append"
    echo $chunktype
fi

git 
git clean -f -d -x
cmake CMakeLists.txt -DCHUNK_TYPE=$chunktype -DCMAKE_BUILD_TYPE=DEBUG
make