/************************************************************************************
 *Description: lal_adc
 *Created on: 2022-12-10
 *Author: quliuliu
 ************************************************************************************/

#ifndef __LAL_ADC_H__
#define __LAL_ADC_H__

#include "data_types.h"

/****************************************************************
 *描 述: 获取ADC的值
 *返 回: >0为ADC的值，=-1为错误返回
 *参 数: num为DRMS的序号,由DRMS宏列表提供
 */
int32_t lal_adc_read(uint32_t num, int32_t *p_value);


//DRMS
#define DRM0   "/sys/devices/platform/soc/2100000.aips-bus/2198000.adc/iio:device0/in_voltage9_raw"   
#define DRM1   "/sys/devices/platform/soc/2100000.aips-bus/2198000.adc/iio:device0/in_voltage3_raw" 
#define DRM2   "/sys/devices/platform/soc/2100000.aips-bus/2198000.adc/iio:device0/in_voltage4_raw" 
#define DRM3   "/sys/devices/platform/soc/2100000.aips-bus/2198000.adc/iio:device0/in_voltage1_raw" 
#define DRM4   "/sys/devices/platform/soc/2100000.aips-bus/2198000.adc/iio:device0/in_voltage2_raw" 


#endif /* __LAL_ADC_H__ */