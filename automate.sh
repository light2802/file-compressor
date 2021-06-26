#!/bin/bash
i=0
while [ $i -lt 100 ]
do
    base64 /dev/urandom | head -c $i > random.txt
    ./aar.exe -e random.txt
    rm random.txt
    a=`expr $a + 1`
done