/**
 * @file         core_ipc_server.c
 * @brief        进程间通讯服务进程
 * @author       APOI
 * @version      V0.0.1     初始版本
 * @date         2023/01/12
 * @copyright    Copyright (c) 2022 by SofarSolar, All Rights Reserved.
 */

#include "core_ipc_server.h"
#include "../../include/sdk/sdk_ipc.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUF_SIZE 200


static ipc_server_event_list_t event_list;  //事件注册列表
static fd_set fdsr; //文件描述符集的定义

static uint32_t conn_amount; //当前的连接数

/**
 * @brief    展现连接情况，事件注册列表的实时（变化时）打印
 */
static void show_client(void)
{
    int32_t out_loop = 0;
    int32_t in_loop = 0; 
    CORE_LOG_INFO("\n***********************Show start********************************\n");
    CORE_LOG_INFO("MAX_CLINE: %04d client_amount:%04d event_all_num=%04d\n",MAX_CLINE,conn_amount,event_list.event_num);
    for ( out_loop = 0; out_loop < event_list.event_num; out_loop++ )
    {
        CORE_LOG_INFO("\nevent_num [%02d] name: %-15s fd_num =%04d\n",out_loop, event_list.ipc_event[out_loop].name,event_list.ipc_event[out_loop].socket_num);
        for ( in_loop = 0; in_loop < event_list.ipc_event[out_loop].socket_num; in_loop++)
        {        
            CORE_LOG_INFO("socked[%02d]:%04d ",in_loop,event_list.ipc_event[out_loop].socket_id[in_loop]);
        }    
           
    }
    CORE_LOG_INFO("\n---------------------Show end------------------------------------\n");

}

/**
 * @brief    向指的send_id发送数据
 * @param    [in] type      消息类型，ipc_msg_type_e
 * @param    [in] send_id   所要发送数据的Socket_ID
 * @param    [in] p_buffer  存放将要发送数据的地址
 * @param    [in] len       数据长度
 * @return   执行结果 
 * @retval   >=0 成功 ,值对应的发送字节数
 * @retval   < 0失败，返回错误代码
 */
static int32_t send_msg(ipc_msg_type_e type, int32_t send_id, int8_t *p_buffer, uint32_t len)
{
    ipc_msg_t send_msg = {0};
    uint32_t send_len = len;
    int32_t  ret = 0;

    if ( type != EVENT_SEND_DATA )
    {
        send_msg.type = EVENT_RESPONSE;
        memcpy(send_msg.data,p_buffer,len);
        send_msg.data_len = len ; 
        send_len +=  MSG_HEAD_LEN; 
        ret = send(send_id, (int8_t *)&send_msg, send_len,0);
    }
    else
    {
        ret = send(send_id, p_buffer, send_len,0);
    }
    
    return(ret);
}

/**
 * @brief    读取指定事件的数据，并进行处理和转发（转发到同一事件中的所的ID)，内部使用
 * @param    [in] event_no  事件编号，最大值与业务允许创建最大进程数有关。
 * @param    [in] p_obj_id  数据来源ID，读取此ID收到的数据
 * @return   执行结果 
 * @retval   >=0 成功 ,值对应的发送字节数
 * @retval   < 0失败，返回错误代码
 */
