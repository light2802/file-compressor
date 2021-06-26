#!/usr/bin/env perl
i=1
while [ $i -lt 10000000000 ]
do
    lorem -w $i > random.txt
    ./aar.exe -e random.txt
    rm random.txt
    rm random.txt.aar
    i=`expr $i + 10`
done