/************************************************************************************
 *Description: lal_uart
 *Created on: 2022-12-10
 *Author: quliuliu
 ************************************************************************************/

#ifndef __LAL_UART_H__
#define __LAL_UART_H__

#include "data_types.h"


// 以下宏的值，需对应和sdk_uart保持一致

/* 波特率 */
#define UART_BAUD_RATE_2400                  2400
#define UART_BAUD_RATE_4800                  4800
#define UART_BAUD_RATE_9600                  9600
#define UART_BAUD_RATE_19200                 19200
#define UART_BAUD_RATE_38400                 38400
#define UART_BAUD_RATE_57600                 57600
#define UART_BAUD_RATE_115200                115200
#define UART_BAUD_RATE_230400                230400
#define UART_BAUD_RATE_460800                460800
#define UART_BAUD_RATE_921600                921600
#define UART_BAUD_RATE_2000000               2000000
#define UART_BAUD_RATE_2500000               2500000
#define UART_BAUD_RATE_3000000               3000000

/* 数据位 */
#define UART_DATA_BITS_5                     5
#define UART_DATA_BITS_6                     6
#define UART_DATA_BITS_7                     7
#define UART_DATA_BITS_8                     8
#define UART_DATA_BITS_9                     9

/* 校验位 */
#define UART_PARITY_NONE                     0
#define UART_PARITY_ODD                      1
#define UART_PARITY_EVEN                     2

/* 停止位 */
#define UART_STOP_BITS_1                     0
#define UART_STOP_BITS_2                     1
#define UART_STOP_BITS_3                     2
#define UART_STOP_BITS_4                     3

/* 硬件流控制开关 */
#define UART_HWFLOW_ON                       1
#define UART_HWFLOW_OFF                      0

/**
  * @enum lal_uart_index_t
  * @brief UART索引编号
  */
typedef enum
{
    LAL_UART1    = 0,  
    LAL_UART2,     
	LAL_UART3,
	LAL_UART4,
	LAL_UART5,
	LAL_UART6,
	LAL_UART7,
	LAL_UART_MAX,
}lal_uart_index_e;


/**
* @struct 串口属性
* @brief 配置串口各种属性
*/
typedef struct
{
    uint32_t baud_rate;      ///< 波特率             
    uint32_t data_bits;      ///< 数据位         
    uint32_t stop_bit;       ///< 停止位
    uint32_t parity;         ///< 校验位
    uint32_t flow_ctrl;      ///< 流控制
}lal_uart_conf_t;



//定义串口属性结构体
typedef struct uart_arrt
{
    int32_t baudrate;       //串口波特率---4800 9600 19200 115200	//默认为115200
    int8_t  databit;        //串口数据位---5, 6, 7, 8				//默认为8
    int8_t  *stopbit;       //串口停止位---"1", "1.5", "2"			//默认为"1"
    int8_t  parity;         //串口校验位---N(o), O(dd), E(ven)		//默认为"N"
    int8_t  flow_ctrl;      //控制流---0, 1, 2                      //默认位0
}uart_attr_t;               //{B115200, 8, "1", 'N', 0}

//定义串口操作结构体
typedef struct lal_uart
{
    int32_t fd;        //串口open()后获得的句柄
    //...
}lal_uart_t;


/**
 * @brief    打开串口
 * @param    [in] port 虚拟串口号
 * @return   0 成功
 * @return   <0 失败原因
 */
int32_t lal_uart_open(uint32_t port);

/**
 * @brief    关闭串口
 * @param    [in] port 虚拟串口号
 * @return   0 成功；<0 失败
 */
int32_t lal_uart_close (uint32_t port);

/**
 * @brief    配置串口属性
 * @param    [in] port        虚拟串口号
 * @param    [in] p_uart_conf 串口属性缓存地址
 * @return   0 成功；<0 失败
 */
int32_t lal_uart_setup(uint32_t port, lal_uart_conf_t *p_conf);

/**
 * @brief    串口通讯接收接口函数
 * @param    [in] port   虚拟串口号
 * @param    [out] p_buf 接收缓存指针
 * @param    [in] len    要接收字节数
 * @param    [in] timeout_ms 等待时长：-1 无限等待；0：不等待；其他：等待具体ms值 
 * @return   >= 0为读取到的字节数；< 0 为异常：SDK_EPERR-> 参数错误；
 */
int32_t lal_uart_read(uint32_t port, uint8_t *p_buf, int32_t len, int32_t timeout_ms);

/**
 * @brief    串口通讯发送接口函数
 * @param    [in] port   虚拟串口号
 * @param    [in] p_buf  发送缓存指针
 * @param    [in] len    发送字节数
 * @return   SDK_OK 成功；SDK_EPERR 失败
 */
int32_t lal_uart_write(uint32_t port, uint8_t *p_buf, int32_t len);


#endif /* __LAL_UART_H__ */