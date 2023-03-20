/** 
 * @file          sdk_ipc.c
 * @brief         进程间通讯接口函数功能实现
 * @author        qinmingsheng
 * @version       V0.0.1     初始版本
 * @date          2023/1/11 11:12:30
 * @copyright     Copyright (c) 2022 by SofarSolar, All Rights Reserved. 
 */

#include "sdk_ipc.h"
#include "../../../include/core.h"

#include <stdio.h>
#include <stdint.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>   //select
#include <errno.h>

#define MSG_HEAD_LEN    4  //2 * sizeof(uint16_t);   //消息头有2个uint16_T 类型数，所以乘以2
#define CORE_LOG_ERROR(fmt, args...)      printf(fmt,##args)
#define CORE_LOG_INFO(fmt, args...)       printf(fmt,##args)

#define LOG_ERROR(fmt, args...)       printf(fmt,##args)
#define LOG_INFO(fmt, args...)       printf(fmt,##args)

//#define CLIENT_PORT 8000
#define SYSTEM_OK   0

 typedef struct
 {
    int8_t   name[MAX_NAME_LEN];
    int32_t  socket_id;
 }ipc_event_t;    //ipc接口函数内部使用，不对外发布

typedef struct
 {
    ipc_event_t ipc_event[MAX_EVENT_NUM]; //
    uint16_t event_num;
 }ipc_event_list_t;

static ipc_event_list_t ipc_events;//每个进程最多可以注册不同的事件。

/**
 * @brief    	检查事件名称是否已经存在
 * @param    	[in] p_name 所有注册的事件名称
 * @return   	执行结果
 * @retval		>0 成功,值表示返回的ID标识符
 * @retval		<0 失败，值对应错误代码 -1:事件不存在
 */
static int32_t event_is_exist(int8_t *p_name)
{
    
    int32_t  ret = -1 ;
    uint16_t loop = 0;
    uint16_t num = ipc_events.event_num;
    int32_t comp = -1;
    
    for ( loop = 0; loop < num; loop++ )
    {
        comp = strcmp((const char*)p_name,(const char*)&ipc_events.ipc_event[loop].name);

 //       CORE_LOG_INFO("Received :%s data:%s loop%d %d comp=%d\n",(const char*)p_name,(const char*)&ipc_events.ipc_event[loop].name,loop,ipc_events.event_num,comp);
        if (comp == 0)
        {
            return (ipc_events.ipc_event[loop].socket_id);    //返回事件对应的Socket id
        }
        else
        {
            continue;
        }
    }
    return (ret);    //事件不存在，返加 -1
}

/** 
 * @brief        注册事件服务到进程通信服务中
 * @param        [in] p_name ，所有注册的事件名称（长度越小越好，建议不要超过31个字节）
 * @return       [int32_t]执行结果
 * @retval		   >0 成功,值表示返回的ID标识符
 * @retval		   <0 失败，值对应错误代码 -1:未知原因 -2：达到最大事件数 -3：名字长度非法
 * @note         使用服务前必须调用此函数进行注册
 */
int32_t sdk_ipc_regist(int8_t *p_name)
{
    int32_t  sockfd = 0;
    int32_t  ret = -1;

    int8_t   *p_rev_name = p_name;
    int8_t   *p_ret = NULL;
    int8_t   event_num = 0;
    uint16_t rec_len = 0;
    int8_t   recvline[MAX_DATA_LEN] = {0};
    int8_t   buffer[MAX_NAME_LEN] = {0};

    struct sockaddr_in    servaddr;
    ipc_msg_t msg_data;
    ipc_msg_t *p_msg_data;

    if ( p_name == NULL )
    {
        return(-1);
    }
    
    event_num = ipc_events.event_num;
    if ( event_num >= MAX_EVENT_NUM )
    {
        CORE_LOG_ERROR("Alread arrived MAX_EVENT_NUM:  %02d\n", MAX_EVENT_NUM);

        return (-2);       //事件数超过最大值
    }

    rec_len = strlen((const char *)p_rev_name);    //取名字的长度
    if ( rec_len >= MAX_NAME_LEN )    //长度合法性判断
    {
//        memcpy(&buffer,p_rev_name,MAX_NAME_LEN-1);
        return (-3);      //名字长度超过MAX_NAME_LEN - 1
    }
    else
    {
        memcpy(&buffer,p_rev_name,rec_len);
    }

    ret = event_is_exist(&buffer[0]);
    if ( ret > SYSTEM_OK )
    {
        CORE_LOG_ERROR("event: %s already existed\n",(const char *)p_rev_name);
        return ret;
    }else
    {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if ( sockfd < 0)
        {
            CORE_LOG_ERROR("create socket error: %s(errno: %d)\n", strerror(errno),errno);
        }
        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(SERVER_PORT);
        servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        ret = connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
        if ( ret < 0 )
        {
            CORE_LOG_ERROR("connect error: %s(errno: %d)\n",strerror(errno),errno);
            return (ret);
        }      

        msg_data.type = EVENT_REGIST;
        memcpy(msg_data.data ,&buffer,strlen((const char*)&buffer));
        msg_data.data_len = strlen((const char*)&buffer);

        ret = send(sockfd, (int8_t *)&msg_data, msg_data.data_len + sizeof(uint32_t), 0);
        if ( ret < 0)
        {
            CORE_LOG_ERROR("send msg error: %s(errno: %d)\n", strerror(errno), errno);
        }
        else
        {   
            rec_len = recv(sockfd, recvline, MAX_DATA_LEN+MSG_HEAD_LEN,0);
            if( rec_len == -1 ) 
            {
                CORE_LOG_ERROR("recv error");
            }
            else
            {
                p_msg_data = (ipc_msg_t *)recvline;

                CORE_LOG_INFO("Received :rec_len%d data_len=%d data:%s \n",rec_len,p_msg_data->data_len, p_msg_data->data);
                
                p_ret = (int8_t *)strstr((const char *)p_msg_data->data,(const char *)p_rev_name);//查看返回值是否有注册名字，有说明成功
                if (p_ret != NULL)
                {
                    event_num = ipc_events.event_num;

                    memcpy(ipc_events.ipc_event[event_num].name ,p_rev_name,MAX_NAME_LEN);
                    ipc_events.ipc_event[event_num].socket_id = sockfd;
                    ipc_events.event_num++;
                    return(sockfd);
                    
                } 
                else
                {
                    close(sockfd);  //注册失败，关闭当前连接
                }              
                
            }

        }
        return (ret);
    }
    
   return (ret);
}

