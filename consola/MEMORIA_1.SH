#!/bin/bash
FILE=consola
make $FILE
if test -f "./$FILE"; then
    ./$FILE cfg/MEMORIA_1.txt 4096
fi