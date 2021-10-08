#!/bin/bash

make all

if [ $? == 0 ]; then
    ./c202-test > c202.out
    ./c202-advanced-test > c202-advanced.out
    ./c204-test > c204.out
    ./c204-advanced-test > c204-advanced.out
    ./c206-test > c206.out
    ./c206-advanced-test > c206-advanced.out
fi

declare -a arr=("diff c202.out expect-c202.output"
                "diff c202-advanced.out expect-c202-advanced.output"
                "diff c204.out expect-c204.output"
                "diff c204-advanced.out expect-c204-advanced.output"
                "diff c206.out expect-c206.output"
                "diff c206-advanced.out expect-c206-advanced.output")

for i in "${arr[@]}" 
do
    eval "$i"
    if [ $? == 0 ]; then
        echo -e "$i \tPASSED!\n"
    fi
done
