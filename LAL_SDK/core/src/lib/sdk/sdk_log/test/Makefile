#####################################################################
# 日   期: 2023-01-30
# 作   者: qinmingsheng
# 说   明：生成可执行文件Makefile
# 修改记录：		内容				日期			作者
# 1、			 初始版本  			2023/01/30	 	   APOI
#
# 版   权：Copyright (c) 2023 by SofarSolar, All Rights Reserved. 
#####################################################################


#目标的可执行文件名
TARGET := log_test

#工程buildroot的库绝对路径
#BUILDROOT_PATH = /home/work/M6Y2C-iMX6ULL/system/rootfs/buildroot-2020.08/output/staging/

#编译器 
CROSS_COMPILE = arm-linux-gnueabihf-
#CROSS_COMPILE = 
CC = $(CROSS_COMPILE)gcc
STRIP = $(CROSS_COMPILE)strip

#告警及优化等级
CFLAGS := -g -Wall -O1

# LIBS 告诉链接器要链接哪些库文件（库名）
#LIBS = -lpthread -lsqlite3 -lssl -lcrypto -ldl -l_sdk -l_lal
LIBS =  -l_sdk_log -lzlog

# LDFLAGS 告诉链接器从哪里寻找库文件（库路径）
#LDFLAGS = --sysroot=$(BUILDROOT_PATH) -L../../lib 
LDFLAGS =  -L../ -L../../../../lib
#头文件路径
#INC = -I../../include -I../../include/sdk -I../../include/lal  -I$(BUILDROOT_PATH)usr/include 
INC = -I..  -I../../../../include 

# 列出当前目录下所有以“.c”结尾的文件
SOURCE = $(wildcard *.c)

# 调用patsubst函数，生成与源文件对应的“.o”文件列表
OBJS := $(patsubst %.c, %.o, $(SOURCE))

RM = rm -f

#将生成的.o文件链接成可执行文件
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

#编译成.o文件 
$(OBJS):%.o: %.c
	$(CC) $(CFLAGS) $(INC) -o $@ -c $<


# 声明伪目标
.PHONY : clean

# 删除所有中间文件和目标文件
clean :
	$(RM) $(TARGET) $(OBJS)
