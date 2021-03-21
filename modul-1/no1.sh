#!/bin/bash

n_input=$#
mod=$(( $n_input % 2 ))
sum=0

if [ $mod -eq 0 ]
then
  echo "Anda belum beruntung, coba lagi."
else
  for i in $*
    do
        let sum=$sum+$i
    done
  echo $sum
fi
