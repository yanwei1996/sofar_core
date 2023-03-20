#!/bin/bash
mkdir -p install
./configure --host=arm-linux-gnueabihf --enable-static --prefix=$PWD/install --exec_prefix=$PWD/install#预定义变量PWD表示当前目录
make
make install