/** 
 * @file          sdk_log.c
 * @brief         log打印接口函数功能实现
 * @author        qinmingsheng
 * @version       V0.0.1     初始版本
 * @date          2023/1/31 11:12:30
 * @copyright     Copyright (c) 2023 by SofarSolar, All Rights Reserved. 
 */


#include "sdk_log.h"
#include "zlog.h"
#include <stdio.h>
#include <stdint.h>
#include<sys/types.h>
#include<unistd.h>
#include <string.h>

#define sdk_printf(...)          printf(__VA_ARGS__)

#define sdk_printf(...)          printf(__VA_ARGS__)

static int8_t g_log_level = 3;        //运行过程中的打印级别 默认3，打印警告
static int8_t g_zlog_init_flag = -1;    //sdk_log_init 初始化结果标识，小于0失败，大于0成功

/** 
 * @brief        日志初始化函数
 * @return       [int32_t] 执行结果
 * @retval       =0 成功
 * @retval       <0 失败 
 * @note         
 */
int32_t sdk_log_init(void)
{
    int32_t ret;

	ret = dzlog_init("zlog.conf", "my_cat");
	if (ret) {
		printf("sdk_log init failed\n");
        g_zlog_init_flag = -1;
		return -1;
	}
    else
    {
        dzlog_info("hello, sdk_log");
        g_zlog_init_flag = 1;
    }

    return 0;
}

/** 
 * @brief        设置日志打印级别
 * @param        [in] log_level 要设置的打印级别的值
 * @note         
 */
void sdk_log_set_level(uint8_t log_level)
{
    g_log_level = log_level;
}

/** 
 * @brief        日志结束运行时调用
 * @param        [in] void
 * @note         和sdk_log_init函数成对出现
 */
void sdk_log_finish(void)
{
    void zlog_fini(void);
}

void log_a(const int8_t *p_format, ...)
{
    va_list args;
    uint8_t buf[MAX_PRINT_BUF_LEN] = {0};
    va_start(args, p_format);
    vsnprintf((char *)buf, MAX_PRINT_BUF_LEN, (const char *)p_format, args);    // 支持浮点数据输出
    va_end(args);
    if ( g_log_level > LOG_LVL_ASSERT )  //判断打印级别
    {
        if ( g_zlog_init_flag > 0 )     //zlog初始化成功
        {
            dzlog_fatal("%s",(const char *)buf);
        }
        else
        {
            sdk_printf("%s",(const char *)buf);
        }
    } 
}

void log_e(const int8_t *p_format, ...)
{
    va_list args;
    uint8_t buf[MAX_PRINT_BUF_LEN] = {0};
    va_start(args, p_format);
    vsnprintf((char *)buf, MAX_PRINT_BUF_LEN, (const char *)p_format, args);    // 支持浮点数据输出
    va_end(args);
    if ( g_log_level > LOG_LVL_ERROR )  //判断打印级别
    {
        if ( g_zlog_init_flag > 0 )     //zlog初始化成功
        {
            dzlog_error("%s",(const char *)buf);
        }
        else
        {
            sdk_printf("%s",(const char *)buf);
        }
    } 
}

void log_w(const int8_t *p_format, ...)
{
    va_list args;
    uint8_t buf[MAX_PRINT_BUF_LEN] = {0};
    va_start(args, p_format);
    vsnprintf((char *)buf, MAX_PRINT_BUF_LEN, (const char *)p_format, args);    // 支持浮点数据输出
    va_end(args);
    if ( g_log_level > LOG_LVL_WARN )  //判断打印级别
    {
        if ( g_zlog_init_flag > 0 )     //zlog初始化成功
        {
            dzlog_warn("%s",(const char *)buf);
        }
        else
        {
            sdk_printf("%s",(const char *)buf);
        }
    } 
}

void log_i(const int8_t *p_format, ...)
{
    va_list args;
    uint8_t buf[MAX_PRINT_BUF_LEN] = {0};
    va_start(args, p_format);
    vsnprintf((char *)buf, MAX_PRINT_BUF_LEN, (const char *)p_format, args);    // 支持浮点数据输出
    va_end(args);
    if ( g_log_level > LOG_LVL_INFO )  //判断打印级别
    {
        if ( g_zlog_init_flag > 0 )     //zlog初始化成功
        {
            dzlog_info("%s",(const char *)buf);
        }
        else
        {
            sdk_printf("%s",(const char *)buf);
        }
    } 
}

