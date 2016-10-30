#!/bin/bash

# parse command line args
input=("$@")

exec_1=${input[0]}  # fst executable
exec_2=${input[1]}  # snd executable

params=$(cat ${input[2]}) # location of param list
params=(${params[@]})

len=${#params[@]}

ind=0
cnt=1

# loops through arguments 6 at a time and runs
# exec_1 and exec_2 against each other
while [ $ind -lt $len ]; do

  # collect the results of time ./exec_n <6 args>
  # into a single result variable
  one=$((time ./$exec_1 ${params[@]:$ind:6}) 2>&1)
  two=$((time ./$exec_2 ${params[@]:$ind:6}) 2>&1)

  # split results into arrays
  one=(${one[@]})
  two=(${two[@]})

  # print out comparison between the two executables
  # for each case

# ==========================
  #Case n: name of case
  # ==========================
  # x-range num
  # y-range maxiter

  #        one       two
  #count : one_count two_count
  #real  : one_time  two_time
  #user  : one_time  two_time
  #sys   : one_time  two_time
  # ==========================

  printf "\n"
  printf "================================"
  printf "\nCase %d: %s\n"                  $cnt  ${params[$ind+6]}
  printf "================================"

  printf "\nx = {%g, %g} "                  ${params[$ind]} ${params[$ind+1]}
  printf "\tnum     = %g\n"                 ${params[$ind+4]}
  printf "y = {%g, %g} "                    ${params[$ind+2]} ${params[$ind+3]}
  printf "\tmaxiter = %g\n\n"               ${params[$ind+5]}
  printf "%12s %10s\n"                      $exec_1 $exec_2
  printf "%-6s: %-10d %-10d\n" "count"      ${one[0]} ${two[0]}
  printf "%-6s: %-10s %-10s\n"              ${one[1]} ${one[2]} ${two[2]}
  printf "%-6s: %-10s %-10s\n"              ${one[3]} ${one[4]} ${two[4]}
  printf "%-6s: %-10s %-10s\n"              ${one[5]} ${one[6]} ${two[6]}
  printf "================================"
  printf "\n"

  # increase iteration
  let ind=ind+7
  let cnt=cnt+1
done

printf "\n"
