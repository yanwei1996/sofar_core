cmd_/home/sofar/Desktop/imx6ull_bsp/rootfs/buildroot-2020.08/output/host/arm-buildroot-linux-gnueabihf/sysroot/usr/include/misc/.install := /bin/bash scripts/headers_install.sh /home/sofar/Desktop/imx6ull_bsp/rootfs/buildroot-2020.08/output/host/arm-buildroot-linux-gnueabihf/sysroot/usr/include/misc ./include/uapi/misc cxl.h; /bin/bash scripts/headers_install.sh /home/sofar/Desktop/imx6ull_bsp/rootfs/buildroot-2020.08/output/host/arm-buildroot-linux-gnueabihf/sysroot/usr/include/misc ./include/misc ; /bin/bash scripts/headers_install.sh /home/sofar/Desktop/imx6ull_bsp/rootfs/buildroot-2020.08/output/host/arm-buildroot-linux-gnueabihf/sysroot/usr/include/misc ./include/generated/uapi/misc ; for F in ; do echo "\#include <asm-generic/$$F>" > /home/sofar/Desktop/imx6ull_bsp/rootfs/buildroot-2020.08/output/host/arm-buildroot-linux-gnueabihf/sysroot/usr/include/misc/$$F; done; touch /home/sofar/Desktop/imx6ull_bsp/rootfs/buildroot-2020.08/output/host/arm-buildroot-linux-gnueabihf/sysroot/usr/include/misc/.install