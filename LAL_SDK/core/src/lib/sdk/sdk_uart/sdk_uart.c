/**
* @file		sdk_uart.c
* @brief    uart通信接口
* @copyright Shouhang
* @author   yangwenjuan
* @note     2023.02.17 新拟制
* @version  V1.0   
* @date     2023/02/17
*/

#include "errors.h"
#include "lal_uart.h"
#include "sdk_uart.h"
#include<stdio.h>

#define log_e(...)		printf(__VA_ARGS__)	//fixme 


/**
 * @brief    打开串口
 * @param    [in] port 虚拟串口号
 * @return   SDK_OK 成功
 * @return   SDK_EPERR 失败
 */
int32_t sdk_uart_open(uint32_t port)
{
	int32_t ret = ERR_NONE;

	if (port >= SDK_UART_MAX)
	{
		ret = ERR_PARA;
	}
	else
	{
		ret = lal_uart_open(port);
	}

	return ret;
}

/**
 * @brief    关闭串口
 * @param    [in] port 虚拟串口号
 * @return   SDK_OK 成功；SDK_EPERR 失败
 */
int32_t sdk_uart_close(uint32_t port)
{
	int32_t ret = ERR_NONE;

	if (port >= SDK_UART_MAX)
	{
		ret = ERR_PARA;
	}
	else
	{
		ret = lal_uart_close(port);
	}

	return ret;
}

/**
 * @brief    配置串口属性
 * @param    [in] port        虚拟串口号
 * @param    [in] p_uart_conf 串口属性缓存地址
 * @return   SDK_OK 成功；SDK_EPERR 失败
 */
int32_t sdk_uart_setup(uint32_t port, sdk_uart_conf_t *p_uart_conf)
{
	int32_t ret = ERR_NONE;

	if ((port >= SDK_UART_MAX) || (p_uart_conf == NULL))
	{
		ret = ERR_PARA;
	}
	else
	{
		ret = lal_uart_setup(port, (lal_uart_conf_t *)p_uart_conf);
	}

	return ret;
}

/**
 * @brief    串口通讯发送接口函数
 * @param    [in] port   虚拟串口号
 * @param    [in] p_buf  发送缓存指针
 * @param    [in] len    发送字节数
 * @return   SDK_OK 成功；SDK_EPERR 失败
 */
int32_t sdk_uart_write(uint32_t port, uint8_t *p_buf, uint32_t len)
{
	int32_t ret = ERR_NONE;

	if ((port >= SDK_UART_MAX) || (p_buf == NULL))
	{
		ret = ERR_PARA;
	}
	else
	{
		ret = lal_uart_write(port, p_buf, len);
	}

	return ret;
}

/**
 * @brief    串口通讯接收接口函数
 * @param    [in] port   虚拟串口号
 * @param    [out] p_buf 接收缓存指针
 * @param    [in] len    要接收字节数
 * @param    [in] timeout_ms 等待时长：-1 无限等待；0：不等待；其他：等待具体ms值 
 * @return   >= 0为读取到的字节数；< 0 为异常：SDK_EPERR-> 参数错误；
 */
int32_t sdk_uart_read(uint32_t port, uint8_t *p_buf, uint32_t len, int32_t timeout_ms)
{
	int32_t ret = ERR_NONE;

	if ((port >= SDK_UART_MAX) || (p_buf == NULL))
	{
		ret = ERR_PARA;
	}
	else
	{
		ret = lal_uart_read(port, p_buf, len, timeout_ms);
	}

	return ret;
}