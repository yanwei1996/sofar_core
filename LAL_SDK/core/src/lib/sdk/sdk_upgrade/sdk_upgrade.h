/**
 * @file         sdk_upgrade.h
 * @brief        升级接口
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
#ifndef __SDK_UPGRADE_H__
#define __SDK_UPGRADE_H__

#include "data_types.h"


typedef enum
{
   UT_APP      = 0x00,   //app
   UT_CORE     = 0x01,   //core
   UT_KERNEL   = 0x02,   //kernel
   UT_ROOTFS   = 0x03,   //system
   UT_SAFETY   = 0x04,   //safety
   UT_PACK     = 0x80,   //pack
 }updata_type_e;

/******************** sdk_upgrade Api ***********************/

/**
* @brief		开始升级
* @param		[in] flag 升级标志
* -# UT_APP - 0x00  
* -# UT_CORE - 0x01 
* @return		执行结果
* @retval		0 成功
* @retval		<0 失败原因
*/
int32_t sdk_start_upgrade_form_package(uint8_t *p_dir, uint8_t flag);


/**
* @brief		开始升级
* @param		[in] flag 升级标志
* -# UT_APP - 0x00  
* -# UT_CORE - 0x01 
* @return		执行结果
* @retval		0 成功
* @retval		<0 失败原因
*/
int32_t sdk_start_upgrade(uint8_t *dir,uint8_t flag);

/**
* @brief		升级进度获取
* @param		[in] type 升级设备类型
* @return		执行结果
* @retval		0 - 100 升级进度条
* @retval		<0 失败原因  
* @pre			执行sdk_start_upgrade后执行才有效。
*/
int32_t sdk_upgrade_progress_get(uint8_t flag);

/**
* @brief		升级状态获取
* @param		[in] type 升级设备类型
* @return		执行结果
* @retval		0 升级成功
* @retval		<0 升级失败 
*/
int32_t sdk_upgrade_status_get(uint8_t flag);



#endif
