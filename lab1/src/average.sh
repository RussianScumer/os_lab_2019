#!/bin/bash

summ = $0
count = $0
for arg in "$@"; do 
   summ = $(( summ + arg ))
    count = $((count + 1))
done

avg = $((summ / count))

echo $count
echo $avg
