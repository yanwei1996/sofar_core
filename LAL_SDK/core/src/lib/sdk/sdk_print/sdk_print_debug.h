/************************************************************************************
 *Description: sdk_print_debug
 *Created on: 2022-06-20
 *Author: quliuliu
 ************************************************************************************/

#ifndef __SDK_PRINT_DEBUG_H__
#define __SDK_PRINT_DEBUG_H__

int sdk_print(int type, int level, const char *file, const char *func, int line, const char *fmt, ...);

#define _DEBUG_LEVEL_ 1 //取值为1时在终端打印，为2时打印到日志

#define _DEBUG_
#ifdef  _DEBUG_
    #define DEBUG(fmt, ...) sdk_print(1, _DEBUG_LEVEL_, __FILE__,__func__, __LINE__, fmt, ##__VA_ARGS__)
#else
    #define DEBUG(fmt, ...)
#endif



#endif /* __SDK_PRINT_DEBUG_H__ */