static int32_t send_event_data(uint16_t event_no, int32_t *p_obj_id)
{
    int32_t  ret = 0;
    int32_t  socked_id = *p_obj_id;
    int32_t  socked_send_id = 0;

    uint16_t event_num = event_no;
    uint16_t i = 0; //循环使用

    int8_t rev_buf[MAX_DATA_LEN] = {0};
    uint16_t len = 0;

    ipc_msg_t *p_recv_msg;

    ret = recv(socked_id,rev_buf,MAX_DATA_LEN + MSG_HEAD_LEN,0);
    //如果客户端主动断开连接，会进行四次挥手，会出发一个信号，此时相应的套接字会有数据返回，告诉select，我的客户断开了，你返回-1
    if(ret <=0) //客户端连接关闭，清除文件描述符集中的相应的位
    {
        CORE_LOG_ERROR("event[%03d] client_id =%04d close ret=%d\n",event_no,socked_id,ret);

        close(socked_id);
        FD_CLR(socked_id,&fdsr);
        *p_obj_id = NULL_ZERO;
        conn_amount--;   
    }
    else     //否则有相应的数据发送过来 ，进行相应的处理
    {
        if (ret <= MAX_DATA_LEN + MSG_HEAD_LEN)
        {   
            p_recv_msg = (ipc_msg_t *)rev_buf;

            CORE_LOG_INFO("event[%d] client_id =%d  send:type =%d len=%d data=%s\n",event_no,socked_id,p_recv_msg->type,p_recv_msg->data_len,p_recv_msg->data);
            if ( p_recv_msg->type == EVENT_SEND_DATA )
            {
                for ( i = 0; i < event_list.ipc_event[event_num].socket_num; i++ )
                {
                    socked_send_id = event_list.ipc_event[event_num].socket_id[i];
                    if ( (socked_send_id != NULL_ZERO) && (socked_id != socked_send_id) )
                    {

                        ret = send_msg(EVENT_SEND_DATA,socked_send_id, p_recv_msg->data, p_recv_msg->data_len);
                              
                    }
                    
                }
            }else if ( p_recv_msg->type == EVENT_CONNECT )
            {
                len = strlen((const char *)STR_CONNECT_RES);
                ret = send_msg(EVENT_CONNECT,socked_id, (int8_t *)STR_CONNECT_RES, len);
            }
            
            
        }
            
    }
    return(ret);
}

/**
 * @brief    向指定的事件服务中添加连接
 * @param    [in] event_no  事件编号
 * @param    [in] socket_id 要向事件编号为event_no的事件中添加的连接ID 
 * @return   执行结果 
 * @retval   >=0 成功 ,值对应的发送字节数
 * @retval   =-1失败，
 */
static int32_t core_add_socketid_to_event(uint16_t event_no , int32_t socket_id)
{
    int32_t  ret = -1;
    uint16_t socket_num = 0;
    uint16_t i = 0;

    socket_num = event_list.ipc_event[event_no].socket_num;

    if ( socket_num <= MAX_PROCESS_NUM )    //为保证任一进程间都可以互相通讯，每个事件服务中最大的连接数取允许的最大进程数量。
    {
        for ( i = 0; i < socket_num; i++ )  //查找事件中是否存在值为 0 的ID
        {
            if (event_list.ipc_event[event_no].socket_id[i] == NULL_ZERO)
            {
                event_list.ipc_event[event_no].socket_id[i] = socket_id;
                break;
            }
        }

        if ( i == socket_num )  //事件数理内没有值为0的ID，需要增加
        {
            event_list.ipc_event[event_no].socket_num++;
            event_list.ipc_event[event_no].socket_id[i] = socket_id;
        }
        CORE_LOG_INFO("add socket to event:event_no=%d socket_num=%d id=%d i=%d\n",event_no, socket_num,socket_id,i);

        ret = 0;
    }
    else
    {
        CORE_LOG_INFO("Alread arrive event max id=%d \n",MAX_PROCESS_NUM);
    }
       
    return(ret);

}

/**
 * @brief    向服务中增加所需要服务的事件
 * @param    [in] event_no  事件编号
 * @param    [in] p_name    事件名称（名字）
 * @param    [in] socket_id 要向事件中增加的连接ID号
 * @return   执行结果 
 * @retval   >=0 成功,值对应的发送字节数
 * @retval   =-1失败，
 */
static int32_t core_add_event(uint16_t event_no, int8_t *p_name, int32_t socket_id)
{
    int32_t  ret = -1;
    uint16_t event_num = event_no; //传入的编号，认为就是目前事件的数量。

    if ( event_num <= MAX_CLINE )
    {
        memcpy(event_list.ipc_event[event_num].name,p_name,MAX_NAME_LEN);
        event_list.ipc_event[event_num].socket_num = 0;
        core_add_socketid_to_event(event_num , socket_id);
        event_list.event_num++;

        ret = 0;
    }
    else
    {
        CORE_LOG_INFO("Alread arrive max_event=%d \n",MAX_CLINE);
    }
    
    return(ret);
}

/**
 * @brief    注册管理函数，主要进行事件的合法性判断和增加。
 * @param    [in] pbuffer   所要处理的数据存在地址
 * @param    [in] reg_id    要注册的连接ID
 * @return   执行结果 
 * @retval   >=0 成功 ,值对应的发送字节数
 * @retval   =-1失败，
 */
