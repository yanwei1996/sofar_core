/** 
 * @file          sdk_version.h
 * @brief         各种版本号的获取文件
 * @author        qinmingsheng
 * @version       V0.0.1     初始版本
 * @date          2022/12/20 11:12:30
 * @lastAuthor    qinmingsheng
 * @copyright     Copyright (c) 2022 by SofarSolar, All Rights Reserved. 
 */
#ifndef __SDK_VERSION_H__
#define __SDK_VERSION_H__

#include "data_types.h"
#include <stdint.h>

typedef enum
{
    HW_VERSION_TYPE = 0,    //硬件版本号
    BOOT_VERSION_TYPE,      //启动文件版本号
    SDK_VERSION_TYPE,       //SDK版本号
    HAL_VERSION_TYPE,       //HAL层版本号
    LAL_VERSION_TYPE,       //LAL层版本号
    VERSION_TYPE_MAX,
}version_type_e;

/** 
 * @brief        版本获取函数
 * @param        [in]  type 版本类型，传入参数
 * @param        [out] p_version 版本数据存在p_version所指向的地址中，传出参数
 * @param        [in] len 版本号p_version缓存大小，避免数据超出传入的数组大小
 * @return       [int32_t] 执行结果
 * @retval       >=  版本号的实际长度(必须小于传入的长度) 
 * @retval       < 0 失败原因
 */
int32_t sdk_version_get(version_type_e type, int8_t *p_version, uint16_t len);


#endif