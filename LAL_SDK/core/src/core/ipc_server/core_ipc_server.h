
/**
 * @file         core_ipc_server.h
 * @brief        Core内部使用，主要是一些IPC通讯服务功能使用。
 * @author       APOI
 * @version      V0.0.1
 * @date         2023/01/12
 * @copyright    Copyright (c) 2023 by SofarSolar, All Rights Reserved.
 */
#ifndef __CORE_IPC_SERVER_H__
#define __CORE_IPC_SERVER_H__


#include "../../include/core.h"
#include "../../include/sdk/sdk_ipc.h"
#include <stdint.h>


#define CORE_LOG_ERROR(fmt, args...)      printf(fmt,##args)
#define CORE_LOG_INFO(fmt, args...)       printf(fmt,##args)

//#define SERVER_PORT 8000 //连接时使用的端口
#define MAX_CLINE ((MAX_PROCESS_NUM * ( MAX_PROCESS_NUM -1 ))/2) //连接队列中的最大数，保证每2个进程间至少有一个连接 

#define PROCESS_EVENT_NUM MAX_EVENT_NUM
#define NULL_ZERO    0

#define STR_OVER_MAX_CONNECT  "Over_Max_Connections\0"
#define STR_CONNECT_RES       "Connect OK\0"

 typedef struct
 { 
   int8_t   name[MAX_NAME_LEN];
   int32_t  socket_id[MAX_PROCESS_NUM];
   uint16_t socket_num;
 }ipc_server_event_t;    //ipc接口函数内部使用，不对外发布

 typedef struct
 {
   ipc_server_event_t ipc_event[ PROCESS_EVENT_NUM * MAX_PROCESS_NUM]; //总的事件数量=每人进程事件的最大值 * 业务层最大进程数 
   uint16_t event_num;
 }ipc_server_event_list_t;

#endif