#!/bin/bash

chunktype="chunk_append"

if [ "$1" == "a" ]
then
    chunktype="chunk_append"
    echo $chunktype
fi

if [ "$1" == "sf" ]
then
    chunktype="chunk_split_follow"
    echo $chunktype
fi

if [ "$1" == "ss" ]
then
    chunktype="chunk_split_search"
    echo $chunktype
fi

#git clean -f -d -x
rm -rf /mnt/pmem/dz*
rm -rf bin
rm -rf lib
cmake CMakeLists.txt -DCHUNK_TYPE=$chunktype -DCMAKE_BUILD_TYPE=DEBUG
make