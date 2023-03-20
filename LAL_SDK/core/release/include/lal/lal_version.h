/** 
 * @file          lal_version.h
 * @brief         lal、uboot版本号的获取接口
 * @author        yangwenjuan
 * @copyright     Copyright (c) 2022 by SofarSolar, All Rights Reserved. 
 */
#ifndef __LAL_VERSION_H__
#define __LAL_VERSION_H__

#include <stdint.h>

/** 
 * @brief        lal软件版本获取函数
 * @param        [out] p_version 版本数据存在p_version所指向的地址中，传出参数
 * @param        [in] len 传入版本号字节长度，避免数据超出传入的数组大小
 * @return       [int32_t] 执行结果
 * @retval       >=  版本号的实际长度(必须小于传入的长度) 
 * @retval       < 0 失败原因
 */
int32_t lal_version_get(int8_t *p_version, uint16_t len);

/** 
 * @brief        uboot版本获取函数
 * @param        [out] p_version 版本数据存在p_version所指向的地址中，传出参数
 * @param        [in] len 传入版本号字节长度，避免数据超出传入的数组大小
 * @return       [int32_t] 执行结果
 * @retval       >=  版本号的实际长度(必须小于传入的长度) 
 * @retval       < 0 失败原因
 */
int32_t lal_uboot_version_get(int8_t *p_version, uint16_t len);


#endif
