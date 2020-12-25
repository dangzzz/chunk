#!/bin/bash

chunktype="chunk_append"

if [ $1 == "a" ]
then
    chunktype="chunk_append"
    echo $chunktype
fi

cmake CMakeLists.txt -DCHUNK_TYPE=$chunktype
make