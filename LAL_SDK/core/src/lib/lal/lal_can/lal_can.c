/************************************************************************************
 *Description: lal_can
 *Created on: 2022-12-10
 *Author: quliuliu
 ************************************************************************************/
#include <stdio.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <linux/socket.h>
#include <linux/can.h>
#include <linux/can/error.h>
#include <linux/can/raw.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include "lal_can.h"

#ifndef AF_CAN
#define AF_CAN 29
#endif
#ifndef PF_CAN
#define PF_CAN AF_CAN
#endif

static int32_t sock1;  //第一个can口对应的句柄
static int32_t sock2;  //第二个can口对应的句柄
//#define DEFAULT_BAUD 500000   //默认500KBit/s
#if 0
static void handle_err_frame(can_frame_t *fr)
{
    if (fr->can_id & CAN_ERR_TX_TIMEOUT) {
        // PRINT_ERROR(error_level, error_log, "error class: %s", CAN_ERR_TX_TIMEOUT);
    }
    if (fr->can_id & CAN_ERR_LOSTARB) {
        // PRINT_ERROR(error_level, error_log, "error class: %s; error code: %02x", CAN_ERR_LOSTARB, fr->data[0]);
    }
    if (fr->can_id & CAN_ERR_CRTL) {
        // PRINT_ERROR(error_level, error_log, "error class: %s; error code: %02x", CAN_ERR_CRTL, fr->data[1]);
    }
    if (fr->can_id & CAN_ERR_PROT) {
        // PRINT_ERROR(error_level, error_log, "error class: %s; error code: %02x %02x", CAN_ERR_PROT, fr->data[2], fr->data[3]);
    }
    if (fr->can_id & CAN_ERR_TRX) {
        // PRINT_ERROR(error_level, error_log, "error class: %s; error code: %02x", CAN_ERR_TRX, fr->data[4]);
    }
    if (fr->can_id & CAN_ERR_ACK) {
        // PRINT_ERROR(error_level, error_log, "error class: %s", CAN_ERR_ACK);
    }
    if (fr->can_id & CAN_ERR_BUSOFF) {
        // PRINT_ERROR(error_level, error_log, "error class: %s", CAN_ERR_BUSOFF);
    }
    if (fr->can_id & CAN_ERR_BUSERROR) {
        // PRINT_ERROR(error_level, error_log, "error class: %s", CAN_ERR_BUSERROR);
    }
    if (fr->can_id & CAN_ERR_RESTARTED) {
        // PRINT_ERROR(error_level, error_log, "error class: %s", CAN_ERR_RESTARTED);
    }
}

int32_t lal_can_open(lal_can_t *p_can, int32_t num, int32_t baudrate)
{   
    if((num!=0) && (num!=1))
    {
        printf("[%s:%s:%d] CAN device number error \n", __FILE__,__func__, __LINE__);
        return -1;
    }

    int8_t name[10]={0};
    sprintf(name, "can%d", num);
    strncpy(p_can->name, name, sizeof(p_can->name)-1);
    p_can->baudrate = baudrate;

    // 配置波特率，打开CAN设备
    int8_t order[128];
    memset(order, 0, sizeof(order));
    sprintf(order, "ifconfig %s down", p_can->name);//关闭CAN
    system(order);
    memset(order, 0, sizeof(order));
    sprintf(order, "ip link set %s type can bitrate %d triple-sampling on", p_can->name, p_can->baudrate);//设置CAN波特率
    system(order);        
    memset(order, 0, sizeof(order));
    sprintf(order, "ifconfig %s up", p_can->name);//打开CAN
    system(order); 

    // 创建套接口
    p_can->sock_handle = socket(AF_CAN, SOCK_RAW, CAN_RAW);
    if(p_can->sock_handle < 0)
    {
        printf("[%s:%s:%d] %s\n", __FILE__,__func__, __LINE__, strerror(errno));
        return -1;
    }

    // 将套接字与CAN绑定
    int32_t ret;
    struct sockaddr_can addr;   //CAN地址
    struct ifreq ifr;   //网络协议结构体
    strcpy(ifr.ifr_name, p_can->name);
    ret = ioctl(p_can->sock_handle, SIOCGIFINDEX, &ifr);   //指定CAN设备          
    if(ret < 0) 
    {
        printf("[%s:%s:%d] %s\n", __FILE__,__func__, __LINE__, strerror(errno));
        return -1;
    }
    addr.can_family = PF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    ret = bind(p_can->sock_handle, (struct sockaddr *)&addr, sizeof(addr));    
    if (ret < 0) {
        printf("[%s:%s:%d] %s\n", __FILE__,__func__, __LINE__, strerror(errno));
        return -1;
    }

	return 0;
}

