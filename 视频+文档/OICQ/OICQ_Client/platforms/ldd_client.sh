#!/bin/sh
exe="libqxcb.so"
des="$(pwd)/../"
deplist=$(ldd $exe | awk  '{if (match($3,"/")){ printf("%s "),$3 } }')
cp $deplist $des
