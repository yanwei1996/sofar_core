/**
* @file		sdk_version.c
* @brief    版本管理模块
* @copyright Shouhang
* @author   yangwenjuan
* @note     2023.01.05 新拟制
* @version  V1.0   
* @date     2023/01/05
*/

#include "core.h"
#include "lal_adc.h"
#include "lal_version.h"
#include "sdk_version.h"
#include "errors.h"
#include <string.h>
#include <stdio.h>

#define log_e(...)		printf(__VA_ARGS__)	    // fixme 后面easylog可以用后，屏蔽掉  

#define SOFTWARE_VER_LEN	15  // 《软件版本号规范》定义，新产品，版本号规则：V99.255.255-PH3
#define HARDWARE_VER_LEN	7

static uint8_t sdk_software_ver[SOFTWARE_VER_LEN] = "V1.0.4";      // ARM的sdk软件版本号，后面改成读.h文件的形式？
uint8_t hardware_ver[HARDWARE_VER_LEN] = "----";	//"V1.0"

/** 
 * @brief        将adc值转换为硬件版本号(集储项目)
 * @param        [in]  adc_value adc值
 * @return       void
 */
static void hw_version_convert_cmu(int32_t adc_value)
{
	int32_t vref = 33000;	//3300mv,放大10倍
	int32_t val = 0;

	val = (adc_value * vref) >> 12;	//4096对应3.3V

	if ((val >= 0) && (val <= 4125))
	{
		strcpy(hardware_ver, "V1.0");	
	}
	else if ((val >= 4126) && (val <= 8250))
	{
		strcpy(hardware_ver, "V2.0");	
	}
	else if ((val >= 8251) && (val <= 12375))
	{
		strcpy(hardware_ver, "V3.0");	
	}
	else if ((val >= 12376) && (val <= 16500))
	{
		strcpy(hardware_ver, "V4.0");	
	}
	else if ((val >= 16501) && (val <= 20625))
	{
		strcpy(hardware_ver, "V5.0");	
	}
	else if ((val >= 20626) && (val <= 24750))
	{
		strcpy(hardware_ver, "V6.0");	
	}
	else if ((val >= 24751) && (val <= 28875))
	{
		strcpy(hardware_ver, "V7.0");	
	}
	else if ((val >= 28876) && (val <= 33000))
	{
		strcpy(hardware_ver, "V8.0");	
	}
}

/** 
 * @brief        将adc值转换为硬件版本号(leo5项目)
 * @param        [in]  adc_value adc值
 * @return       void
 */
static void hw_version_convert_leo5(int32_t adc_value)
{
	int32_t vref = 3300;	//3300mv
	int32_t val = 0;

	val = (adc_value * vref) >> 12;	//4096对应3.3V

	if ((val > 0) && (val <= 156))
	{
		strcpy(hardware_ver, "VX00");	
	}
	else if ((val > 156) && (val <= 461))
	{
		strcpy(hardware_ver, "VX01");	
	}
	else if ((val > 461) && (val <= 646))
	{
		strcpy(hardware_ver, "VX02");	
	}
	else if ((val > 646) && (val <= 1065))
	{
		strcpy(hardware_ver, "VX03");	
	}
	else if ((val > 1065) && (val <= 1356))
	{
		strcpy(hardware_ver, "VX04");	
	}
	else if ((val > 1356) && (val <= 1571))
	{
		strcpy(hardware_ver, "VX05");	
	}
	else if ((val > 1571) && (val <= 1868))
	{
		strcpy(hardware_ver, "VX06");	
	}
	else if ((val > 1868) && (val <= 2163))
	{
		strcpy(hardware_ver, "VX07");	
	}
	else if ((val > 2163) && (val <= 2367))
	{
		strcpy(hardware_ver, "VX08");	
	}
	else if ((val > 2367) && (val <= 2538))
	{
		strcpy(hardware_ver, "VX09");	
	}
	else if ((val > 2538) && (val <= 2750))
	{
		strcpy(hardware_ver, "VX10");	
	}
}


/** 
 * @brief        版本获取函数
 * @param        [in]  type 版本类型，传入参数
 * @param        [out] p_version 版本数据存在p_version所指向的地址中，传出参数
 * @param        [in] len 传入版本号字节长度，避免数据超出传入的数组大小
 * @return       [int32_t] 执行结果
 * @retval       >=  版本号的实际长度(必须小于传入的长度) 
 * @retval       < 0 失败原因
 */
int32_t sdk_version_get(version_type_e type, int8_t *p_version, uint16_t len)
{
	int32_t ret = 0;
	int32_t hw_ver_adc_num = 0;
	int32_t hw_ver_value;
	int32_t ver_len;
	
	// 入参合法性检查
	if ((type >= VERSION_TYPE_MAX) || (p_version == NULL) )
	{
		ret = ERR_PARA;
		goto __exit;
	}
	
	switch (type)
	{
		case HW_VERSION_TYPE:
		{
			hw_ver_adc_num = 0;							// fixme 需根据硬件配置一下，后续需改为从配置文件中读取
			lal_adc_read(hw_ver_adc_num, &hw_ver_value);
			#ifdef __PROJECT_LEO5__
			hw_version_convert_leo5(hw_ver_value);		// fixme 根据硬件设计拟公式————刘博、刘文强； 开机从环境变量中读机型
			#else
			hw_version_convert_cmu(hw_ver_value);
			#endif
			ver_len = strlen(hardware_ver);
			ret = ver_len;
			ver_len = (ver_len > len) ? len : ver_len;
			memcpy(p_version, hardware_ver, ver_len);			
			break;
		}
		
		case BOOT_VERSION_TYPE:
		{
			ret = lal_uboot_version_get(p_version, ver_len);	// 屈工 uboot 写 到环境变量， lal接口读出
			break;
		}	
		
		case SDK_VERSION_TYPE:
		{
			ver_len = strlen(sdk_software_ver);
			ret = ver_len;
			ver_len = (ver_len > len) ? len : ver_len;
			memcpy(p_version, sdk_software_ver, ver_len);	
			break;
		}
		
		case HAL_VERSION_TYPE:
		{
			ret =ERR_PARA;		
			break;
		}	
		case LAL_VERSION_TYPE:
		{
			ret = lal_version_get(p_version, ver_len);
			break;
		}
		default:
		{
			ret = ERR_PARA;
			break;
		}		
	}

__exit:	
	if (ret < 0)
	{
		log_e("Function[%s] Operation failed, vertiontype = %d, ret = %d!\r\n", __func__, type, ret);
	}
	return ret;
}

