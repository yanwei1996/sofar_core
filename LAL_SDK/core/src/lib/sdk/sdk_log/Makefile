#####################################################################
# 日   期: 2022-6-18
# 作   者: quliuliu
# 说   明：生成静态库和动态库Makefile
# 修改记录：		内容				日期			作者
# 1、修改编译级别，打开警告便于调试		  2023/0302/01	 APOI
#
# 版   权：Copyright (c) 2023 by SofarSolar, All Rights Reserved. 
#####################################################################

LIB_TARGET = sdk_log
LIB_TARGET_A = lib_$(LIB_TARGET).a
LIB_TARGET_SO = lib_$(LIB_TARGET).so

#编译器 
CROSS_COMPILE = arm-linux-gnueabihf-
#CROSS_COMPILE = 
CC = $(CROSS_COMPILE)gcc
AR = $(CROSS_COMPILE)ar
STRIP = $(CROSS_COMPILE)strip

#头文件路径
INC = -I../../../include 

# 列出当前目录下所有以“.c”结尾的文件
SOURCE = $(wildcard *.c)
# 调用patsubst函数，生成与源文件对应的“.o”文件列表
OBJS := $(patsubst %.c, %.o, $(SOURCE))

RM = rm -f

all:$(LIB_TARGET_A) $(LIB_TARGET_SO)

#生成.a静态库文件
$(LIB_TARGET_A): $(OBJS)
	$(AR) -crv $@ $(OBJS)

#生成.so动态库文件
$(LIB_TARGET_SO): $(OBJS)
	$(CC) -shared   -o $@ $(OBJS)

#生成.o文件 ==> %.o: %.c
$(OBJS): $(wildcard *.c)
	$(CC) $(INC) -Wall -fPIC -O1 -c $<  -o $@

clean:
	$(RM) $(LIB_TARGET_A) $(LIB_TARGET_SO) $(OBJS)