int32_t lal_can_read(lal_can_t *p_can, can_frame_t *rframe, int32_t timeout_ms)
{
    int32_t len=-1;
    fd_set rd_set;
	struct timeval time_out;

	time_out.tv_sec=0;//秒
	time_out.tv_usec=(time_t)timeout_ms*1000;//微秒

	FD_ZERO(&rd_set); //每次循环都要清空集合，否则不能检测描述符变化  
	FD_SET(p_can->sock_handle,&rd_set); //添加描述符 

	switch(select(p_can->sock_handle+1,&rd_set,NULL,NULL,&time_out))//&time_out
	{  
		case -1:
            // printf("[%s:%s:%d] select failure %s\n", __FILE__,__func__, __LINE__, strerror(errno));
			return -1;  
		case 0:
            // printf("[%s:%s:%d] select timeout_ms %s\n", __FILE__,__func__, __LINE__, strerror(errno));
			return -1;
		default:
            if((len=read(p_can->sock_handle, rframe, sizeof(can_frame_t))) < sizeof(can_frame_t)) 
            {
                printf("[%s:%s:%d] %s\n", __FILE__,__func__, __LINE__, strerror(errno));
                return -1;
            }

            if(rframe->can_id & CAN_ERR_FLAG)   //检查数据帧是否错误
            {
                handle_err_frame(rframe);
                printf("[%s:%s:%d] CAN device error\n", __FILE__,__func__, __LINE__);
                return -1;
            }
	}

	return len;
}

int32_t skd_can_write(lal_can_t *p_can, can_frame_t *rframe)
{
    
    if(write(p_can->sock_handle, rframe, sizeof(can_frame_t)) < 0) 
    {
        printf("[%s:%s:%d] %s\n", __FILE__,__func__, __LINE__, strerror(errno));
        return -1;
    }

    return 0;
}

int32_t lal_can_close(lal_can_t *p_can)
{
	if(close(p_can->sock_handle)==-1)
	{
        printf("[%s:%s:%d] %s\n", __FILE__,__func__, __LINE__, strerror(errno));
		return -1;
	}
	return 0;
}
#else
static int32_t init_can_sock(const uint8_t port,const uint32_t baud)
{
    uint8_t cmd[64] = {0};
    uint8_t can_name[8] = {0};
    struct sockaddr_can addr;
    struct ifreq ifr;
    int32_t ret;
    int32_t sock;

    if((port != CAN_PORT_0) && (port != CAN_PORT_1))
    {
        printf("invalid port num\n");
        return -1;
    }

    if(port == CAN_PORT_0)
    {
        sock = sock1;
        strcpy(can_name,"can0");
    }
    else
    {
        sock = sock2;
        strcpy(can_name,"can1");
    }
    #if 0
    if(sock > 0)  
    {
        close(sock); //如果之前处于打开状态，这里需要先关闭
    }
    #endif

    sprintf(cmd,"ifconfig %s down",can_name);
    system(cmd);
    sprintf(cmd,"ip link set %s type can bitrate %d",can_name,baud); //设置通信波特率
    system(cmd);
    sprintf(cmd,"ifconfig %s up",can_name);
    system(cmd);

    sock = socket(PF_CAN, SOCK_RAW, CAN_RAW);        /* 创建套接字    */
    if (sock < 0) {
        perror("socket PF_CAN failed");
        return -1;
    }

    /* 把套接字绑定到can0接口    */
    strcpy(ifr.ifr_name,can_name);
    ret = ioctl(sock, SIOCGIFINDEX, &ifr);                
    if (ret < 0) {
        perror("ioctl failed");
        close(sock);
        return -1;
    }

    addr.can_family = PF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    ret = bind(sock, (struct sockaddr *)&addr, sizeof(addr));    
    if (ret < 0) {
        perror("bind failed");
        close(sock);
        return -1;
    }

    if(port == CAN_PORT_0)
    {
        sock1 = sock;
    }
    else
    {
        sock2 = sock;
    }
    return 0;
}

