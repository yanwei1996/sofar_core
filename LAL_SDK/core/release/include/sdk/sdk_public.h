#ifndef __SDK_PUBLIC_H__
#define __SDK_PUBLIC_H__

#include "stdbool.h"
#include <stdint.h>


#define RTC_BIN_FORMAT        		((uint32_t)0x000000000)
#define RTC_BCD_FORMAT        		((uint32_t)0x000000001)
#define SDK_IS_RTC_FORMAT(FORMAT)   (((FORMAT) == HAL_RTC_FORMAT_BIN) || ((FORMAT) == HAL_RTC_FORMAT_BCD))


/**
  * @struct hal_rtc_time_t
  * @brief RTC时间。
  */
typedef struct  {
    uint8_t  tm_sec;     ///<  秒 	0-59
    uint8_t  tm_min;     ///<  分钟	0-59 
    uint8_t  tm_hour;    ///<  小时 1-12
    uint8_t  tm_weekday; ///<  星期	1-7
    uint8_t  tm_day;     ///<  日 	1-31
    uint8_t  tm_mon;     ///<  月 	1-12
    uint8_t  tm_year;    ///<  年 范围00-99
}sdk_rtc_t;




#ifndef SDK_API_INTERFACE_ENABLE	///< SDK接口对外声明是否打开


/**
 * @brief   延时ms
 * @param   [in] 需要延时的ms数
 */
void sdk_delay_ms(uint32_t ms);

/**
 * @brief   延时us
 * @param   [in] 需要延时的us数, <1000us
 */
void sdk_delay_us(uint32_t us);

/**
 * @brief   设置RTC时间 
 * @param   [in] rtc_format 时间格式:BCD/HEX
			[in] p_time rtc时间结构体,详见sdk_rtc_t定义   
 * @return   执行结果
 * @retval   ERR_NONE 成功  
 * @retval   ERR_IO 失败   
 */
int32_t sdk_rtc_set(uint32_t rtc_format, sdk_rtc_t *p_time);


/**
 * @brief   读取RTC时间 
 * @param   [in] rtc_format 时间格式:BCD/HEX
			[out] p_time rtc时间结构体,详见hal_rtc_t定义
 * @return   执行结果
 * @retval   ERR_NONE 成功  
 * @retval   ERR_IO 失败   
 */
int32_t sdk_rtc_get(uint32_t rtc_format, sdk_rtc_t *p_time);


/**
 * @brief   获取当前tick
 * @return   当前tick值
 */
uint32_t sdk_tick_get(void);

/**
 * @brief   判断时间tick是否超时
 * @param   [in] start_tick 启动计时的tick  
 * @param   [in] interval 中间间隔
 * @return   是否超时 
 * @retval   true 已超时
 * @retval   false 未超时
 */
bool sdk_is_tick_over(uint32_t start_tick, uint32_t interval);

/**
 * @brief	判断rtc时间是否超时
 * @param	[in] *p_start_time 启动rtc时间 
 * @param	[in] interval 中间间隔 (单位S)
 * @return	是否超时 
 * @retval	1 已超时
 * @retval	0 未超时
 * @retval	<0 异常
 */
int32_t sdk_is_time_over(sdk_rtc_t *p_start_time, uint32_t interval);

/**
 * @brief   看门狗使能
 * @param   [in] state ENABLE = 1 使能 DISABLE = 0 禁能
 * @return   void
 */
int32_t sdk_wdt_enable(uint8_t state);

/**
 * @brief	喂狗
 * @return	void
 */
int32_t sdk_wdt_feed(void);

/**
 * @brief	系统复位
 * @return	void
 */
void sdk_sys_reset(void);




#endif
#endif

