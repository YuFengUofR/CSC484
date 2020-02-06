#!/bin/bash

SOLVER=push_relabel

touch ${SOLVER}_result.txt

for i in 0{1..9} {10..16}
do
  echo "./${SOLVER} tests/${i}.in >> ${SOLVER}_result.txt"
  time ./${SOLVER} tests/${i}.in 900000000 >> ${SOLVER}_result.txt
done
