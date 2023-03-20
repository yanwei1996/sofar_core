/************************************************************************************
 *Description: lal_uart
 *Created on: 2022-12-10
 *Author: quliuliu
 ************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>

#include "lal_uart.h"
#include "errors.h"

#define log_e(...)		printf(__VA_ARGS__)	//fixme 


//初始化
int32_t lal_uart_init(void)
{
	return 0;
}

static void set_baudrate(struct termios *opt, unsigned int baudrate)
{
	if(baudrate==9600)
	{
		cfsetispeed(opt,B9600);
		cfsetospeed(opt,B9600);
	}
	else if(baudrate==19200)
	{
		cfsetispeed(opt,B19200);/*设置串口接收波特率*/
		cfsetospeed(opt,B19200);/*设置串口发送波特率*/
	}
	else if(baudrate==38400)
	{
		cfsetispeed(opt,B38400);
		cfsetospeed(opt,B38400);
	}
    else if(baudrate==57600)
	{
		cfsetispeed(opt,B57600);
		cfsetospeed(opt,B57600);
	}
    else if(baudrate==115200)
	{
		cfsetispeed(opt,B115200);
		cfsetospeed(opt,B115200);
	}
    else
    {
        printf("baudrate err\n");
    }
}

static void set_stopbit(struct termios *opt, const char *stopbit)
{
    if (0 == strcmp (stopbit, "1")) {
    opt->c_cflag &= ~CSTOPB; /* 1位停⽌位t */
    }else if (0 == strcmp (stopbit, "1.5")) {
    opt->c_cflag &= ~CSTOPB; /* 1.5位停⽌位 */
    }else if (0 == strcmp (stopbit, "2")) {
    opt->c_cflag |= CSTOPB; /* 2 位停⽌位 */
    }else {
    opt->c_cflag &= ~CSTOPB; /* 1 位停⽌位 */
    }
}

static void set_data_bit(struct termios *opt, unsigned int databit)
{
    opt->c_cflag &= ~CSIZE;//在c_cflag里把表示数据位的位域清零
    switch (databit) {
    case 8:
    opt->c_cflag |= CS8;
    break;
    case 7:
    opt->c_cflag |= CS7;
    break;
    case 6:
    opt->c_cflag |= CS6;
    break;
    case 5:
    opt->c_cflag |= CS5;
    break;
    default:
    opt->c_cflag |= CS8;
    break;
    }
}

static void set_parity(struct termios *opt, char parity)
{
    switch (parity) {
    case 'N': /* ⽆校验 */
    case 'n':
    opt->c_cflag &= ~PARENB;
    break;
    case 'E': /* 偶校验 */
    case 'e':
    opt->c_cflag |= PARENB;
    opt->c_cflag &= ~PARODD;
    break;
    case 'O': /* 奇校验 */
    case 'o':
    opt->c_cflag |= PARENB;
    opt->c_cflag |= ~PARODD;
    break;
    case 'S':
    case 's':
    opt->c_cflag &= ~PARENB; /*清除校验位 disable pairty checking Space校验 */
    opt->c_cflag &= ~PARENB; /*清除校验位 disable pairty checking Space校验 */
    opt->c_cflag &= ~CSTOPB;
    opt->c_iflag |= INPCK;
    break;
    default: /* 其它选择为⽆校验 */
    opt->c_cflag &= ~PARENB;
    break;
    }
}

int32_t uart_attr_set(lal_uart_t *p_uart, uart_attr_t *p_attr) 
{
    struct termios opt;
    tcgetattr(p_uart->fd, &opt); //获取初始设置
    set_baudrate(&opt, p_attr->baudrate);//波特率
    set_data_bit(&opt, p_attr->databit);	//串口数据位databit
    set_parity(&opt, p_attr->parity);	//串口校验位parity
    set_stopbit(&opt, p_attr->stopbit);//串口停止位
    opt.c_cflag &= ~CRTSCTS; // 不使⽤硬件流控制
    opt.c_cflag |= (CLOCAL | CREAD); //保证本程序不会成为端口的所有者，从而妨碍控制工作和挂起信号.CLOCAL--忽略 modem 控制线,本地连线, 不具数据机控制功能, CREAD--使能接收标志
    
    opt.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);//选择行方式输入:行式输入是不经处理的.
    
    opt.c_oflag &= ~OPOST; //选择行式输出

    opt.c_iflag &= ~ICRNL;//预防0D强制变0A

    //取消软件流控制（不设置可能存在丢码）
    //opt.c_iflag &= ~(IXON | IXOFF | IXANY); 
    opt.c_iflag &= ~IXON;
    
	opt.c_cc[VMIN] = 0; //设置⾮规范模式下的超时时长和最⼩字符数：阻塞模式起作⽤
    opt.c_cc[VTIME] = 0; //VTIME与VMIN配合使⽤，是指限定的传输或等待的最长时间 单位：0.1S
    tcflush (p_uart->fd, TCIFLUSH); //清空串口接收发送缓冲中的数据
    
    return (tcsetattr (p_uart->fd, TCSANOW, &opt)); /* TCSANOW--改变⽴即发⽣ */
}

