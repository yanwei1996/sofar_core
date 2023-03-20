/**
* @file		sdk_can.c
* @brief    can通信接口
* @copyright Shouhang
* @author   yangwenjuan
* @note     2023.02.20 新拟制
* @version  V1.0   
* @date     2023/02/20
*/

#include "errors.h"
#include "lal_can.h"
#include "sdk_can.h"
#include<stdio.h>

#define log_e(...)		printf(__VA_ARGS__)	//fixme 


/**
 * @brief		打开CAN功能 
 * @param		[in] port 虚拟CAN端口号
 * -# HAL_CAN1 - 0x00	
 * -# HAL_CAN2 - 0x01
 * @return		执行结果
 * @retval		0 成功
 * @retval		<0 失败 
 */
int32_t sdk_can_open(uint32_t port)
{
	int32_t ret = ERR_NONE;

	if (port >= SDK_CAN_MAX)
	{
		ret = ERR_PARA;
	}
	else
	{
		ret = lal_can_open(port);
	}

	return ret;
}

/**
 * @brief		关闭CAN功能 
 * @param		[in] port 虚拟CAN端口号
 * -# HAL_CAN1 - 0x00	
 * -# HAL_CAN2 - 0x01
 * @return		执行结果
 * @retval		0 成功
 * @retval		<0 失败 
 */
int32_t sdk_can_close(uint32_t port)
{
	int32_t ret = ERR_NONE;

	if (port >= SDK_CAN_MAX)
	{
		ret = ERR_PARA;
	}
	else
	{
		ret = lal_can_close(port);
	}

	return ret;
}

/**
 * @brief		can参数配置
 * @param		[in] port 虚拟CAN端口号
 * @param		[in] *p_cfg can参数配置结构体指针
 * -# HAL_CAN1 - 0x00	
 * -# HAL_CAN2 - 0x01
 * @return		执行结果
 * @retval		0 成功
 * @retval		<0 失败 
 * @warning		sdk_can_open后调用
 */
int32_t sdk_can_setup(uint32_t port, can_cfg_t *p_cfg)
{
	int32_t ret = ERR_NONE;

	if ((port >= SDK_CAN_MAX) || (p_cfg == NULL))
	{
		ret = ERR_PARA;
	}
	else
	{
		ret = lal_can_setup(port, (lal_can_conf_t *)p_cfg);
	}
	return ret;
}


/**
 * @brief		CAN发数据 加锁
 * @param		[in] port 虚拟CAN端口号
 * -# HAL_CAN1 - 0x00	
 * -# HAL_CAN2 - 0x01
 * @param		[in] *p_can_msg can数据帧指针  
 * @param		[in] len 帧数据长度   
 * @return		执行结果
 * @retval		>=0 发送数据长度   
 * @retval		<0 失败
 * @note		执行lal_can_open后执行才有效
 */
int32_t sdk_can_write(uint32_t port, can_frame_t *p_can_frame, uint32_t len)
{
	int32_t ret = ERR_NONE;

	if ((port >= SDK_CAN_MAX) || (p_can_frame == NULL))
	{
		ret = ERR_PARA;
	}
	else
	{
		ret = lal_can_send(port, (lal_can_msg_t *)p_can_frame, len);
	}

	return ret;
}


/**
 * @brief		CAN收数据 
 * @param		[in] port 虚拟CAN端口号 0:can1 1:can2 
 * @param		[in] *p_can_msg can数据帧指针  
 * @param		[in] len 帧数据长度 
 * @param		[in] timeout_ms 超时时间(阻塞)
 * -# -1  永久超时，直到接收数据为止
 * -# >=0 表示超时接收时间：ms
 * @return		执行结果
 * @retval		>=0 接收数据长度   
 * @retval		<0 失败    
 * @note		执行hal_can_open后执行才有效
 */
int32_t sdk_can_read(uint32_t port, can_frame_t *p_can_frame, uint32_t len, int32_t timeout_ms)
{
	int32_t ret = ERR_NONE;

	if ((port >= SDK_CAN_MAX) || (p_can_frame == NULL))
	{
		ret = ERR_PARA;
	}
	else
	{
		ret = lal_can_rev(port, (lal_can_msg_t *)p_can_frame, len, timeout_ms);
	}

	return ret;
}