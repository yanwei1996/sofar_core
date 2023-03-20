##############################################
# 日期: 2022-6-18
# 作者: quliuliu
# 说明：生成静态库和动态库Makefile
##############################################

# #将.c 编译成.o 文件
# 	gcc -fPIC -c test.c
# #由.o 文件创建静态库
# 	ar -crv lib_test.a test.o
# #由.o 文件创建动态库文件
# 	gcc -shared -o lib_test.so test.o


LIB_TARGET = sdk_version
LIB_TARGET_A = lib_$(LIB_TARGET).a
LIB_TARGET_SO = lib_$(LIB_TARGET).so

#编译器 
CROSS_COMPILE = arm-linux-gnueabihf-
CC = $(CROSS_COMPILE)gcc
AR = $(CROSS_COMPILE)ar
STRIP = $(CROSS_COMPILE)strip

#头文件路径
INC = -I../ -I../../../include -I../../../include/lal


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
