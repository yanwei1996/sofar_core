/************************************************************************************
 *Description: sdk_print_error
 *Created on: 2022-06-20
 *Author: quliuliu
 ************************************************************************************/

#ifndef __SDK_PRINT_ERROR_H__
#define __SDK_PRINT_ERROR_H__

int sdk_print(int type, int level, const char *file, const char *func, int line, const char *fmt, ...);

#define _ERROR_LEVEL_ 1 //取值为1时在终端打印，为2时打印到日志

#define _ERROR_
#ifdef  _ERROR_
    #define ERROR(fmt, ...) sdk_print(2, _ERROR_LEVEL_, __FILE__,__func__, __LINE__, fmt, ##__VA_ARGS__)
#else
    #define ERROR(fmt, ...)
#endif



#endif /* __SDK_PRINT_ERROR_H__ */