static int32_t core_register_manage( int8_t* pbuffer, int32_t reg_id)
{   
    int32_t  send_id = reg_id;
    int32_t  ret = -1;
    uint16_t send_len = 0;
    uint16_t i = 0;

    int8_t name_buffer[MAX_NAME_LEN] = {0};
    int8_t send_buffer[50] = {0};

    ipc_msg_t *p_recv_msg = (ipc_msg_t *)pbuffer;
//      memset(&buf[ret],'\0',1);
    CORE_LOG_INFO("client[%d] send:type =%d len=%d data=%s\n",reg_id,p_recv_msg->type,p_recv_msg->data_len,p_recv_msg->data);
    if ( p_recv_msg->type == EVENT_REGIST ) //事件注册
    {   
        if ( p_recv_msg->data_len >= MAX_NAME_LEN )    //名字长度合法性判断
        {
            memcpy(&name_buffer,p_recv_msg->data,MAX_NAME_LEN-1);
        }
        else
        {
            memcpy(&name_buffer,p_recv_msg->data,p_recv_msg->data_len);
        }

        if ( 0 == event_list.event_num)
        {
            ret = core_add_event(0,name_buffer,send_id);        //添加事件
        }
        else
        {
            for ( i = 0; i < event_list.event_num; i++)
            {
                ret = strcmp((const char *)event_list.ipc_event[i].name, (const char *)name_buffer);
                if ( 0 == ret)   //有相同的事件
                {
                    ret = core_add_socketid_to_event(i,send_id);
                    break;
                }                                       
            }   

            if (i == event_list.event_num)  //上面循环没有找到相同事件，需要增加事件
            {
                ret = core_add_event(i,name_buffer,send_id);        //添加事件
            }
                     
        }
        
        if (ret < 0)
        {
            strcat((char*)send_buffer,"error");
        }
        else
        {
            strcat((char *)send_buffer,(const char*)p_recv_msg->data);
            strcat((char*)send_buffer,"ok");
        }
               
        send_len = strlen((const char*)send_buffer); //
        CORE_LOG_INFO("Regst:send_len=%d resp data %s\n",send_len,(const char*)send_buffer);
        send_msg(EVENT_RESPONSE,send_id, send_buffer, send_len);       
    }  
    return(ret);
}

