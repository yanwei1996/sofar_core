/** 
 * @file          sdk_ai.h
 * @brief         adc获取接口
 * @author        qinmingsheng
 * @version       V0.0.1     初始版本
 * @copyright     Copyright (c) 2022 by SofarSolar, All Rights Reserved. 
 */
#ifndef __SDK_AI_H__
#define __SDK_AI_H__

#include <stdint.h>

#define     SDK_ADC_CHANNEL_NUM     8

/** 
 * @brief        adc获取接口
 * @param        [in]  channel  ADC采样通道（范围0~7）
 * @return       结果
 * @retval       >0 adc转换结果（范围：0~4096）
 * @retval       -1 获取失败 
 */
int32_t sdk_adc_get(uint32_t channel);


#endif
