#!/bin/bash

if [ "$1" = "release" ]
then
    ./build/release/bin/dotvillage 
else
    ./build/debug/bin/dotvillage 
fi