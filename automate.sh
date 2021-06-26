#!/bin/bash
i=250000000
while [ $i -lt 10000000000 ]
do
    base64 /dev/urandom | head -c $i > random.txt
    ./aar.exe -e random.txt
    rm random.txt
    i=`expr $i + 250000000`
done
