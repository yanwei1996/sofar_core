/************************************************************************************
 *Description: lal_can
 *Created on: 2022-12-10
 *Author: quliuliu
 ************************************************************************************/

#ifndef __LAL_CAN_H__
#define __LAL_CAN_H__

#include <arpa/inet.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/error.h>
#include <linux/can/raw.h>

#include "data_types.h"


// 定义在<linux/can.h>中的基本CAN帧结构和套接字地址结构
// struct can_frame
// {
//     uint32_t     can_id;            //32 bit CAN_ID + EFF/RTR/ERR flags
//     uint8_t      can_dlc;           //数据长度: 0 .. 8
//     uint8_t      data[8];
// };

#if 0
typedef struct can_frame can_frame_t;


typedef struct lal_can
{
    int8_t  name[10];           //CAN的名称
    int32_t baudrate;           //CAN通信波特率
    int32_t sock_handle;        //CAN套接字句柄
}lal_can_t;

/****************************************************************
 *描 述: 打开CAN
 *返 回: =0为正常返回，=-1为错误返回
 *参 数: p_can为定义的can的结构体; num为can的设备序号[0, 1]; baudrate为can通信的波特率
 */
int32_t lal_can_open(lal_can_t *p_can, int32_t num, int32_t baudrate);

/****************************************************************
 *描 述: 读取CAN通信数据
 *返 回: >0为正常返回读取的数据长度，=-1为错误返回
 *参 数: p_can为定义的can的结构体; rframe为基本CAN帧结构; timeout_ms为超时时间
 */
int32_t lal_can_read(lal_can_t *p_can, can_frame_t *rframe, int32_t timeout_ms);

/****************************************************************
 *描 述: CAN通信写数据
 *返 回: =0为正常返回，=-1为错误返回
 *参 数: p_can为定义的can的结构体; rframe为基本CAN帧结构;
 */
int32_t skd_can_write(lal_can_t *p_can, can_frame_t *rframe);

/****************************************************************
 *描 述: 关闭CAN
 *返 回: =0为正常返回，=-1为错误返回
 *参 数: p_can为定义的can的结构体;
 */
int32_t lal_can_close(lal_can_t *p_can);



#else
typedef struct
{
    uint32_t     can_id;            //32 bit CAN_ID + EFF/RTR/ERR flags
    uint8_t      can_dlc;           //数据长度: 0 .. 8
    uint8_t      data[8];
}lal_can_msg_t;

typedef struct
{
    uint32_t baud;      ///< 波特率    
	uint32_t mode;		///< 模式
}lal_can_conf_t;
#define CAN_PORT_0 0x00
#define CAN_PORT_1 0x01

int32_t lal_can_open(uint32_t port);
int32_t lal_can_close(uint32_t port);
int32_t lal_can_setup(uint32_t port, lal_can_conf_t *p_conf );
int32_t lal_can_rev(uint32_t port, lal_can_msg_t *p_buff, int32_t len, int32_t timeout_ms);
int32_t lal_can_send(uint32_t port, lal_can_msg_t *p_buff, int32_t len);
int32_t lal_can_flush(uint32_t port);

#endif


#endif /* __LAL_CAN_H__ */