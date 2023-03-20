/**
 * @file     	sdk_key.h
 * @brief    	按键功能定义
 * @details     主要包含了关于按键功能的相关函数
 * @author   	renwj
 * @note     	无
 * @version  	V1.0.1 初始版本
 * @date     	2022/12/30
 * @copyright   Copyright(c) 2022 by SofarSolar, All Rights Reserved. 
 **********************************************************************************
 * @par 修改日志:
 * <table>
 * <tr><th>Date        <th>Version  <th>Author    <th>Description
 * <tr><td>2022/2/30   <td>1.0.1    <td>renwj     <td>创建初始版本
 * </table>
 *
 **********************************************************************************
 * @par 示例:
 * @code
 * 暂无
 * @endcode
 */

#ifndef __SDK_KEY_H__
#define __SDK_KEY_H__

#include "data_types.h"

#define KEY_ID_MAX         16   ///< 按键编号最大值

/**
 * @brief 	按键的键值
 */
typedef enum 
{
    KEY_NO_PRESS = 0,           ///< 无按键按下
    KEY1_SHORT_PRESS,           ///< 按键1短按
    KEY1_LONG_PRESS,            ///< 按键1长按
    KEY2_SHORT_PRESS,           ///< 按键2短按
    KEY2_LONG_PRESS,            ///< 按键2长按
    KEY3_SHORT_PRESS,           ///< 按键3短按
    KEY3_LONG_PRESS,            ///< 按键3长按
    KEY4_SHORT_PRESS,           ///< 按键4短按
    KEY4_LONG_PRESS,            ///< 按键4长按
    KEY1_2_PRESS,               ///< 组合按键,按键1和按键2同时按下
} key_val_e;


#ifndef SDK_API_INTERFACE_ENABLE	///< SDK接口对外声明是否打开


/**
 * @brief			获取按键的信息
 * @return			执行结果
 * @retval			按键值获取
 * @warning			无
 */
key_val_e sdk_key_get(void);

#endif


#endif /*__SDK_KEY_H__*/
