#!/bin/bash

if [ $# -eq 1 ]
    then
    echo "compiling.."
    make
    echo "executing.."
    time ./sokoban-lappis-solver $1
else
    echo "$0 error - you must provide one and only one parameter."
fi

