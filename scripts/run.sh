#!/bin/bash

if [ "$1" = "release" ]
then
    ./build/release/bin/DotVillage 
else
    ./build/debug/bin/DotVillage 
fi