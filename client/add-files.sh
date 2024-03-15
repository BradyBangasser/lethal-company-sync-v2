#!/usr/bin/env sh

# This is a script to automate adding files to cmake

echo "set(SOURCE_FILES" > files.cmake
find src -ls | grep -E "\.(c|cpp)" | awk -v q='\"' -v t='\t' '{ print "   ",q$(11)q }' >> files.cmake
echo ")" >> files.cmake