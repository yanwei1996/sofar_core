/**
 * @file         sdk_store.h
 * @brief        获取指定存储设备的路径接口
 * @author       yanwei
 * @version      V0.0.1
 * @date         2023/03/06
 * @copyright    Copyright (c) 2023 by SofarSolar, All Rights Reserved.
 **********************************************************************************
 * @par 修改日志:
 * <table>
 * <tr><th>Date        <th>Version  <th>Author    <th>Description
 * <tr><td>2023/02/16  <td>0.0.1    <td>yanwei     <td>创建初始版本
 * </table>
 *
 **********************************************************************************
 * @par 示例:
 * @code
 * 暂无
 * @endcode
 */
 
#ifndef __SDK_STORE_H__
#define __SDK_STORE_H__
#include "data_types.h"

typedef enum
{
   USB_FLSH = 0,    //U盘
   TF_CARD,         //TF卡
 }store_device_type_e;

/**
 * @brief    获取指定存储设备的路径
 * @param    type 存储设备
 * @param    p_path 路径
 * @param    len       长度
 * @return   int32_t        >= 0 成功   <0失败 不同值对应不 同的错误代码。
 * @retval 0  获取成功，设备路径为p_path
 * @retval -1 获取失败，设备不存在
 * @retval -2 入参错误
 */
int32_t  sdk_store_path_get(store_device_type_e type,int8_t *p_path, uint32_t len);



/**
 * @brief    获取指定存储设备是否存在
 * @param    type 存储设备
 * @return   int32_t        
 * @return   >= 0 存在  
 * @return   <0 不存在
 */
int32_t  sdk_store_is_exist(store_device_type_e type);

#endif