void log_d(const int8_t *p_format, ...)
{
    va_list args;
    uint8_t buf[MAX_PRINT_BUF_LEN] = {0};
    va_start(args, p_format);
    vsnprintf((char *)buf, MAX_PRINT_BUF_LEN, (const char *)p_format, args);    // 支持浮点数据输出
    va_end(args);
    if ( g_log_level > LOG_LVL_DEBUG )  //判断打印级别
    {
        if ( g_zlog_init_flag > 0 )     //zlog初始化成功
        {
            dzlog_debug("%s",(const char *)buf);
        }
        else
        {
            sdk_printf("%s",(const char *)buf);
        }
    } 
}
/**
 * @brief    log打印功能的实现函数
 * @param    [in] log_level 日志级别
 * @param    [in] format    MyParamDoc
 * @param    [in] ...       可变的输入参数
 */
void sdk_log(uint8_t log_level,const char *p_format, ...)
{
    va_list args;
    uint8_t buf[MAX_PRINT_BUF_LEN] = {0};
    va_start(args, p_format);
    vsnprintf((char *)buf, MAX_PRINT_BUF_LEN, p_format, args);    // 支持浮点数据输出
    va_end(args);
    switch (log_level)
    {
        case LOG_LVL_ASSERT:        //< 表明出现了不合理数据，导致系统卡死
            {
                if ( g_log_level > LOG_LVL_ASSERT )  //判断打印级别
                {
                    if ( g_zlog_init_flag > 0 )     //zlog初始化成功
                    {
                        dzlog_fatal("%s",(const char *)buf);
                    }
                    else
                    {
                        sdk_printf("%s",(const char *)buf);
                    }
                } 
            }
            break;
        case LOG_LVL_ERROR:
            {
                if ( g_log_level > LOG_LVL_ERROR )  //判断打印级别
                {
                    if ( g_zlog_init_flag > 0 )     //zlog初始化成功
                    {
                        dzlog_error("%s",(const char *)buf);
                    }
                    else
                    {
                        sdk_printf("%s",(const char *)buf);
                    }
                } 
            }
            break;
        case LOG_LVL_WARN:
            {
                if ( g_log_level > LOG_LVL_WARN )  //判断打印级别
                {
                    if ( g_zlog_init_flag > 0 )     //zlog初始化成功
                    {
                        dzlog_warn("%s",(const char *)buf);
                    }
                    else
                    {
                        sdk_printf("%s",(const char *)buf);
                    }
                }           
            }
            break;
        case LOG_LVL_INFO:
            { 
                if ( g_log_level > LOG_LVL_INFO )  //判断打印级别
                {
                    if ( g_zlog_init_flag > 0 )     //zlog初始化成功
                    {
                        dzlog_info("%s",(const char *)buf);
                    }
                    else
                    {
                        sdk_printf("%s",(const char *)buf);
                    }
                }                
            }
            break;
        case LOG_LVL_DEBUG:
            {
                if ( g_log_level > LOG_LVL_DEBUG )  //判断打印级别
                {
                    if ( g_zlog_init_flag > 0 )     //zlog初始化成功
                    {
                        dzlog_debug("%s",(const char *)buf);
                    }
                    else
                    {
                        sdk_printf("%s",(const char *)buf);
                    }
                }       
            }
            break;
        default:
            break;
    }
    
}

/**
 * @brief      按16进制模式进行打印
 * @param      [in] name    HEX项目的名字，将在日志头部显示
 * @param      [in] width   每行16进制的数据个数据，如：16 32.
 * @param      [in] buf     HEX数据区
 * @param      [in] size    数据总的大小
 */
int32_t sdk_log_hexdump(const int8_t *p_name, uint8_t width, uint8_t *p_buf, uint16_t size)
{
    int32_t ret = -1;
    uint8_t buf[MAX_PRINT_BUF_LEN] = {0};
    uint8_t  *p_tr = &buf[0];
    uint8_t *p_temp = p_buf;
    uint16_t i = 0;

    if ((NULL == p_buf) || (0 == size))
    {
        return(-1);
    }
    if ( g_log_level > LOG_LVL_DEBUG )  //固定为debug级别
    {
        if (NULL != p_name)
        {
            sdk_log(LOG_LVL_DEBUG, "\r\n%s", p_name);   //打印名字
        }
        for (i = 1; i <= size; i++)
        {
            p_tr += snprintf((char *)p_tr,MAX_PRINT_BUF_LEN, "%02X ", *p_temp);
            p_temp++;

            if (0 == i % width)
            {
                *p_tr = 0;
                sdk_log(LOG_LVL_DEBUG, "%s", &buf[0]);
                p_tr = &buf[0];
            }
        }

        if (size % width)
        {
            *p_tr = 0;
            sdk_log(LOG_LVL_DEBUG, "%s", &buf[0]);
        }
        ret = size;
    }
    return (ret);
}
