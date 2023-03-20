/**
 * @file		sdk_public.c
 * @brief		功能模块（含时间管理、看门狗管理、软件复位等）
 * @copyright	Shouhang
 * @author		yangwenjuan
 * @note		2022.12.28 新拟制
 * @version		V1.0   
 * @date		2022/12/28
 */


#include "data_types.h"
#include "lal_rtc.h"
#include "errors.h"
#include "sdk_public.h"
#include <sys/time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/times.h>
#include <sys/reboot.h>


#define log_e(...)		printf(__VA_ARGS__)	    //fixme 后面easylog可以用后，屏蔽掉  

/**
 * @brief	ms延时函数（阻塞延时） 
 * @param	[in] 需要延时的ms数
 * @return	无 
 */
void sdk_delay_ms(uint32_t ms)
{
    struct timeval tv;

    tv.tv_sec = 0;        		 	 /*SECOND */
    tv.tv_usec = (time_t)ms * 1000;  /*MICROSECOND */
    select(0, NULL, NULL, NULL, &tv);	
}

/**
 * @brief	延时us
 * @param	[in] 需要延时的us数, <1000us
 */
void sdk_delay_us(uint32_t us)
{
    struct timeval tv;

    tv.tv_sec = 0;        		 	 /*SECOND */
    tv.tv_usec = (time_t)us;  		 /*MICROSECOND */
    select(0, NULL, NULL, NULL, &tv);	
}

/**
 * @brief	rtc时间入参合法性/有效性检查————内部函数，非SDK接口函数
 * @param	[in] p_time rtc时间结构体,详见sdk_rtc_t定义  
 * @return	判断结果 
 * @retval	true	参数合法
 * @retval	false	参数非法
 */
static bool check_is_rtc_time_valid(sdk_rtc_t *p_time)
{
	static int32_t mon_days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};	//每个月的总天数
	bool ret = true;
	
	if (p_time == NULL)
	{
		return false;
	}	
	
	if (((p_time->tm_year % 4 == 0) && (p_time->tm_year % 100 != 0)) || (p_time->tm_year % 400 == 0)) //判断是否是闰年
	{
		mon_days[1] = 29;	//闰年2月天数为29天
	}
	else
	{
		mon_days[1] = 28;	//平年2月天数为28天
	}
	
	if ((p_time->tm_year > 37) || (p_time->tm_mon < 1) || (p_time->tm_mon > 12))	//年份、月份无效 陈晓2023.3.15修改,32位系统只能到2037
	{
		ret = false;
	}
	else if ((p_time->tm_day < 1) || (p_time->tm_day > mon_days[p_time->tm_mon]))	//日期无效	
	{
		ret = false;
	}
	else if ((p_time->tm_hour < 0) || (p_time->tm_hour > 23)\
        || (p_time->tm_min < 0) || (p_time->tm_min > 59)\
        || (p_time->tm_sec < 0) || (p_time->tm_sec > 59))
	{
		ret = false;    //时、分、秒无效
	}
	else
	{
		ret = true;		
	}
	
	return ret;	
}

/**
  * @brief	设置RTC时间 
  * @param	[in] rtc_format 时间格式:BCD/HEX
			[in] p_time rtc时间结构体,详见sdk_rtc_t定义   
  * @return	执行结果
  * @retval	ERR_NONE 成功  
  * @retval	ERR_IO 失败
  * @retval	ERR_PARA 失败   
  */
int32_t sdk_rtc_set(uint32_t rtc_format, sdk_rtc_t *p_time) 
{
	int32_t ret = ERR_NONE;
    rtc_t rtc_time;

	if (((rtc_format != RTC_BIN_FORMAT) && (rtc_format != RTC_BCD_FORMAT)) || (p_time == NULL))	
	{
		ret = ERR_PARA;
		goto __exit;
	}
	
	ret = check_is_rtc_time_valid(p_time);
	if (!ret)
	{
		ret = ERR_PARA;
		goto __exit;		
	}	

    rtc_time.tm_year = p_time->tm_year + 100;//p_time->tm_year-1900;
    rtc_time.tm_mon  = p_time->tm_mon-1;
    rtc_time.tm_day = p_time->tm_day;
    rtc_time.tm_hour = p_time->tm_hour;
    rtc_time.tm_min  = p_time->tm_min;
    rtc_time.tm_sec  = p_time->tm_sec;
	ret = lal_rtc_set(rtc_format, &rtc_time);

__exit:
	if (ret < 0)
	{
		log_e("Function[%s] Operation failed, ret = %d\r\n", __func__, ret);
		log_e("time: %02d.%02d.%02d--%02d:%02d:%02d!\r\n", rtc_time.tm_year, rtc_time.tm_mon, rtc_time.tm_day, rtc_time.tm_hour, rtc_time.tm_min, rtc_time.tm_sec);
	}
	return ret;
}

