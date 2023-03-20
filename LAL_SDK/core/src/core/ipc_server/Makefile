
##############################################
# 日期: 2022-6-18
# 作者: quliuliu
# 说明：生成可执行文件Makefile
##############################################

#目标的可执行文件名
TARGET := core_ipc_server



#编译器 
CROSS_COMPILE = arm-linux-gnueabihf-
CC = $(CROSS_COMPILE)gcc
STRIP = $(CROSS_COMPILE)strip

#告警及优化等级
CFLAGS := -g -Wall -O2

# LIBS 告诉链接器要链接哪些库文件（库名） -lzlog -lsqlite3 -lssl -lcrypto 
LIBS = -lpthread  -ldl -l_sdk -l_lal  -lmodbus   -lzlog    -lsqlite3   -lssl -lcrypto 



#工程buildroot的库绝对路径
BUILDROOT_PATH = ../../../../open_lib/sysroot
# LDFLAGS 告诉链接器从哪里寻找库文件（库路径）
LDFLAGS = --sysroot=$(BUILDROOT_PATH) -L../../lib -L../../../../open_lib/sysroot/usr/lib -L../../../../open_lib/lib -L../../../../open_lib/sqlite/lib/lib -L../../../../open_lib/zlog-1.2.15/lib/

#头文件路径
INC = -I../../include -I../../include/sdk -I../../include/lal -I../../include/libmodbus  -I$(BUILDROOT_PATH)/usr/include 

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
