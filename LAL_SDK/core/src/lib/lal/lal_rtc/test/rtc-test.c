//注意：波特率尽量不要用115200，有数据错误风险，用19200，高速光耦、晶振、中断影响

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include "../../../include/sdk_uart.h"

//编译指令
//arm-linux-gnueabihf-gcc uart-test.c -o uart-test  -L../  -l_sdk_uart



int main(int argc, char* argv[])
{
	int uart_port= atoi(argv[1]);
	int baudrate = atoi(argv[2]);
	printf("uart_port= %d\tbaudrate=%d\n", uart_port, baudrate);

	int len=-1;
	char buf[32]={0};                              //存信息的数组

	sdk_uart_t uart;
    uart_attr_t arrt = {baudrate, 8, "1", 'N', 0};    //设置串口

	if(sdk_uart_open(&uart, uart_port, &arrt) != 0)   // 打开串口
	{
		printf("sdk_can_open faild\n");
	}

	while(1)
    {
		memset(buf, 0, sizeof(buf));
		if((len=sdk_uart_read(&uart, buf, sizeof(buf), 1000)) < 0)
		{
			printf("sdk_uart_read faild\n");
		}

		for(int i=0;i<len;i++)
		{
			printf("%02x ", buf[i]);
		}
		printf("\n");

		if(skd_uart_write(&uart, buf, len) != 0)
		{
			printf("skd_uart_write faild\n");
		}
    }

	if(sdk_uart_close(&uart) != 0)
    {
		printf("sdk_uart_close faild\n");
    }

	return 0;
}

