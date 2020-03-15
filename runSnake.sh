#!/bin/sh

mkdir bin
cd ./tools
make all
cd ../bin
clear
./runSnake
