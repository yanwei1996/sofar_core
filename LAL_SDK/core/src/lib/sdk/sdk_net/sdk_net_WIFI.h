/************************************************************************************
 *Description: sdk_net_wifi
 *Created on: 2023-2-28
 *Author: yanwei
 ************************************************************************************/
 
#ifndef __SDK_NET_WIFI_H__
#define __SDK_NET_WIFI_H__

#include "data_types.h"


#define WIFI_MAX_CONN_NUM    16
#define MAX_STR_BUFFER_LEN   32
#define MAX_NET_ADDR_LEN	15

typedef enum
{
   MODE_RF_CLOSE = 0,    // 关闭Wi-Fi
   MODE_STA,             // STA模式   
   MODE_AP,              // AP模式
   MODE_AP_STA,          // AP+STA模式
 }wifi_mode_e;
 
 typedef struct
{
    uint8_t str_len;
    uint8_t str_buf[MAX_STR_BUFFER_LEN];
}str_t;

typedef struct
{
    str_t   ssd;
    str_t   password;
    uint8_t en_dhcp;
}   wifi_ap_config_t;

typedef struct
{
    str_t   ssd;
    str_t   password;
}   wifi_sta_config_t;

typedef struct
{
    str_t    name;
    uint8_t  status;
    uint8_t  ip_addr[MAX_NET_ADDR_LEN];     // IP地址
    uint8_t  bssid[MAX_NET_ADDR_LEN];       // mac地址
    uint8_t  rssi;
}wifi_connect_sta_info_t;

typedef struct
{
   uint32_t num;
   wifi_connect_sta_info_t sta_info[WIFI_MAX_CONN_NUM];
}wifi_connect_sta_list_t;

typedef int32_t (*p_wifi_event_cb)(void);

typedef enum  
{
	WIFI_RESET,         // 设备复位
	WIFI_EVENT_MAX,
}wifi_evnet_e;

/**
 * @brief    设置模块的工作模式AP、STA、AP+STA
 * @param    [in] mode      wifi_mode_e类型，要设置的模式值。
 * @return   int32_t        >= 0 成功   <0失败 不同值对应不 同的错误代码。
 */
int32_t sdk_wifi_mode_set(wifi_mode_e mode);

/**
 * @brief    设置模块的工作模式STA、AP+STA下的AP参数
 * @param    [in] p_ap       wifi_ap_config_t类型，所要设置的参数数据
 * @return   int32_t        >= 0 成功   <0失败 不同值对应不 同的错误代码。
 * @note     调用此函数前确定模块是工作在AP、AP+STA模式下。
 */
int32_t sdk_wifi_ap_set(wifi_ap_config_t *p_ap);

/**
 * @brief    设置STA参数，模块工作在STA或AP+STA模式下有效
 * @param    [in] p_sta     wifi_sta_config_t类型，所要设置的参数数据
 * @return   int32_t        >= 0 成功   <0失败 不同值对应不 同的错误代码。
 * @note     调用此函数前确定模块是工作在STA或AP+STA模式下。
 */
int32_t sdk_wifi_sta_set(wifi_sta_config_t *p_sta);

/**
 * @brief    获取WIFI接收信号强度
 * @param    [in] p_rssi
 * @return   int32_t    >= 0 成功 强度值存放在p_rssi  <0失败 不同值对应不 同的错误代码。
 */
int32_t sdk_wifi_rssi_get(int16_t *p_rssi);  

/**
 * @brief    获取周围热点名称
 * @param    [in] p_sta     指向wifi_sta_config_t类型的地址
 * @param    [in] num       热点数量
 * @return   int32_t        >= 0 成功   <0失败 不同值对应不 同的错误代码。
 * @note     调用此函数前确定模块是工作在STA或AP+STA模式下。
 */
int32_t sdk_wifi_hotspots(wifi_sta_config_t *p_sta,uint8_t num);

/**
 * @brief    当做AP或AP+STA功能时，获取已连接sta的信息
 * @param    [in] p_sta     MyParamDoc
 * @return   int32_t        >= 0 成功 结果存放到 *p_sta中 <0失败 不同值对应不 同的错误代码。
 * @note     调用此函数前确定模块是工作在AP或AP+STA模式下。
 */
int32_t sdk_wifi_sta_info_get(wifi_connect_sta_list_t **p_sta);

/**
 * @brief    Wi-Fi事件回调注册
 * @param    [in] event     Wi-Fi事件类型
 * @param    [in] event_cb  事件回调函数指针
 * @return   int32_t >= 0 成功   <0失败 不同值对应不 同的错误代码。
 * @warning  禁止注册回调包含阻塞操作
 */
int32_t sdk_wifi_evt_regist(wifi_evnet_e event, p_wifi_event_cb event_cb);

#endif
