#!/bin/bash

chunktype="chunk_append"

if [ "$1" == "a" ]
then
    chunktype="chunk_append"
    echo $chunktype
fi

if [ "$1" == "s" ]
then
    chunktype="chunk_split"
    echo $chunktype
fi

#git clean -f -d -x
rm -rf /mnt/pmem/dz*
cmake CMakeLists.txt -DCHUNK_TYPE=$chunktype -DCMAKE_BUILD_TYPE=RELEASE
make