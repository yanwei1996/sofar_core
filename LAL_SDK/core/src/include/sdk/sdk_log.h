/** 
 * @file          sdk_log.h
 * @brief         log日志相关功能
 * @author        qinmingsheng
 * @version       V0.0.1     初始版本
 * @date          2022/12/19 20:15:14
 * @lastAuthor    Please set LastEditors
 * @copyright     Copyright(c) 2022 by SofarSolar, All Rights Reserved. 
 */

#ifndef __SDK_LOG_H__
#define __SDK_LOG_H__

#include <stdio.h>
#include <stdint.h>

#define LOG_LVL_DISABLE                     0	    ///< 关闭调试
#define LOG_LVL_ASSERT                      1		///< 表明出现了不合理数据，导致系统卡死
#define LOG_LVL_ERROR                       2		///< 表明出现了系统错误和异常，无法正常完成目标操作。
#define LOG_LVL_WARN                        3		///< 表明系统出现轻微的不合理但不影响运行和使用；
#define LOG_LVL_INFO                        4		///< 用于打印程序应该出现的正常状态信息， 便于追踪定位
#define LOG_LVL_DEBUG                       5		///< 级别最低，可以随意的使用于任何觉得有利于在调试时更详细的了解系统运行状态的东东

#define MAX_PRINT_BUF_LEN   1024

/** 
 * @brief        日志初始化函数
 * @return       [int32_t] 执行结果
 * @retval       =0 成功
 * @retval       <0 失败 
 * @note         
 */
int32_t sdk_log_init(void);

/** 
 * @brief        设置日志打印级别
 * @param        [in] log_level 要设置的打印级别的值
 * @note         
 */
void sdk_log_set_level(uint8_t log_level);

/**
 * @brief      按16进制模式进行打印
 * @param      [in] p_name  HEX项目的名字，将在日志头部显示
 * @param      [in] width   每行16进制的数据个数据，如：16 32.
 * @param      [in] p_buf   HEX数据区
 * @param      [in] size    数据总的大小
 * @return     [int32_t] 执行结果
 * @retval     =0 成功
 * @retval     <0 失败 
 * @note 
 */
int32_t sdk_log_hexdump(const int8_t *p_name, uint8_t width, uint8_t *p_buf, uint16_t size);
/** 
 * @brief        日志结束运行时调用
 * @param        [in] void
 * @note         和sdk_log_init函数成对出现
 */
void sdk_log_finish(void);

void log_a(const int8_t *p_format, ...);
void log_e(const int8_t *p_format, ...);
void log_w(const int8_t *p_format, ...);
void log_i(const int8_t *p_format, ...);
void log_d(const int8_t *p_format, ...);

#endif

