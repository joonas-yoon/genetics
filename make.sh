#!/bin/sh

filename=$1

is_cpp=`echo "$filename" | egrep "^.+\.cpp$" | wc -l`
if [ $is_cpp -lt 1 ]
then
    filename='main.cpp'
    echo "Autodetect... '$filename'"
fi

echo "Compile with g++... $filename"

# delete file extention
name=${filename%\.cpp}

g++ $filename -std=c++11 -w -O2 -o "$name.runme"
echo "+------------------------------------------+"
beginTime=$(date +%s%N)
result=`"./$name.runme" < "./$name.in"`
endTime=$(date +%s%N)

elapsed=`expr $endTime - $beginTime`
elapsed=`expr $elapsed / 1000000`

echo "$result"

echo "+------------------------------------------+"
echo "time   : $elapsed ms"
echo "$result" > "./$name.out"

rm -rf "$name.runme"