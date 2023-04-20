#!/bin/bash
FILE=cpu
make $FILE
if test -f "./$FILE"; then
    ./$FILE ./cfg/tlb_lru.config
fi