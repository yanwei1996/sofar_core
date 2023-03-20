/************************************************************************************
 *项目: Inverter-320kw-ARM
 *描述: sdk_print
 *日期: 2022-06-20
 *作者: quliuliu
 ************************************************************************************/

#ifndef __SDK_PRINT_H__
#define __SDK_PRINT_H__

#include "data_types.h"

#define DEBUG_LEVEL_ 1 //取值为1时在终端打印，为2时打印到日志
#define ERROR_LEVEL_ 1 //取值为1时在终端打印，为2时打印到日志

#define _PRINT_
#ifdef  _PRINT_
    #define print_debug(fmt, ...) sdk_print(1, DEBUG_LEVEL_, __FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__)
    #define print_error(fmt, ...) sdk_print(2, ERROR_LEVEL_, __FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__)
#else
    #define print_debug(fmt, ...)
    #define print_error(fmt, ...)
#endif

#endif /* __SDK_PRINT_H__ */