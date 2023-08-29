#!/bin/bash
g++ $1.cpp -o $1 -lglut -lGLU -lGL
if [ $? -eq 0 ]; then
    echo "Compilation Successful"
else
    echo "Compilation Failed"
    exit 1
fi
./$1