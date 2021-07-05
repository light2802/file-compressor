#!/bin/bash
echo "start"
i=1
base64 /dev/urandom | head -c 1000000000 > random.txt
while [ $i -lt 500 ]
do
    ./a.out -e random.txt
    rm random.txt
    mv random.txt.aar random.txt
    i=`expr $i + 1`
done

echo "end"
