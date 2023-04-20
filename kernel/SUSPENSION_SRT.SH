#!/bin/bash
FILE=kernel
make $FILE
if test -f "./$FILE"; then
    ./$FILE ./cfg/suspension_srt.config
fi