#!/bin/bash

#batch compile for all .cpp files in ./code

path=./code

for file in $(ls $path)
        do
		filename="${file%.*}"
		mpic++ $path/$file -o ./test/$filename
        done
