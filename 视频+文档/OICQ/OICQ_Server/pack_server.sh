#!/bin/sh
exe="oicqserver"
des="/opt/OICQ_Server"
deplist=$(ldd $exe | awk  '{if (match($3,"/")){ printf("%s "),$3 } }')
cp $deplist $des
