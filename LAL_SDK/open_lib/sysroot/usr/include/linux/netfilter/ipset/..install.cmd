cmd_/home/sofar/Desktop/imx6ull_bsp/rootfs/buildroot-2020.08/output/host/arm-buildroot-linux-gnueabihf/sysroot/usr/include/linux/netfilter/ipset/.install := /bin/bash scripts/headers_install.sh /home/sofar/Desktop/imx6ull_bsp/rootfs/buildroot-2020.08/output/host/arm-buildroot-linux-gnueabihf/sysroot/usr/include/linux/netfilter/ipset ./include/uapi/linux/netfilter/ipset ip_set.h ip_set_bitmap.h ip_set_hash.h ip_set_list.h; /bin/bash scripts/headers_install.sh /home/sofar/Desktop/imx6ull_bsp/rootfs/buildroot-2020.08/output/host/arm-buildroot-linux-gnueabihf/sysroot/usr/include/linux/netfilter/ipset ./include/linux/netfilter/ipset ; /bin/bash scripts/headers_install.sh /home/sofar/Desktop/imx6ull_bsp/rootfs/buildroot-2020.08/output/host/arm-buildroot-linux-gnueabihf/sysroot/usr/include/linux/netfilter/ipset ./include/generated/uapi/linux/netfilter/ipset ; for F in ; do echo "\#include <asm-generic/$$F>" > /home/sofar/Desktop/imx6ull_bsp/rootfs/buildroot-2020.08/output/host/arm-buildroot-linux-gnueabihf/sysroot/usr/include/linux/netfilter/ipset/$$F; done; touch /home/sofar/Desktop/imx6ull_bsp/rootfs/buildroot-2020.08/output/host/arm-buildroot-linux-gnueabihf/sysroot/usr/include/linux/netfilter/ipset/.install