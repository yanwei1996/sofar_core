##############################################
# 日期: 2023-2-10
# 作者: duyumeng
# 说明：生成静态库和动态库Makefile
##############################################

# #将.c 编译成.o 文件
# 	gcc -fPIC -c test.c
# #由.o 文件创建静态库
# 	ar -crv lib_test.a test.o
# #由.o 文件创建动态库文件
# 	gcc -shared -o lib_test.so test.o


LIB_TARGET = sdk_modbus
LIB_TARGET_A = lib_$(LIB_TARGET).a
LIB_TARGET_SO = lib_$(LIB_TARGET).so

#编译器 
CROSS_COMPILE = arm-linux-gnueabihf-
CC = $(CROSS_COMPILE)gcc
AR = $(CROSS_COMPILE)ar
STRIP = $(CROSS_COMPILE)strip

# LIBS 告诉链接器要链接哪些库文件（库名）
LIBS = -lpthread -lsqlite3 -lssl -lcrypto -ldl -lmodbus -l_sdk

# LDFLAGS 告诉链接器从哪里寻找库文件（库路径）
LDFLAGS = --sysroot=$(BUILDROOT_PATH2) --sysroot=$(BUILDROOT_PATH1) -L../../lib

#头文件路径
INC = -I../../include -I../../../include/libmodbus

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
	$(CC) -shared -o $@ $(OBJS)

#生成.o文件 ==> %.o: %.c
$(OBJS): $(wildcard *.c)
	$(CC) $(INC) -fPIC -c $<  -o $@

clean:
	$(RM) $(LIB_TARGET_A) $(LIB_TARGET_SO) $(OBJS)
