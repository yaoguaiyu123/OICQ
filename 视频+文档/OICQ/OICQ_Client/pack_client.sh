#!/bin/sh
exe="appoicqclient"
des="/opt/OICQ_Client"
deplist=$(ldd $exe | awk  '{if (match($3,"/")){ printf("%s "),$3 } }')
cp $deplist $des
