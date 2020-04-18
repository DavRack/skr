#!/bin/sh

gcc main.c -o skr

if [ "$1" = "-x" ]; then
    sudo ./skr
elif [ "$1" = "-g" ]; then
        gcc -g main.c -o skr
fi
