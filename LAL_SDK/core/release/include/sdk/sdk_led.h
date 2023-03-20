/**
 * @file     	sdk_led.h
 * @brief    	led模块sdk接口
 * @author   	renwenjie
 * @note     	无
 * @version  	V1.0.1
 * @date     	2022/12/30
 * @copyright   Copyright(c) 2022 by SofarSolar, All Rights Reserved. 
 */

#ifndef __SDK_LED_H__
#define __SDK_LED_H__

#include "data_types.h"


#ifndef SDK_API_INTERFACE_ENABLE	// SDK接口对外声明是否启用总开关
#ifndef SDK_LED_ENABLE				// sdk_led模块是否启用开关

typedef enum{
	LED_ID_1 = 0,
	LED_ID_2 = 1,
	LED_ID_3 = 2,		
	LED_ID_NUM = 3,		//led数量，目前只3个，将来可能扩展，扩展时按需增加id，NUM对应递增		
}led_id_e;

/**
 * @brief	led闪烁配置
 * @param	[in] id			led编号（<LED_ID_NUM）
 * @param	[in] period		led闪烁周期
 * -# 最小周期为任务周期或扫描周期（建议10ms）,需是任务及扫描周期的倍数
 * @param	[in] duty		led闪烁占空比
 * -# 0-100,表示0%-100%
 * @param	[in] times		led闪烁次数
 * -# -1 持续闪烁
 * -# >=0 闪烁次数
 * @return	执行结果
 * @retval	0  成功
 * @retval	-1 失败
 */
int32_t sdk_led_flash(uint32_t id, uint32_t period, uint32_t duty, int32_t times);
 
/**
 * @brief	LED灯亮
 * @param	[in] id 	led编号
 * @return	执行结果
 * @retval	0  成功
 * @retval	-1 失败
 */
int32_t sdk_led_on(uint32_t id);

/**
 * @brief	LED灯灭
 * @param	[in]  id 	led编号
 * @return	执行结果
 * @retval	0  成功
 * @retval	-1 失败
 */
int32_t sdk_led_off(uint32_t id);


#endif
#endif
#endif
