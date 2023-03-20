
#ifndef __LAL_RTC_H__
#define __LAL_RTC_H__


#include "data_types.h"


// typedef struct tm rtc_t;	
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
}rtc_t;


/**
* @brief        打开RTC       
* @return       执行结果
* @retval       >0 返回句柄  
* @retval       <0 lal_EIO 失败  
*/
int32_t lal_rtc_open(); 


/**
* @brief        关闭RTC      
* @return       执行结果
* @retval       lal_OK 成功  
* @retval       lal_EIO 失败  
*/
int32_t lal_rtc_close();


/**
* @brief        RTC控制
* @param        [in] cmd 控制命令
* @param        [in] p_arg 控制参数       
* @return       执行结果
* @retval       lal_OK 成功  
* @retval       lal_EIO 失败  
*/
int32_t lal_rtc_ioctl(uint8_t cmd, void *p_arg);


/**
* @brief        设置RTC时间
* @param        [in] rtc_format 时间格式
* @param        [in] p_time 时间结构体       
* @return       执行结果
* @retval       lal_OK 成功  
* @retval       lal_EIO 失败  
*/
int32_t lal_rtc_set(uint32_t rtc_format, rtc_t* p_time);


/**
* @brief        读取RTC时间
* @param        [in] rtc_format 时间格式
* @param        [in] p_time 时间结构体     
* @return       执行结果
* @retval       lal_OK 成功  
* @retval       lal_EIO 失败  
*/
int32_t lal_rtc_get(uint32_t rtc_format, rtc_t* p_time);


#endif /* __LAL_RTC_H__ */