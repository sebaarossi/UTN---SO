#!/bin/bash
FILE=consola
make $FILE
if test -f "./$FILE"; then
    ./$FILE cfg/BASE_1.txt 40
fi