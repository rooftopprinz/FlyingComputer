#!/bin/sh

git submodule update --recursive

cd Logless && mkdir build && cd build && ../configure
make logless
make spawner