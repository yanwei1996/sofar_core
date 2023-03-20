/************************************************************************************
 *Description: sdk_print
 *Created on: 2022-06-20
 *Author: quliuliu
 ************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#include "sdk_print.h"
// #include "sdk_print_debug.h"
// #include "sdk_print_error.h"

#define PRINT_BUF_LEN 1024

// 颜色打印
#define NONE         "\033[m"
#define RED          "\033[0;32;31m"
#define LIGHT_RED    "\033[1;31m"
#define GREEN        "\033[0;32;32m"
#define LIGHT_GREEN  "\033[1;32m"
#define BLUE         "\033[0;32;34m"
#define LIGHT_BLUE   "\033[1;34m"
#define DARY_GRAY    "\033[1;30m"
#define CYAN         "\033[0;36m"
#define LIGHT_CYAN   "\033[1;36m"
#define PURPLE       "\033[0;35m"
#define LIGHT_PURPLE "\033[1;35m"
#define BROWN        "\033[0;33m"
#define YELLOW       "\033[1;33m"
#define LIGHT_GRAY   "\033[0;37m"
#define WHITE        "\033[1;37m"

// 创建互斥锁
pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;

void get_sys_time(char *time)
{
    struct timeval tv;
    struct tm *tm_ptr;
    
    gettimeofday(&tv, NULL);
 
    tm_ptr = localtime(&tv.tv_sec);
    sprintf(time, "%d-%02d-%02d %02d:%02d:%02d.%ld", 1900+tm_ptr->tm_year, 1+tm_ptr->tm_mon, 
		tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec, tv.tv_usec/1000);
}

// debug & error打印接口
int sdk_print(int type, int level, const char *file, const char *func, int line, const char *fmt, ...)
{
    char time[32]={0};
    char buf[PRINT_BUF_LEN] = {0};
    char print_file_name[32] = {0};
    get_sys_time(time);
    switch (type) 
    {
        case 1:
            snprintf(buf, PRINT_BUF_LEN, BLUE "DEBUG " NONE "[%s][%s:%s:%d] ", time, file, func, line);
            break;
        case 2:
            snprintf(buf, PRINT_BUF_LEN, RED  "ERROR " NONE "[%s][%s:%s:%d] ", time, file, func, line);
            break;
        default:
            break;
    }
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf + strlen(buf), PRINT_BUF_LEN - strlen(buf), fmt, args);
    va_end(args);
    if(level==1)
    {
        printf("%s", buf);
    }
    else if(level==2)
    {
        sprintf(print_file_name, "/opt/log/%s.log", file);
        // 线程间进行互斥   // ===>进程间互斥以后增加   // ...code...
        pthread_mutex_lock(&print_mutex);
        FILE * fd = fopen(print_file_name,"a+");
        fputs(buf,fd);
        fclose(fd);
        pthread_mutex_unlock(&print_mutex);
    }
    else
    {
         
    }
    
    return 0;
}





