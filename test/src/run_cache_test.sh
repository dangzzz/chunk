#!/bin/bash

y=1

for((i=1;i<=64;i=i+y));  
do   
./bin/che_test $i y

if [ i == 8 ]
then
    y=2
fi

if [ i == 36 ]
then
    y=8
fi

done  