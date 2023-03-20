/**
 * @file     	sdk_buzzer.h
 * @brief    	蜂鸣器模块sdk接口
 * @author   	renwenjie
 * @note     	无
 * @version  	V1.0.1
 * @date     	2022/12/30
 * @copyright   Copyright(c) 2022 by SofarSolar, All Rights Reserved. 
 */
 

#ifndef __SDK_BUZZER_H__
#define __SDK_BUZZER_H__

#include "data_types.h"

#ifndef SDK_API_INTERFACE_ENABLE	///< SDK接口对外声明是否打开


typedef enum{
	BUZZER_ID_SYS = 0,
	BUZZER_ID_NUM = 1,		//蜂鸣器数量，目前单板上只1个，将来可能扩展，扩展时按需增加id，NUM对应递增		
}buzzer_id_e;

/**
 * @brief	蜂鸣器滴答
 * @param	[in] id	蜂鸣器编号
 * @param	[in] period		蜂鸣器鸣响周期
 * -# 最小周期为任务周期或扫描周期（建议10ms）,需是任务及扫描周期的倍数
 * @param	[in] duty		蜂鸣器鸣响占空比
 * -# 0-100,表示0%-100%
 * @param	[in] times		蜂鸣器鸣响次数
 * -# -1表示持续鸣响
 * -# >=0表示实际鸣响次数
 * @return	执行结果
 * @retval	0  设置成功
 * @retval	-1 设置失败
 */
int32_t sdk_buzzer_tick(uint32_t id, uint32_t period, uint32_t duty, int32_t times);

/**
 * @brief	蜂鸣器响
 * @param	[in] id	蜂鸣器标号
 * @return	执行结果
 * @retval	0  设置成功
 * @retval	-1 设置失败
 * @warning	无警告
 */
int32_t sdk_buzzer_on(uint32_t id);

/**
 * @brief	蜂鸣器灭
 * @param	[in] id	蜂鸣器标号
 * @return	执行结果
 * @retval	0  设置成功
 * @retval	-1 设置失败
 * @warning	无警告
 */
int32_t sdk_buzzer_off(uint32_t id);




#endif
#endif