/** 
 * @brief        服务连接函数，连接其它进程或线程已经注册成功的事件。
 * @param        [in] p_name ，注册时的事件名称
 * @return       [int32_t] 执行结果
 * @retval       >0 成功，值表示返回的obj_id,通过此ID可以进行数据的收发。
 * @retval       <0 失败，值对应错误代码。-1：此事件未注册或未成功注册
 * @note         只有本进程或其它进程成功调用sdk_msg_regist(int8_t *p_name)后，此函数才有可能正确返回
 */
int32_t sdk_ipc_connect(int8_t *p_name)
{
    int32_t ret = -1;
    int32_t check_id = 0;

    int8_t *p_ret = NULL;
    int8_t revbuf[MAX_DATA_LEN] = {0}; //赋值初始为0
    int8_t *p_rev_name = p_name;
    int8_t   buffer[MAX_DATA_LEN] ="check_connect";
    fd_set fdsr; //文件描述符集的定义


    struct timeval tv = {0};
      
    ipc_msg_t msg_data;
    ipc_msg_t *p_msg_data = NULL;

    ret = event_is_exist(p_rev_name);
    if ( ret > SYSTEM_OK )
    {
        check_id = ret;
        msg_data.type = EVENT_CONNECT;
        memcpy(msg_data.data ,&buffer,strlen((const char*)&buffer));
        msg_data.data_len = strlen((const char*)&buffer);
        ret = send(check_id, (int8_t *)&msg_data, msg_data.data_len + sizeof(uint32_t), 0); //发一次数据，来判断此连接是否还存在
        CORE_LOG_ERROR("sdk_event_is_exist:ret= %d(check_id%d)\n", ret, check_id);
        if ( ret <= 0)
        {
            CORE_LOG_ERROR("send msg error: %s(errno: %d)\n", strerror(errno), errno);
        }
        else
        {
            tv.tv_sec  = 1;
            tv.tv_usec = 0; 

            //这两部是非常重要的，不可缺少的，缺少了有可能导致状态不会更新
            FD_ZERO(&fdsr);
            FD_SET(check_id,&fdsr); 

            //如果文件描述符中有连接请求 会做相应的处理，实现I/O的复用 多用户的连接通讯
        //   CORE_LOG_INFO("tv.tv_sec=%ld tv_tv_usec=%ld \n",tv.tv_sec,tv.tv_usec);
            ret = select(check_id +1,&fdsr,NULL,NULL,&tv);
            if ( ret < 0 ) //没有找到有效的连接 失败
            {
        //        CORE_LOG_ERROR("select error!\n");
                return (-2);    //连接已断开
            }
            else if ( ret == 0 )// 指定的时间到，
            {
        //        CORE_LOG_INFO("Read msg timeout \n");
//                return (0);
                return (-1);    //连接失败
            }
            if ( FD_ISSET( check_id,&fdsr) )
            {
                ret = recv(check_id,revbuf,sizeof(revbuf),0);   
                if ( ret <= 0 ) //服务端连接关闭，清除文件描述符集中的相应的位
                {
                    CORE_LOG_ERROR("Connect [%d] close\n",check_id);
                    close(check_id);
                    FD_CLR(check_id,&fdsr);               
                }
                else        //否则有相应的数据发送过来 ，进行相应的处理
                {
                    if ( ret < MAX_DATA_LEN )
                    {  
                        memcpy(&msg_data,revbuf,ret);
                        p_msg_data = (ipc_msg_t*)revbuf;
                        CORE_LOG_INFO("Get data len=%d data=%s\n",p_msg_data->data_len,p_msg_data->data); //发布时可以通过打印级别来控制此语句是否打印。                                             
                        p_ret = (int8_t *)strstr((const char *)p_msg_data->data,(const char *)"Connect OK");//查看返回值是否有注册名字，有说明成功
                        if (p_ret != NULL)
                        {
                            ret = check_id;
                        }
                        
                    }
                       
                }
            }
            
        }
       
    }
    return(ret);
}

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
int32_t sdk_ipc_send(int32_t obj_id, const void *p_msg_t, uint32_t msg_size) 
{
    ipc_msg_t msg_send;
    int32_t ret = -1;

    if ((obj_id == 0)||(p_msg_t == NULL)||(msg_size == 0))
    {
        return (ret);
    }
    
    msg_send.type = EVENT_SEND_DATA;
    if(msg_size > MAX_DATA_LEN)
    {
        memcpy(msg_send.data ,p_msg_t,MAX_DATA_LEN);
        msg_send.data_len = MAX_DATA_LEN;
    }
    else
    {
        memcpy(msg_send.data ,p_msg_t,msg_size);
        msg_send.data_len = msg_size;
    }
    

    ret = send(obj_id, (int8_t *)&msg_send, msg_send.data_len + MSG_HEAD_LEN, 0);
    if ( ret < 0 )
    {
        CORE_LOG_ERROR("send msg error: %s(errno: %d)\n", strerror(errno), errno);
    }
    else
    {
         if ( ret > 0 )
        {
            ret = msg_send.data_len; //发送数据实际的有效字节数
        }
    }
    return (ret);
}

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
 * @date         2022/12/20 09:34:20
 * @lastEditors  APOI
 */
