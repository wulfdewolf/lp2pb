#!/bin/bash

cat *instances.competition | while read F
do
    i=$(echo "$F" | cut -d - -f 1)
    mv ./instances/"$i"* .
done
rm -rf ./instances
mkdir instances
mv *.lp ./instances
rm *instances.competition