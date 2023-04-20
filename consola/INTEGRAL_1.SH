#!/bin/bash
FILE=consola
make $FILE
if test -f "./$FILE"; then
    ./$FILE cfg/INTEGRAL_1.txt 2048
fi