//读串口
#if 1

static lal_uart_t uart_fd[LAL_UART_MAX] = {0};

/**
 * @brief    打开串口
 * @param    [in] port 虚拟串口号
 * @return   0 成功
 * @return   <0 失败原因
 */
int32_t lal_uart_open(uint32_t port)
{
	int8_t uart_name[32]={0};

	int32_t ret = ERR_NONE;

	if (port >= LAL_UART_MAX)
	{
		ret = ERR_PARA;
		goto __exit;
	}

	if(port < 8)
	{
		sprintf(uart_name, "/dev/ttymxc%d", port);
	}
	else if(port==10)
	{
		sprintf(uart_name, "/dev/ttyACM0");
	}
	else
	{
		printf("[%s:%s:%d] uart device number error \n", __FILE__,__func__, __LINE__);
		goto __exit;
	}

	if (uart_fd[port].fd > 0)	//原先串口已打开，先关闭串口
	{
		ret = -2;
		goto __exit;
	}	

	uart_fd[port].fd = open(uart_name, O_RDWR|O_NOCTTY);//|O_NDELAY, 0 //O_RDWR | O_NOCTTY

	if(uart_fd[port].fd < 0)
	{
		perror("open uart device error\n");
	}

	ret = uart_fd[port].fd;

__exit:
	if (ret < 0)
	{
		log_e("Function[%s] Operation __exit, ret = %d!\r\n", __func__, ret);	
	}

	return ret;
}

/**
 * @brief    关闭串口
 * @param    [in] port 虚拟串口号
 * @return   0 成功；<0 失败
 */
int32_t lal_uart_close (uint32_t port) 
{
	int32_t ret = ERR_NONE;
	
	if (port >= LAL_UART_MAX)
	{
		ret = ERR_PARA;
	}
	else
	{
		ret = close(uart_fd[port].fd);	
		if (ret == 0)
		{
			uart_fd[port].fd = 0;
		}
	}

	if (ret < 0)
	{
		log_e("Function[%s] Operation __exit, ret = %d!\r\n", __func__, ret);	
	}

	return ret;
}

/**
 * @brief    配置串口属性
 * @param    [in] port        虚拟串口号
 * @param    [in] p_uart_conf 串口属性缓存地址
 * @return   0 成功；<0 失败
 */
