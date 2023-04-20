#!/bin/bash
FILE=kernel
make $FILE
if test -f "./$FILE"; then
    ./$FILE ./cfg/mclock.config
fi