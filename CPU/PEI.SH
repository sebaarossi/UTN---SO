#!/bin/bash
FILE=cpu
make $FILE
if test -f "./$FILE"; then
    ./$FILE cfg/pei.config
fi