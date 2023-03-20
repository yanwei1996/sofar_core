#!/bin/bash
set -e
#set -v

#临时设置环境变量，只对当前运行的终端有效，每次重启终端时都要再设置一次
export PATH=$PWD/gcc-linaro-arm-linux-gnueabihf-4.9-2014.09_linux/bin:$PATH

echo "input 1: build all"
echo "input 2: clean all"
echo "input 3: show all sdk .so modules"

read -p "please select a num：" num

case "$num" in
    1)
		cd core 
		make clean 
		make
		cd ../testcode
		make clean
		make
		cd ../
    ;;
    2)
		cd core 
		make clean
		cd ../testcode
		make clean
		cd ../
    ;;
    3)
		objdump -tT core/src/lib/lib_sdk.so |grep sdk_ #| awk '{print $7}'
    ;;
    *)
		echo "input erro" 　　
esac