int32_t sdk_ipc_receive(int32_t obj_id, void *p_msg_t, uint32_t *msg_size, const int32_t wait_time)
{
    fd_set fdsr; //文件描述符集的定义
    int32_t sock_fd = obj_id;
    int32_t ret = 0;
    int8_t buf[MAX_DATA_LEN + MSG_HEAD_LEN] = {0}; //赋值初始为0
    struct timeval tv;
    struct timeval *p_tv = &tv; //定义一个指针便于把NULL赋值给 Select 最后一个参数

    if ( (p_msg_t == NULL)||(msg_size == NULL)||(obj_id <= 0) )
    {
        return (-1);
    }

    if ( wait_time >= 0 )
    {
        p_tv->tv_sec = wait_time/1000;
        p_tv->tv_usec = (wait_time%1000) * 1000;   //小于1000ms的部分转为微秒
    }
    else
    {
        p_tv = NULL;
    }
    
    //这两部是非常重要的，不可缺少的，缺少了有可能导致状态不会更新
    FD_ZERO(&fdsr);
    FD_SET(sock_fd,&fdsr); 

    //如果文件描述符中有连接请求 会做相应的处理，实现I/O的复用 多用户的连接通讯
 //   CORE_LOG_INFO("tv.tv_sec=%ld tv_tv_usec=%ld \n",tv.tv_sec,tv.tv_usec);
    ret = select(sock_fd +1,&fdsr,NULL,NULL,p_tv);
    if ( ret < 0 ) //没有找到有效的连接 失败
    {
//        CORE_LOG_ERROR("select error!\n");
        return (-2);    //连接已断开
    }
    else if ( ret == 0 )// 指定的时间到，
    {
//        CORE_LOG_INFO("Read msg timeout \n");
        return (0);
    }
    if ( FD_ISSET( sock_fd,&fdsr) )
    {
        ret = recv(sock_fd,buf,sizeof(buf),0);   
        if ( ret <=0 ) //服务端连接关闭，清除文件描述符集中的相应的位
        {
          CORE_LOG_ERROR("Connect [%d] close\n",sock_fd);
          close(sock_fd);
          FD_CLR(sock_fd,&fdsr);
          sock_fd = 0;
          
        }
        else        //否则有相应的数据发送过来 ，进行相应的处理
        {
            if ( ret <= (MAX_DATA_LEN + MSG_HEAD_LEN) ) //实际长度+帧头长度
            {  
                memcpy(p_msg_t,buf,ret);
                *msg_size = ret;
//                CORE_LOG_INFO("Get data len=%d data=%s\n",ret,buf); //发布时可以通过打印级别来控制此语句是否打印。                                             
            }
            else
            {
                memcpy(p_msg_t,buf,(MAX_DATA_LEN + MSG_HEAD_LEN));
                *msg_size = (MAX_DATA_LEN + MSG_HEAD_LEN);

            }
                
        }
    }

    return (ret);
}