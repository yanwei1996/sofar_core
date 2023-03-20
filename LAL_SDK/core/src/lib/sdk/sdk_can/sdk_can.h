/**
 * @file     	sdk_can.h
 * @brief    	can sdk接口
 * @author   	renwenjie
 * @note     	无
 * @version  	V1.0.1
 * @date     	2022/12/30
 * @copyright   Copyright(c) 2022 by SofarSolar, All Rights Reserved. 
 */
 
#ifndef __SDK_CAN_H__
#define __SDK_CAN_H__

#include <stdint.h>


#define SDK_CAN_MODE_NORMAL         0	///< CAN 普通模式
#define SDK_CAN_MODE_LISEN          1	///< CAN 监听模式
#define SDK_CAN_MODE_LOOPBACK       2	///< CAN LOOP BACK模式
#define SDK_CAN_MODE_LOOPBACK_LISEN	3	///< CAN LOOP BACK 监听模式

#define SDK_CAN_BAUD_1M   			1000UL * 1000	///< 1 MBit/sec
#define SDK_CAN_BAUD_800K 			1000UL * 800	///< 800 kBit/sec
#define SDK_CAN_BAUD_500K 			1000UL * 500 	///< 500 kBit/sec
#define SDK_CAN_BAUD_250K 			1000UL * 250	///< 250 kBit/sec
#define SDK_CAN_BAUD_125K 			1000UL * 125	///< 125 kBit/sec
#define SDK_CAN_BAUD_100K 			1000UL * 100	///< 100 kBit/sec
#define SDK_CAN_BAUD_50K  			1000UL * 50		///< 50 kBit/sec
#define SDK_CAN_BAUD_20K  			1000UL * 20 	///< 20 kBit/sec
#define SDK_CAN_BAUD_10K  			1000UL * 10   	///< 10 kBit/sec

#define SDK_CAN_MAX                 2   // can外设端口个数

/**
 * @struct hal_can_msg_t
 * @brief CAN消息
 */
// typedef struct
// {
//     uint32_t id  : 29;					///< CAN ID, 标志格式 11 位，扩展格式 29 位
//     uint32_t ide : 1;					///< 扩展帧标识位 标准帧or扩展帧
//     uint32_t rtr : 1;					///< 远程帧标识位 数据帧or遥控帧
//     uint32_t rsv : 1;					///< 保留位
//     uint32_t len : 8;					///< 消息长度
//     uint32_t priv : 8;					///< 报文发送优先级
//     int32_t  hdr : 8;					///< 硬件过滤表号
//     uint32_t reserved : 8;				///< 保留
//     uint8_t  data[8];       			///< 数据段
// }can_frame_t;   //rtt平台
typedef struct
{
    uint32_t     can_id;            //32 bit CAN_ID + EFF/RTR/ERR flags
    uint8_t      can_dlc;           //数据长度: 0 .. 8
    uint8_t      data[8];
}can_frame_t;

/**
 * @struct hal_can_conf_t
 * @brief 配置CAN各种属性
 */
typedef struct
{
    uint32_t baud;      ///< 波特率    
	uint32_t mode;		///< 模式
}can_cfg_t;


/**
 * @brief		sdk can回调函数定义 
 * @param		[in] port 虚拟CAN端口号		0:can1 1:can2
 */
typedef void(*sdk_can_irq_callback)(uint32_t port, uint32_t size);



#ifndef SDK_API_INTERFACE_ENABLE	///< SDK接口对外声明是否打开


/**
 * @brief		打开CAN功能 
 * @param		[in] port 虚拟CAN端口号
 * -# HAL_CAN1 - 0x00	
 * -# HAL_CAN2 - 0x01
 * @return		执行结果
 * @retval		0 成功
 * @retval		<0 失败 
 */
int32_t sdk_can_open(uint32_t port);


/**
 * @brief		关闭CAN功能 
 * @param		[in] port 虚拟CAN端口号
 * -# HAL_CAN1 - 0x00	
 * -# HAL_CAN2 - 0x01
 * @return		执行结果
 * @retval		0 成功
 * @retval		<0 失败 
 */
int32_t sdk_can_close(uint32_t port);


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
int32_t sdk_can_setup(uint32_t port, can_cfg_t * p_cfg);


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
 * @note		执行hal_can_open后执行才有效
 */
int32_t sdk_can_write(uint32_t port, can_frame_t *p_can_frame, uint32_t len);


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
int32_t sdk_can_read(uint32_t port, can_frame_t *p_can_frame, uint32_t len, int32_t timeout_ms);


/**
 * @brief		配置CAN中断函数  
 * @param		[in] port 虚拟CAN端口号		 0:can1 1:can2
 * @param		[in] tx_cb 发送中断回调函数  
 * @param		[in] rx_cb 接收中断回调函数     
 * @return		执行结果
 * @retval		=0 成功 
 * @retval		<0 失败
 * @note		执行hal_can_open后执行才有效。
 * @warning 		不使用的中断回调可以直接填写NULL 
 */
int32_t sdk_can_set_irq(uint32_t port, sdk_can_irq_callback tx_cb, sdk_can_irq_callback rx_cb);


/**
 * @brief		关闭CAN中断
 * @param		[in] port 虚拟CAN端口号
 * -# HAL_CAN1 - 0x00	
 * -# HAL_CAN2 - 0x01
 * @return		执行结果
 * @retval		HAL_OK(0) 成功
 * @retval		HAL_EIO(<0) 失败  
 * @note		执行hal_can_open后执行才有效。
 */
int32_t sdk_can_free_irq(uint32_t port);


#endif
#endif


