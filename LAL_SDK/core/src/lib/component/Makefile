##############################################
# 日期: 2022-6-18
# 作者: quliuliu
# 说明：生成静态库和动态库Makefile
##############################################

# #将.c 编译成.o 文件
# 	gcc -fPIC -c test.c
# #由.o 文件创建静态库
# 	ar -crv libtest.a test.o
# 将.c生成动态库文件
# 	gcc -shared -fPIC -o test.c libtest.so 

LIB_TARGET = cu
LIB_TARGET_A = lib_$(LIB_TARGET).a
LIB_TARGET_SO = lib_$(LIB_TARGET).so

INC_COMPONENT := ../../include/component
DEPEND_PATH := /home/work/M6Y2C-iMX6ULL/system/rootfs/buildroot-2020.08/output/staging

#编译器 
CROSS_COMPILE = arm-linux-gnueabihf-
CC = $(CROSS_COMPILE)gcc
AR = $(CROSS_COMPILE)ar
STRIP = $(CROSS_COMPILE)strip

#头文件路径
INC = -I../../include -I$(DEPEND_PATH)/usr/include \
	-Icu_component \
	-Icu_sofar_can \
	-Icu_sofar_sci
		 

# LIBS 告诉链接器要链接哪些库文件（库名）
LIBS = #-lsqlite3 -ldl

# LDFLAGS 告诉链接器从哪里寻找库文件（库路径）
LDFLAGS = #--sysroot=$(BUILDROOT_PATH) 

# 列出当前目录下所有以“.c”结尾的文件
SOURCE += $(wildcard cu_component/*.c)
SOURCE += $(wildcard cu_sofar_can/*.c)
SOURCE += $(wildcard cu_sofar_sci/*.c)

# 调用patsubst函数，生成与源文件对应的“.o”文件列表
OBJS := $(patsubst %.c, %.o, $(SOURCE))

# 同时生成动态库和静态库
all:$(LIB_TARGET_A) $(LIB_TARGET_SO)

# 生成.so动态库文件
$(LIB_TARGET_SO):
	$(CC)  -fPIC -shared  $(INC)  $(LIBS)  $(LDFLAGS)  $(SOURCE) -o ../$@ 

# 生成.a静态库文件
$(LIB_TARGET_A): $(OBJS)
	$(AR) -crv ../$@ $(OBJS)

# 生成.o文件 ==> %.o: %.c
$(OBJS): $(SOURCE)
	$(CC) $(INC) -fPIC -c $<  -o $@ 

RM = rm -rf

clean:
	$(RM) ../$(LIB_TARGET_A) ../$(LIB_TARGET_SO) $(OBJS) $(INC_COMPONENT)