int32_t lal_uart_setup(uint32_t port, lal_uart_conf_t *p_conf)
{
	int32_t ret = ERR_NONE;
	uart_attr_t uart_attr;

	if ((port >= LAL_UART_MAX) || (p_conf == NULL))
	{
		ret = ERR_PARA;
		return ret;
	}

	// 波特率
	uart_attr.baudrate = p_conf->baud_rate;
	
	// 数据位
	uart_attr.databit = p_conf->data_bits;	

	// 校验位—— fix me
	if (p_conf->parity == UART_PARITY_ODD)	// 奇校验
	{
		uart_attr.parity = 'O';
	}
	else if (p_conf->parity == UART_PARITY_EVEN)
	{
		uart_attr.parity = 'E';
	}
	else
	{
		uart_attr.parity = 'N';
	}

	// 停止位
	if (p_conf->stop_bit == UART_STOP_BITS_1)
	{
		uart_attr.stopbit = "1";
	}
	else if (p_conf->stop_bit == UART_STOP_BITS_2)
	{
		uart_attr.stopbit = "2";
	}
	else
	{
		uart_attr.stopbit = "1";		
	}

	// 流控制
	uart_attr.flow_ctrl = p_conf->flow_ctrl;	

	ret = uart_attr_set(&uart_fd[port], &uart_attr);

	if (ret < 0)
	{
		log_e("Function[%s] Operation __exit, ret = %d!\r\n", __func__, ret);	
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
int32_t lal_uart_read(uint32_t port, uint8_t *p_buf, int32_t len, int32_t timeout_ms)
{
    int32_t ret = -1;
	fd_set rd_set;
	struct timeval	time_out;

	if ((port >= LAL_UART_MAX) || (p_buf == NULL))
	{
		ret = ERR_PARA;
	}

	time_out.tv_sec=0;//秒
	time_out.tv_usec=(time_t)timeout_ms*1000;//微秒

	FD_ZERO(&rd_set); //每次循环都要清空集合，否则不能检测描述符变化  
	FD_SET(uart_fd[port].fd,&rd_set); //添加描述符 

	switch(select(uart_fd[port].fd+1, &rd_set, NULL, NULL, &time_out))
	{  
		case -1: 
			printf("select failure:%s\n",strerror(errno));
			return -1;  
		case 0:
			//printf("select timeout_ms\n");
			return -1;
		default: 
			if(FD_ISSET(uart_fd[port].fd, &rd_set))	//测试句柄是否可写
			{
				if((ret=read(uart_fd[port].fd, p_buf, len))<0)
				{
					printf("read from serial port failure:%s\n",strerror(errno));
					return -1;
				}
			}
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
int32_t lal_uart_write(uint32_t port, uint8_t *p_buf, int32_t len)
{
	int32_t ret = -1;

	if ((port >= LAL_UART_MAX) || (p_buf == NULL))
	{
		ret = ERR_PARA;
	}

	ret = write(uart_fd[port].fd, p_buf, len);

	if( ret < 0)
	{
		printf("write failure:%s\n",strerror(errno));
		ret = -1;
	}

	return ret;
}



#else	// old 屈工代码

//打开串口，num<8为ttymxc num==10为ttyACM0
int32_t lal_uart_open(lal_uart_t *p_uart, int32_t num, uart_attr_t *attr)
{
	int8_t uart_name[32]={0};
	if(num<8)
	{
		sprintf(uart_name, "/dev/ttymxc%d", num);
	}
	else if(num==10)
	{
		sprintf(uart_name, "/dev/ttyACM0");
	}
	else
	{
		printf("[%s:%s:%d] uart device number error \n", __FILE__,__func__, __LINE__);
	}
	

	p_uart->fd = open(uart_name, O_RDWR|O_NOCTTY);//|O_NDELAY, 0 //O_RDWR | O_NOCTTY
	if(p_uart->fd < 0)
	{
		perror("open uart device error\n");
		return -1;
	}

	int32_t ret = uart_attr_set(p_uart, attr);
	if(ret < 0)
	{
		perror("set uart attr failed \n");
		close(p_uart->fd);
		return -1;
	}

	return 0;
}
 
int32_t lal_uart_read(lal_uart_t *p_uart, int8_t *buf, uint32_t buf_len, uint32_t timeout_ms)
{
    int32_t len=-1;
	fd_set rd_set;
	struct timeval	time_out;

	time_out.tv_sec=0;//秒
	time_out.tv_usec=(time_t)timeout_ms*1000;//微秒

	FD_ZERO(&rd_set); //每次循环都要清空集合，否则不能检测描述符变化  
	FD_SET(p_uart->fd,&rd_set); //添加描述符 

	switch(select(p_uart->fd+1,&rd_set,NULL,NULL,&time_out))
	{  
		case -1: 
			printf("select failure:%s\n",strerror(errno));
			return -1;  
		case 0:
			//printf("select timeout_ms\n");
			return -1;
		default: 
			if(FD_ISSET(p_uart->fd, &rd_set))	//测试句柄是否可写
			{
				if((len=read(p_uart->fd, buf, buf_len))<0)
				{
					printf("read from serial port failure:%s\n",strerror(errno));
					return -1;
				}
			}
	}

	return len;
}

//写串口
int32_t skd_uart_write(lal_uart_t *p_uart, int8_t *buf, int32_t buf_len)
{
	int32_t len=-1;

	if((len=write(p_uart->fd, buf, buf_len))<0)
	{
		printf("write failure:%s\n",strerror(errno));
		return -1;
	}

	return len;
}

// 关闭串口
int32_t lal_uart_close(lal_uart_t *p_uart)
{
	if(close(p_uart->fd)==-1)
	{
		return -1;
	}
	return 0;
}

#endif
 






