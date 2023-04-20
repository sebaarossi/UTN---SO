#!/bin/bash
FILE=memoria
make $FILE
if test -f "./$FILE"; then
    ./$FILE ./cfg/mclock_m.config
fi