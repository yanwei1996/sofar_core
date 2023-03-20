/***************************************************************************
 * 时间：2022.06.23
 * 作者：quliuliu
 * 说明：首航新能源C/C++编程规范V2.0
 * 作用：对数据类型进行统一命名
 */

#ifndef __DATA_TYPES_H__
#define __DATA_TYPES_H__

#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>


// 在gcc-linaro-6.5.0-2018.12-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/sys/types.h被定义
// typedef char 	    int8_t;
// typedef short       	int16_t;
// typedef int 	        int32_t;
// typedef long 	    int64_t;

typedef float 	        float32_t;
typedef double	        float64_t;

// 在gcc-linaro-6.5.0-2018.12-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include/stdint.h被定义
// typedef unsigned char   uint8_t;
// typedef unsigned short  uint16_t;
// typedef unsigned int    uint32_t;
// typedef unsigned long   uint64_t;



#endif /* __DATA_TYPES_H__ */