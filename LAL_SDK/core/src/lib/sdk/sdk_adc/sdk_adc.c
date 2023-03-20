/**
* @file		sdk_ai.c
* @brief    模拟量采样模块
* @copyright Shouhang
* @author   yangwenjuan
* @note     2023.01.05 新拟制
* @version  V1.0   
* @date     2023/01/05
*/

#include "lal_adc.h"
#include "sdk_adc.h"
#include<stdio.h>

#define log_e(...)		printf(__VA_ARGS__)	//fixme 

/** 
 * @brief        adc获取接口
 * @param        [in]  channel  ADC采样通道（范围0~7）
 * @return       结果
 * @retval       >0 adc转换结果（范围：0~4096）
 * @retval       -1 获取失败 
 */
int32_t sdk_adc_get(uint32_t channel)
{
	int32_t ret = 0;
	int32_t value = 0;

	if (channel >= SDK_ADC_CHANNEL_NUM)
	{
		ret = -1;
	}
	else
	{
		ret = lal_adc_read(channel, &value);
	}

	if (ret < 0)
	{
		log_e("Function[%s] Operation failed, ret = %d!\r\n", __func__, ret);
	}
	return ret;
}

