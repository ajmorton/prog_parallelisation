#!/bin/bash

#batch compile for all .cpp files in ./code

path=./code

for file in $(ls $path)
        do
		filename="${file%.*}"
		g++ $path/$filename.cpp -fopenmp -o ./execs/$filename
        done
