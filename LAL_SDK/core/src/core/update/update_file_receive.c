/******************************************************
 * 说明：接收上位机或升级工具的发送的升级文件 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "sdk_ipc.h"
#include "data_shm.h"

#include "update_file_receive.h"

#define PORTNUM  12088
#define BUFFSIZE 1024

//第一帧数据格式
struct first_frame
{
	char username[16];
	char password[16];
	unsigned long file_len;
};


//接收上位机或升级工具发送的升级文件
void *update_file_receive(void *arg)
{
	//初始化socket套接字
	int sockfd = -1;
	if((sockfd=socket(AF_INET,SOCK_STREAM,0)) == -1)
	{
		printf("socket error [%s:%s:%d] %s\n", __FILE__,__func__, __LINE__, strerror(errno));
		exit(1);
	}
	
	//声明服务端sockaddr_in结构体变量并初始化
	struct sockaddr_in server_addr;
	bzero(&server_addr,sizeof(struct sockaddr_in));
	server_addr.sin_family=AF_INET;					//Internet地址族=AF_INET(IPv4协议)
	server_addr.sin_addr.s_addr=htonl(INADDR_ANY);	//IP地址
	server_addr.sin_port=htons(PORTNUM);			//将主机字节序转化为⽹络字节序
	
	//绑定端口和IP地址
	if(bind(sockfd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr))==-1)
	{
		printf("bind error [%s:%s:%d] %s\n", __FILE__,__func__, __LINE__, strerror(errno));
		exit(1);
	}

	//监听
	if(listen(sockfd,1)==-1)
	{
		printf("listen error [%s:%s:%d] %s\n", __FILE__,__func__, __LINE__, strerror(errno));
		exit(1);
	}

	//服务器阻塞,直到客户端程序建立连接
	int new_fd;
	struct sockaddr_in client_addr;
	socklen_t sock_size = sizeof(client_addr);
	if((new_fd=accept(sockfd,(struct sockaddr *)(&client_addr),&sock_size))==-1)
	{
		printf("accept error [%s:%s:%d] %s\n", __FILE__,__func__, __LINE__, strerror(errno));
		exit(1);
	}

	//接收第一帧数据，解析升级用户名和密码、文件大小
	char rbuf[BUFFSIZE]={0};
	// char wbuf[BUFFSIZE]={0};
	struct first_frame framedata={0};
	if(recv(new_fd, rbuf, BUFFSIZE, 0)==-1)
	{
		printf("recv error [%s:%s:%d] %s\n", __FILE__,__func__, __LINE__, strerror(errno));
		exit(1);
	}
	memcpy(framedata.username, &rbuf[0], 16);
	memcpy(framedata.password, &rbuf[16], 16);
	framedata.file_len = atol(&rbuf[32]);

	//校验用户和密码
	if(strlen(framedata.username) && strlen(framedata.password))
	{
		int i;
		for(i=0;i<10;i++)
		{
			if(strcmp((char *)shm->user[i].username, framedata.username)==0 && strcmp((char *)shm->user[i].password, framedata.password)==0)
			{
				//记录升级账号写入日志中
				// .....
				
				break;
			}
			else if(i==9)
			{
				send(new_fd, "err", BUFFSIZE, 0);
				close(new_fd);
				close(sockfd);
				system("reboot -f");
			}
		}
	}
	else
	{
		send(new_fd, "err", BUFFSIZE, 0);
		close(new_fd);
		close(sockfd);
		system("reboot -f");
	}

	//验证成功后，打开或创建接收文件
	FILE *fp = NULL;
	if((fp=fopen("/opt/update/sofar.tar","w+")) == NULL)
	{
		printf("fopen error [%s:%s:%d] %s\n", __FILE__,__func__, __LINE__, strerror(errno));
	}

	int frame_num = framedata.file_len/1024 + 1;

	while(frame_num)
	{
		//连续接收客户端信息
		memset(rbuf, 0, BUFFSIZE);
		if(recv(new_fd, rbuf, BUFFSIZE, 0)==-1)
		{
			printf("recv error [%s:%s:%d] %s\n", __FILE__,__func__, __LINE__, strerror(errno));
			exit(1);
		}

		//写入文件
		if(fwrite(rbuf, BUFFSIZE, 1, fp) != 1)
		{
			printf("fwrite error [%s:%s:%d] %s\n", __FILE__,__func__, __LINE__, strerror(errno));
			
		}

		//返回接收状态
		if(send(new_fd, "ok", BUFFSIZE, 0)==-1)
		{
			printf("send error [%s:%s:%d] %s\n", __FILE__,__func__, __LINE__, strerror(errno));
			exit(1);
		}

		frame_num--;
	}

	fclose(fp);

	//返回后台升级的进度
	// .....

	close(new_fd);
	close(sockfd);
	
	return NULL;
}

