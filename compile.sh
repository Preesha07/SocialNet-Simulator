#!/bin/bash

g++ mainfile.cpp -o mainfile.out
if [ $? -ne 0 ]; then
    echo "Compilation failed."
    exit 1
fi

echo "Compilation successful. Running program..."
./mainfile.out