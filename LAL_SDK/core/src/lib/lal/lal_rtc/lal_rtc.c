

#include "lal_rtc.h"
#include <errno.h>
#include <time.h>
#include<unistd.h>
#include<sys/time.h>
#include <stdio.h>
#include <string.h>

/**
* @brief        设置RTC时间
* @param        [in] rtc_format 时间格式
* @param        [in] p_time 时间结构体       
* @return       执行结果
* @retval       lal_OK 成功  
* @retval       lal_EIO 失败  
*/
int32_t lal_rtc_set(uint32_t rtc_format, rtc_t* p_time)
{
    struct tm tm_val;
	
	//补充：1、入参合法性检测； 2、变量统一函数头定义
	
    tm_val.tm_year = p_time->tm_year;
    tm_val.tm_mon  = p_time->tm_mon;
    tm_val.tm_mday = p_time->tm_day;
    tm_val.tm_hour = p_time->tm_hour;
    tm_val.tm_min  = p_time->tm_min;
    tm_val.tm_sec  = p_time->tm_sec;	
	
	time_t time_set = mktime(&tm_val);

    struct timeval tv;
    tv.tv_sec = time_set;
    tv.tv_usec = 0;
    if(settimeofday(&tv, (struct timezone *) 0) < 0)
    {
        printf("\n set system datatime error \n");
        return -1;
    }
 
    FILE *fp = NULL;
    if((fp=popen("hwclock -w", "r")) == NULL)
    {
        printf("[%s:%s:%d] %s\n", __FILE__,__func__, __LINE__, strerror(errno));
        return -1;
    }
    pclose(fp);	
	
	return 0;
}


/**
* @brief        读取RTC时间
* @param        [in] rtc_format 时间格式
* @param        [in] p_time 时间结构体     
* @return       执行结果
* @retval       lal_OK 成功  
* @retval       lal_EIO 失败  
*/
int32_t lal_rtc_get(uint32_t rtc_format, rtc_t* p_time)
{
	//补充：1、入参合法性检测； 2、变量统一函数头定义

	time_t now;

	time(&now);	//获得时间戳：自1970-01-01起的秒数
	struct tm *p_tm = localtime(&now);	//localtime()获取系统时间(本地时间)
    p_time->tm_year = p_tm->tm_year;
    p_time->tm_mon  = p_tm->tm_mon;
    p_time->tm_day = p_tm->tm_mday;
    p_time->tm_hour = p_tm->tm_hour;
    p_time->tm_min  = p_tm->tm_min;
    p_time->tm_sec  = p_tm->tm_sec;
	
	return 0;
}