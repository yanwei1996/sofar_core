##############################################
# 日期: 2022-3-9
# 作者: yanwei
# 说明：test
##############################################

#目标的可执行文件名
all : wifi_test cellular4G_test net_public_test store_test upgrade_test drms_test

# 列出当前目录下所有以“.c”结尾的文件
outfile = $(wildcard *_test)

#编译器 
CROSS_COMPILE = arm-linux-gnueabihf-
CC = $(CROSS_COMPILE)gcc
STRIP = $(CROSS_COMPILE)strip

#告警及优化等级
CFLAGS := -g -Wall -O2
# LIBS 告诉链接器要链接哪些库文件（库名） -lzlog -lsqlite3 -lssl -lcrypto 
LIBS = -lpthread  -ldl -l_sdk -l_lal  -lmodbus   -lzlog    -lsqlite3   -lssl -lcrypto 
# LDFLAGS 告诉链接器从哪里寻找库文件（库路径）
BUILDROOT_PATH = ../open_lib/sysroot
LDFLAGS = --sysroot=$(BUILDROOT_PATH) -L../core/src/lib -L../open_lib//sysroot/usr/lib -L../open_lib/lib -L../open_lib/sqlite/lib/lib -L../open_lib/zlog-1.2.15/lib/
#头文件路径
INC = -I../core/src/include -I../core/src/include/sdk -I../core/src/include/lal -I../core/src/include/libmodbus -I$(BUILDROOT_PATH)/usr/include\
  -I../core/src/lib/sdk/sdk_net\
  -I../core/src/lib/sdk/sdk_store\
  -I../core/src/lib/sdk/sdk_upgrade\
  -I../core/src/lib/sdk/sdk_drm
  






#wifi_test:wifi_test.o  
#	$(CC)   wifi_test.o  -o wifi_test $(CFLAGS) $(LDFLAGS) $(LIBS) 
#wifi_test.o: wifi_test.c
#	$(CC)  -c  wifi_test.c  -o wifi_test.o  $(CFLAGS) $(LDFLAGS) $(LIBS) $(INC)


wifi_test:
	$(CC)   wifi_test.c    -o     wifi_test        $(CFLAGS) $(LDFLAGS) $(LIBS) $(INC)

cellular4G_test:
	$(CC)   cellular4G_test.c   -o     cellular4G_test       $(CFLAGS) $(LDFLAGS) $(LIBS) $(INC)
	
net_public_test:
	$(CC)   net_public_test.c   -o     net_public_test       $(CFLAGS) $(LDFLAGS) $(LIBS) $(INC)
	
store_test:
	$(CC)   store_test.c   -o     store_test       $(CFLAGS) $(LDFLAGS) $(LIBS) $(INC)
	
upgrade_test:
	$(CC)   upgrade_test.c   -o     upgrade_test       $(CFLAGS) $(LDFLAGS) $(LIBS) $(INC)

drms_test:
	$(CC)   drms_test.c   -o     drms_test       $(CFLAGS) $(LDFLAGS) $(LIBS) $(INC)
# 声明伪目标
.PHONY : clean

# 删除所有中间文件和目标文件
clean :
	
	#rm -r *o
	rm   -f $(outfile)