static int32_t deinit_can_sock(const uint8_t port)
{
    int32_t sock;
    if((port != CAN_PORT_0) && (port != CAN_PORT_1))
    {
        printf("%s %d invalid port num\n",__func__,__LINE__);
        return -1;
    }

    sock = (port == CAN_PORT_0 ? sock1 : sock2);

    if(sock > 0)
    {
        close(sock);
    }
    return 0;
}

int32_t lal_can_open(uint32_t port)
{
    //return init_can_sock(port,DEFAULT_BAUD);
    return 0;  //这里open不做相应的处理，处理放在setup函数中，即:open之后必须调用setup函数，才能使用。
}

int32_t lal_can_close(uint32_t port)
{
    return deinit_can_sock(port);
}

int32_t lal_can_setup(uint32_t port, lal_can_conf_t *p_conf)
{
    if(p_conf == NULL)
    {
        printf("%s %d,p_conf is NULL\n",__func__,__LINE__);
        return -1;
    }
    if(p_conf->mode != 0)
    {
        printf("unsupport mode\n");  //当前我们的只支持正常模式
        return -1;
    }
    return init_can_sock(port,p_conf->baud);
}

int32_t lal_can_rev(uint32_t port, lal_can_msg_t *p_buff, int32_t len, int32_t timeout_ms)
{
    int32_t sock;
    int32_t ret;
    struct can_frame fr;
    struct timeval tv;
    fd_set rset;

    if((port != CAN_PORT_0) && (port != CAN_PORT_1))
    {
        printf("%s %d invalid port num\n",__func__,__LINE__);
        return -1;
    }
    sock = (port == CAN_PORT_0 ? sock1 : sock2);
    tv.tv_sec = 0;
    tv.tv_usec = timeout_ms * 1000;
    FD_ZERO(&rset);
    FD_SET(sock, &rset);
    
    ret = select(sock+1, &rset, NULL, NULL, &tv);
    if (ret == 0) {
        printf("select time out");
        return -1;
    }
    if (ret < 0) {
        printf("select error");
        return -1;
    }
    /* select调用无错返回时，表示有符合规则的数据帧到达    */
    ret = read(sock, &fr,sizeof(fr));
    if (ret < sizeof(fr)) {
        printf("read failed");
        return -1;
    }
    if(ret != len)
    {
        printf("read length error\n");
        return -1;
    }
    memcpy(p_buff,&fr,sizeof(fr));
    return ret;
}

int32_t lal_can_send(uint32_t port, lal_can_msg_t *p_buff, int32_t len)
{
    int32_t sock;
    int32_t ret;

    if((port != CAN_PORT_0) && (port != CAN_PORT_1))
    {
        printf("%s %d invalid port num\n",__func__,__LINE__);
        return -1;
    }
    sock = (port == CAN_PORT_0 ? sock1 : sock2);

    ret = write(sock,(struct can_frame *)p_buff, sizeof(struct can_frame));        /* 把接收到的数据帧发送出去 */
    if (ret < 0) {
        printf("write failed");
        return -1;
    }
    return ret;
}

int32_t lal_can_flush(uint32_t port)
{
    return 0;
}

#endif