/**
  * @brief	读取RTC时间 
  * @param	[in] rtc_format 时间格式:BCD/HEX
			[out] p_time rtc时间结构体,详见sdk_rtc_t定义
  * @return	执行结果
  * @retval	ERR_NONE 成功  
  * @retval	ERR_IO 失败  
  * @retval	ERR_PARA 失败   
  */
int32_t sdk_rtc_get(uint32_t rtc_format, sdk_rtc_t *p_time)
{
	int32_t ret = ERR_NONE;
    rtc_t rtc_time;

	if (((rtc_format != RTC_BIN_FORMAT) && (rtc_format != RTC_BCD_FORMAT)) || (p_time == NULL))	
	{
		ret = ERR_PARA;
		goto __exit;
	}	
	ret = lal_rtc_get(rtc_format, &rtc_time);
	if (ret == ERR_NONE)
	{
        p_time->tm_year = rtc_time.tm_year-100;
        p_time->tm_mon = rtc_time.tm_mon + 1; //陈晓修改rtc的月份是1~12
        p_time->tm_day = rtc_time.tm_day;	//rtc_time.tm_mday
        p_time->tm_hour = rtc_time.tm_hour;
        p_time->tm_min = rtc_time.tm_min; 
        p_time->tm_sec = rtc_time.tm_sec;                                      
	}

__exit:
	if (ret < 0)
	{
		log_e("Function[%s] Operation failed, ret = %d!\r\n", __func__, ret);
	}
	return ret;
}

/**
 * @brief	获取当前tick
 * @return	当前tick值
 */
uint32_t sdk_tick_get(void)
{
	uint32_t now_tick;
	
	now_tick = times(NULL);	//返回自系统运行以来已运行的ticks数
	
	return now_tick;
}

/**
 * @brief	判断时间tick是否超时
 * @param	[in] start_tick 启动计时的tick  
 * @param	[in] interval 中间间隔
 * @return	是否超时 
 * @retval	true 已超时
 * @retval	false 未超时
 */
bool sdk_is_tick_over(uint32_t start_tick, uint32_t interval)	//自查
{
	uint32_t now_tick, end_tick;
	
	now_tick = times(NULL);
	if ((0xFFFFFFFF - start_tick) >= interval)	//无翻转 
	{
		end_tick = start_tick + interval;	
		if (now_tick >= end_tick)	
		{
			return true;	
		}
		else
		{
			return false;
		}	
	}
	else
	{
		end_tick = interval - (0xFFFFFFFF - start_tick);	
		if ((now_tick >= end_tick) && (now_tick <= start_tick))	
		{
			return true;	
		}
		else
		{
			return false;
		}		
	}	
}

/**
 * @brief	判断rtc时间是否超时
 * @param	[in] *p_start_time 启动rtc时间 
 * @param	[in] interval 中间间隔 (单位S)
 * @return	是否超时 
 * @retval	1 已超时
 * @retval	0 未超时
 * @retval	<0 异常
 */
int32_t sdk_is_time_over(sdk_rtc_t *p_start_time, uint32_t interval)	//接口规范得改，原先接口定义为bool型
{
	int32_t ret = ERR_NONE;
    struct tm tm_val;
	time_t start_time, now;
		
	ret = check_is_rtc_time_valid(p_start_time);//入参合法性判断
	if (!ret)
	{
		ret = ERR_PARA;
		goto __exit;
	}

	tm_val.tm_year = p_start_time->tm_year + 100;	// 原320= 实际年份（16位数，如2023） - 1900， 现在传入的是8位数，比如2023，传入23.  
    //tm_val.tm_year = p_start_time->tm_year-1900; 
    tm_val.tm_mon  = p_start_time->tm_mon-1;
    tm_val.tm_mday = p_start_time->tm_day;
    tm_val.tm_hour = p_start_time->tm_hour;
    tm_val.tm_min  = p_start_time->tm_min;
    tm_val.tm_sec  = p_start_time->tm_sec;	
	
	start_time = mktime(&tm_val);

	time(&now);	//获得时间戳：自1970-01-01起的秒数
	
	if (now >= (start_time + interval))
	{
		ret = 1;	//true 	//定义宏？
	}
	else
	{
		ret = 0;
	}
	return ret;
	
__exit:
	if (ret < 0)
	{
		log_e("Function[%s] Operation failed, ret = %d!\r\n", __func__, ret);
	}
	return ret;
}


/**
 * @brief   看门狗使能
 * @param   [in] state ENABLE = 1 使能 DISABLE = 0 禁能
 * @return   void
 */
int32_t sdk_wdt_enable(uint8_t state)
{
	return -1;
}

/**
 * @brief	喂狗
 * @return	void
 */
int32_t sdk_wdt_feed(void)
{
	return -1;
}

/**
 * @brief	系统复位
 * @return	void
 */
void sdk_sys_reset(void)
{
	sync();
	reboot(RB_AUTOBOOT);
}
