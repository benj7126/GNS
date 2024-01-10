#!/bin/bash

make

if [ $? -eq 0 ]; then
    if [[ "$OSTYPE" == "linux-gnu"* ]]; then
        ./build/debug/GNS
    elif [[ "$OSTYPE" == "msys" ]]; then
        ./build/debug/GNS.exe
    fi
fi
