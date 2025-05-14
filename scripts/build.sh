#!/bin/bash

rm -rf ./build; rm -rf ./install; rm -f ./lib/*
mkdir build
cd build

cmake ..
make
make install