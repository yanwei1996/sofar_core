/**
* @file		lal_version.c
* @brief    lal层版本管理模块
* @copyright Shouhang
* @author   yangwenjuan
* @note     2023.02.16 新拟制
* @version  V1.0   
* @date     2023/02/16
*/

#include "lal_version.h"
#include "errors.h"
#include<string.h>

#define log_e(...)		printf(__VA_ARGS__)	    //fixme 后面easylog可以用后，屏蔽掉  

#define SOFTWARE_VER_LEN	15  // 《软件版本号规范》定义，新产品，版本号规则：V99.255.255-PH3

static uint8_t lal_software_ver[SOFTWARE_VER_LEN] = "V1.0.1";
static uint8_t lal_uboot_software_ver[SOFTWARE_VER_LEN] = "V1.1.1";

/** 
 * @brief        lal软件版本获取函数
 * @param        [out] p_version 版本数据存在p_version所指向的地址中，传出参数
 * @param        [in] len 传入版本号字节长度，避免数据超出传入的数组大小
 * @return       [int32_t] 执行结果
 * @retval       >=  版本号的实际长度(必须小于传入的长度) 
 * @retval       < 0 失败原因
 */
int32_t lal_version_get(int8_t *p_version, uint16_t len)
{
	int32_t ret = ERR_NONE;
	int32_t ver_len;

	if (p_version == NULL)
	{
		ret = ERR_PARA;
	}
	else
	{
		ver_len = strlen(lal_software_ver);
		ret = ver_len;
		ver_len = (ver_len > len) ? len : ver_len;
		memcpy(p_version, lal_software_ver, ret);
	}

	return ret;	
}

/** 
 * @brief        uboot版本获取函数
 * @param        [out] p_version 版本数据存在p_version所指向的地址中，传出参数
 * @param        [in] len 传入版本号字节长度，避免数据超出传入的数组大小
 * @return       [int32_t] 执行结果
 * @retval       >=  版本号的实际长度(必须小于传入的长度) 
 * @retval       < 0 失败原因
 */
int32_t lal_uboot_version_get(int8_t *p_version, uint16_t len)
{
	int32_t ret = ERR_NONE;
	int32_t ver_len;

	// 实现方式：uboot启动后将版本号写入环境变量，lal接口从环境变量读取uboot版本
	if (p_version == NULL)
	{
		ret = ERR_PARA;
	}
	else
	{
		ver_len = strlen(lal_uboot_software_ver);	// fixme 需更改,改为uboot的版本
		ret = ver_len;
		ver_len = (ver_len > len) ? len : ver_len;
		memcpy(p_version, lal_uboot_software_ver, ret);	
	}

	return ret;
}