int32_t main(void)
{
    int32_t sock_fd = NULL_ZERO;//监听套接字
    int32_t new_fd = NULL_ZERO; // 连接套接字
    int32_t regist_fd = NULL_ZERO;
    int32_t i = 0;
    int32_t j = 0;
    int32_t yes = 1;
    int32_t ret;
    int32_t maxsock;

    uint16_t len = 0;

    int8_t buf[BUF_SIZE];

    socklen_t sin_size;
    struct timeval tv;
    struct sockaddr_in server_addr; // 服务器的地址信息
    struct sockaddr_in client_addr; //客户端的地址信息

  //建立sock_fd套接字
    if((sock_fd = socket(AF_INET,SOCK_STREAM,0))==-1)
    {
        perror("setsockopt");
        exit(1);
    }
    printf("sockect_fd = %d\n", sock_fd);
    //设置套接口的选项 SO_REUSEADDR 允许在同一个端口启动服务器的多个实例
    // setsockopt的第二个参数SOL SOCKET 指定系统中，解释选项的级别 普通套接字
    if(setsockopt(sock_fd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int))==-1)
    {
        perror("setsockopt error \n");
        exit(1);
    }
  
    server_addr.sin_family = AF_INET; //主机字节序
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;//通配IP
    memset(server_addr.sin_zero,'\0',sizeof(server_addr.sin_zero));
    if(bind(sock_fd,(struct sockaddr *)&server_addr,sizeof(server_addr)) == -1)
    {
        CORE_LOG_ERROR("bind error!\n");
        exit(1);
    }
    if(listen(sock_fd,MAX_CLINE)==-1)
    {
        CORE_LOG_ERROR("listen error!\n");
        exit(1);
    }
    CORE_LOG_INFO("listen port %d\n",SERVER_PORT);

    conn_amount =0;
    sin_size = sizeof(client_addr);
    maxsock = sock_fd;
    while(1)
    {
        //这两部是非常重要的，不可缺少的，缺少了有可能导致状态不会更新
        FD_ZERO(&fdsr);
        FD_SET(sock_fd,&fdsr); 

        if (regist_fd != NULL_ZERO)
        {
            FD_SET(regist_fd,&fdsr); 
        }
               
        //超时的设定，这里也可以不需要设置时间，将这个参数设置为NULL,表明此时select为阻塞模式
        tv.tv_sec = 30;
        tv.tv_usec =0;
        
        //将所有的连接全部加到这个这个集合中，可以监测客户端是否有数据到来
        for ( i = 0; i < event_list.event_num; i++)
        {
            for ( j = 0; j < event_list.ipc_event[i].socket_num; j++)
            {
                if (event_list.ipc_event[i].socket_id[j] != NULL_ZERO)
                {
                     FD_SET(event_list.ipc_event[i].socket_id[j],&fdsr);
                }
            }            

        }
        
        memset(&buf,0,BUF_SIZE);
        //如果文件描述符中有连接请求 会做相应的处理，实现I/O的复用 多用户的连接通讯
        ret = select(maxsock +1,&fdsr,NULL,NULL,&tv);
        if(ret < 0) //没有找到有效的连接 失败
        {
            CORE_LOG_ERROR("select error!\r\n");
            break;
        }
        else if(ret ==0)// 指定的时间到，
        {
            CORE_LOG_INFO("timeout \n");
            continue;
        }

        //下面这个循环是非常必要的，因为你并不知道是哪个连接发过来的数据，所以只有一个一个去找。
        for ( i = 0; i < event_list.event_num; i++)
        {
            for ( j = 0; j < event_list.ipc_event[i].socket_num; j++)
            {
//                CORE_LOG_INFO("i [%d] j=%d id =%d\n",i,j ,event_list.ipc_event[i].socket_id[j] );
                if (event_list.ipc_event[i].socket_id != NULL)
                {
//                     FD_SET(event_list.ipc_event[i].socket_id[j],&fdsr);
                    if (FD_ISSET(event_list.ipc_event[i].socket_id[j],&fdsr))
                    {
                        send_event_data(i, &event_list.ipc_event[i].socket_id[j]);
                    }
                }
            }                       
        }
        if(FD_ISSET(regist_fd,&fdsr))
        {
            ret = recv(regist_fd,buf,sizeof(buf),0);
            //如果客户端主动断开连接，会进行四次挥手，会出发一个信号，此时相应的套接字会有数据返回，告诉select，我的客户断开了，你返回-1
            
            if(ret <=0) //客户端连接关闭，清除文件描述符集中的相应的位
            {
              CORE_LOG_ERROR("client[%04d] close ret=%04d\n",i,ret);
              close(regist_fd);
              FD_CLR(regist_fd,&fdsr);
              regist_fd = NULL_ZERO;
              conn_amount--;
              
            }
            //否则有相应的数据发送过来 ，进行相应的处理
            else
            {
                if (ret < BUF_SIZE)
                {   
                    ret = core_register_manage(buf,regist_fd);
                 
                    regist_fd = NULL_ZERO;                               
                }                   
            }
            show_client();
        }
        if(FD_ISSET(sock_fd,&fdsr))
        {
            new_fd = accept(sock_fd,(struct sockaddr *)&client_addr,&sin_size);
            if(new_fd <=0)
            {
                perror("accept error\n");
                continue;
            }
            //添加新的fd 到数组中 判断有效的连接数是否小于最大的连接数，如果小于的话，就把新的连接套接字加入集合
            if(conn_amount < MAX_CLINE)
            {
                regist_fd = new_fd;
                conn_amount++;
                CORE_LOG_INFO("new connection client[%d]%s:%d\n",conn_amount,inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
                if(new_fd > maxsock)
                {
                    maxsock = new_fd;
                }
                show_client();
            }        
            else
            {
                CORE_LOG_INFO("Max connections arrive ,exit\n");
                len = sizeof(STR_OVER_MAX_CONNECT);
                send_msg(EVENT_SEND_DATA,new_fd, (int8_t *)STR_OVER_MAX_CONNECT, len);
                close(new_fd);
                len = 0;
                continue;
            }
        }
//        show_client();
    } 
        exit(0);
  }