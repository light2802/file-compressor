#!/bin/bash

input=10000
count=5
output="results.csv"
type="exe"
while getopts i:c:o:t: flag
do
    case "${flag}" in
        u) input=${OPTARG};;
        a) count=${OPTARG};;
        f) output=${OPTARG};;
        t) type=${OPTARG};;
    esac
done

echo "START................................"
echo "Press Ctrl + C to end midway........."
i=1


lorem -w $input > random.txt

while [ $i -lt $count ]
do
    ./aar.exe -e random.txt -r $output
    rm random.txt
    mv random.txt.aar random.txt
    i=`expr $i + 1`
done

rm random.txt

echo "END.................................."
