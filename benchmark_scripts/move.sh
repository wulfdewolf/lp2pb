#!/bin/bash

mkdir instances
rm instances.competition
mv encoding.asp instances
cat ./*instances.competition | while read F
do
    mv "$F" instances
done
rm *.asp 
mv instances/encoding.asp .
rm *instances.competition
rm filter.txt
