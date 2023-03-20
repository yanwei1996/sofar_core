/**
 * @file         sdk_net_cellular.h
 * @brief        蜂窝网接口
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
 
#ifndef __SDK_NET_CELLULAR_H__
#define __SDK_NET_CELLULAR_H__


#include "data_types.h"

#define MAX_NAME_LEN 20

 typedef struct 
{
	uint8_t apn_name[MAX_NAME_LEN];
	uint8_t user_name[MAX_NAME_LEN];
	uint8_t user_password[MAX_NAME_LEN];
}cellular_net_t ;



/**
 * @brief    设置模块的蜂窝工作信息
 * @param    p_apn 值。
 * @return   int32_t        >= 0 成功   <0失败 不同值对应不 同的错误代码。
 */
int32_t sdk_cellular_net_apn_set( cellular_net_t *p_apn);

/**
 * @brief    开启蜂窝工作
 * @return   int32_t        >= 0 成功   <0失败 不同值对应不 同的错误代码。
 */
int32_t sdk_cellular_net_start(void);

/**
 * @brief    获取蜂窝的信号强度
 * @param    rssi 指针
 * @return   int32_t        >= 0 成功   <0失败 不同值对应不 同的错误代码。
 */
int32_t sdk_cellular_net_rssi_get(int16_t *rssi);

/**
 * @brief    获取sim卡的ccid
 * @param    p_iccid  
 * @param    要获取的字符段长度
 * @return   int32_t        >= 0 成功   <0失败 不同值对应不 同的错误代码。
 */
int32_t sdk_cellular_sim_ccid_get(int8_t *p_iccid, int8_t len);

/**
 * @brief    获取sim卡的状态
 * @return   int32_t        >= 0 成功   <0失败 
 */
int32_t sdk_cellular_sim_status_get(void);

/**
 * @brief    获取模块的imei
 * @param    p_imei 
 * @param    要获取的字符段长度
 * @return   int32_t        >= 0 成功   <0失败 
 */
int32_t sdk_cellular_imei_get(int8_t *p_imei, uint32_t len);
/**
 * @brief    获取模块联网状态
 * @return   int32_t        >= 0 成功 不同值对应不同状态   <0未联网  联网失败
 */
int32_t sdk_cellular_net_status_get(void);

/**
 * @brief    获取模块联网erro原因
 * @return   int32_t        -1 sim not exit  -2 ppp fail -3 other erro
 */
int32_t sdk_cellular_net_error_code_get(void);
#endif









