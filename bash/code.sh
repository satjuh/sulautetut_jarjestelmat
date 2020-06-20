#!/bin/bash

for i in $*; do
    echo $i
done

printf  "$0\n"
printf  "Date: $(date +"%d.%m.%Y")\n"
printf  "Time: $(date +"%T")\n"

re='^[0-9]+$'
first=-1
second=-2

until [[ $first =~ $re ]] && [[ $second =~ $re ]]; do
    printf "First number: "
    read first 
    printf "Second number: "
    read second
done

printf "$(($first * $second))\n"