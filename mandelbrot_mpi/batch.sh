#!/bin/bash

#batch compile for all .cpp files in ./code

path=./code

rm ./test/execs/*

export OMPI_CXX="g++"
export OMPI_CXXFLAGS="-O3"


for file in $(ls $path)
        do
		filename="${file%.*}"
		mpic++ $path/$filename.cpp  -fopenmp -o ./test/execs/$filename
        done
