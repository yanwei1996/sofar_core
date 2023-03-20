/************************************************************************************
 *Description: sdk_watchdog
 *Created on: 2022-12-10
 *Author: quliuliu
 ************************************************************************************/


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/watchdog.h>

#include "sdk_watchdog.h"

int wdt_fd = -1;

// 打开看门狗
int32_t sdk_open_watchdog()
{
    wdt_fd = open("/dev/watchdogs", O_WRONLY);
    if (wdt_fd == -1)  
    {
        printf("fail to open watchdogs!\n");
    }
}

// 关闭看门狗
int32_t sdk_close_watchdog()
{
    write(wdt_fd, "V", 1);
    close(wdt_fd);
    printf( "watchdogs is closeed!\n");
}

// 喂看门狗狗
int32_t sdk_watchdog_feed ()
{
    write(wdt_fd, "\0", 1); //写入任意字符喂狗

}

// 获取看门狗超时时间
int32_t sdk_get_watchdog_timeout()
{
    int timeout;
    ioctl(wdt_fd, WDIOC_GETTIMEOUT, &timeout);
}

// 设置看门狗超时时间
int32_t sdk_set_watchdog_timeout()
{
    int timeout;
    ioctl(wdt_fd, WDIOC_SETTIMEOUT, &timeout);
}








