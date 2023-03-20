/** 
 * @file          sdk_ipc.h
 * @brief         进程间通讯接口函数说明
 * @author        qinmingsheng
 * @version       V0.0.1     初始版本
 * @date          2022/12/20 11:12:30
 * @lastAuthor    Please set LastEditors
 * @copyright     Copyright (c) 2022 by SofarSolar, All Rights Reserved. 
 **********************************************************************************
 * @par 修改日志:
 * <table>
 * <tr><th>Date      <th>Version  <th>Author    <th>Description 
 * <tr><td>2022/12/20  <td>0.0.1  <td>Apoi     <td>创建初始版本  
 * <tr><td>2023/03/03  <td>0.0.2  <td>Apoi     <td>修改一些注释,修函数名中_msg为ipc 
 * </table>
 **********************************************************************************
 * @par 示例:
 * @code
    int32_t ret = 0;
    int8_t buf[MAX_DATA_LEN] = {0};
    int32_t event_id[50]= {0};

    memset(buf,0,MAX_DATA_LEN);
    sprintf(buf,"event_name_%02d",i);   //注册事件名字
    ret = sdk_msg_regist(buf);          //注册事件
    if( ret > 0)
    {
        LOG_INFO("Event[%d]: %s regist success\r\n",i,buf);
    }
    else
    {
        LOG_INFO("Event[%d]: %s regist fail\r\n",i,buf);
    }  

    for ( i = 0; i < (MAX_EVENT_NUM + 2); i++)
    {
        memset(buf,0,MAX_DATA_LEN);
        sprintf(buf,"event_name_%02d",i);
        event_id[i] = sdk_msg_connect(buf);
        CORE_LOG_ERROR("connect_id[%d] ---------------\n", event_id[i]);
        if( event_id[i] > 0)
        {
            sdk_msg_send(event_id[i] , &buf, strlen(buf));  //连接成功后才能发送数据
        }
        else
        {
            CORE_LOG_ERROR("event[%d]%s connect fail\n", i,buf);
        }
        
        sleep(1);
    }

    ret = sdk_msg_receive(event_id[0], &buf, &buf_len, 30); //接收event_id[0]的数据
    if (ret > 0)
    {
        LOG_INFO("Get len = %d data: %s\r\n",buf_len,buf);
    }
    else
    if ( -1 == ret)
    {
        LOG_INFO("Connect  = %d data: %s\r\n",buf_len,buf);
    }
 * @endcode
 */
#ifndef __SDK_IPC_H__
#define __SDK_IPC_H__

#include "data_types.h"

#define MAX_NAME_LEN    32    //最后一个字节为空，name实际长度为31个字节。
#define MAX_EVENT_NUM   32    //每个进程的事件总数。
#define MAX_PROCESS_NUM 16    //业务层进程最大值
#define MAX_DATA_LEN    4096

/** 
 * @brief        注册事件服务到进程通信服务中
 * @param        [in] p_name ，所有注册的事件名称（长度越小越好，建议不要超过31个字节）
 * @return       [int32_t]执行结果
 * @retval		  >0 成功,值表示返回的ID标识符
 * @retval		  <0 失败，值对应错误代码 -1:未知原因 -2：达到最大事件数 -3：名字长度非法
 * @note         使用服务前必须调用此函数进行注册
 */
int32_t sdk_ipc_regist(int8_t *p_name);

/** 
 * @brief        服务连接函数，连接其它进程或线程已经注册成功的事件。
 * @param        [in] p_name ，注册时的事件名称
 * @return       [int32_t] 执行结果
 * @retval       >0 成功，值表示返回的obj_id,通过此ID可以进行数据的收发。
 * @retval       <0 失败，值对应错误代码。-1：此事件未注册或未成功注册
 * @note         只有本进程或其它进程成功调用sdk_msg_regist(int8_t *p_name)后，此函数才有可能正确返回
 */
int32_t sdk_ipc_connect(int8_t *p_name);

/** 
 * @brief        数据发送函数，数据发送到指定的obj_id
 * @param        [in] obj_id 函数sdk_msg_connect(int8_t *p_name)成功返回值，
 * @param        [in] p_msg_t 可以是任何类型结构体。
 * @param        [in] msg_size 入参p_msg_t的大小，单位字节。
 * @return       [int32_t] 执行结果
 * @retval       >0成功，实际发送数据长度
 * @retval       <0失败，-1：obj_id无效或不存在、msg_size为0
 * @note         向指定的obj_id发送数据
 */
int32_t sdk_ipc_send(int32_t obj_id, const void *p_msg_t, const uint32_t msg_size); //p_msg_t可以是任何类型结构体。

/** 
 * @brief        接收其它进程或线程序发送到obj_id的数据
 * @param        [in] obj_id 传入参数，指定的数据来源，函数sdk_msg_connect(int8_t *p_name)成功返回值
 * @param        [out] p_msg_t 传出参数，数据存放在p_msg_t指向的地址中。
 * @param        [out] msg_size 传出参数，数据长度
 * @param        [in] wait_time 传入参数，需要等待时间，单位：ms，大于零实际等时间，0表示无等等立即返回，小于0，一直等待。
 * @return       [int32_t] 执行结果
 * @retval       >0 成功
 * @retval       =0 指定的wait_time 时间到。
 * @retval       <0 失败，值对应的是错误代码 -1:传入参数为空 -2:连接已断开
 * @note         接收指定的obj_id数据，
 */
int32_t sdk_ipc_receive(int32_t obj_id, void *p_msg_t, uint32_t *msg_size, const int32_t wait_time); 


#endif /*__SDK_IPC_H__*/
