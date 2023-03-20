/**
 * @file         sdk_net_public.h
 * @brief        网卡接口
 * @author       yanwei
 * @version      V0.0.1
 * @date         2023/03/06
 * @copyright    Copyright (c) 2023 by SofarSolar, All Rights Reserved.
 **********************************************************************************
 * @par 修改日志:
 * <table>
 * <tr><th>Date        <th>Version  <th>Author    <th>Description
 * <tr><td>2023/02/16  <td>0.0.1    <td>yanwei     <td>创建初始版本
 * </table>
 *
 **********************************************************************************
 * @par 示例:
 * @code
 * 暂无
 * @endcode
 */
 
#ifndef __SDK_NET_PUBLIC_H__
#define __SDK_NET_PUBLIC_H__

#include "data_types.h"
#define MAX_NET_ADDR_LEN    16

typedef enum
{
   WLAN0 = 0,    //WIFI无线网卡 0
   WLAN1,        //WIFI无线网卡 1
   ETH0 = 4,    //有线网卡 0
   ETH1,        //有线网卡 1
   PPP0,     //4G
 }net_device_type_e;

typedef struct 
{
    int8_t ip_addr[MAX_NET_ADDR_LEN];     //IP地址
    int8_t mask_addr[MAX_NET_ADDR_LEN];   //掩码地址       
    int8_t gateway_addr[MAX_NET_ADDR_LEN];//网关地址
    int8_t pri_level;                 //网络优先级，不可为空。
}net_config_t;

typedef struct sdk_net
{
    uint8_t ip_addr[MAX_NET_ADDR_LEN];     //IP地址 ，可为空
    uint8_t mask_addr[MAX_NET_ADDR_LEN];   //掩码地址 ，可为空      
    uint8_t gateway_addr[MAX_NET_ADDR_LEN];//网关地址，可为空
    uint8_t pri_level;//网络优先级，不可为空
}net_route_config_t;

/**
 * @brief    对结构中的参数进行设置
 * @param    [in] nic       网卡设备。“网卡也叫“ 网络适配器 ”,英文全称为“Network Interface Card”,简称“NIC”。
 * @param    [in] p_buff    指向net_config_t类型的指针
 * @return   int32_t        >= 0 成功   <0失败 不同值对应不 同的错误代码。
 */
int32_t sdk_net_set(net_device_type_e nic, net_config_t *p_buff); 

/**
 * @brief    设置指定网卡的IP地址
 * @param    [in] nic       网卡设备
 * @param    [in] p_ip      指向uin8_t的字符数组
 * @return   int32_t        >= 0 成功   <0失败 不同值对应不 同的错误代码。 
 */
int32_t sdk_net_ip_set(net_device_type_e nic, uint8_t *p_ip);

/**
 * @brief    设置指定网卡的子网掩码地址
 * @param    [in] nic       网卡设备
 * @param    [in] p_subnet      指向uin8_t的字符数组
 * @return   int32_t        >= 0 成功   <0失败 不同值对应不 同的错误代码。 
 */
int32_t sdk_net_subnetmask_set(net_device_type_e nic, uint8_t *p_subnet);

/**
 * @brief    设置指定网卡的网关地址
 * @param    [in] nic             网卡设备
 * @param    [in] p_gateway       指向uin8_t的字符数组
 * @return   int32_t              >= 0 成功   <0失败 不同值对应不 同的错误代码。 
 */
int32_t sdk_net_gateway_set(net_device_type_e nic, uint8_t *p_gateway); 

/**
 * @brief     设置指定网卡的dns地址
 * @param    [in] nic             网卡设备
 * @param    [in] p_dns       指向uin8_t的字符数组
 * @return   int32_t              >= 0 成功   <0失败 不同值对应不 同的错误代码。 
 */
int32_t sdk_net_dns_set(net_device_type_e nic, uint8_t *p_dns);   //设置指定网卡的DNS服务地址

//下为获取数据接口函数：

/**
 * @brief    sdk_net_get    获取结构体中定义的参数进行设置
 * @param    [in] nic       网卡设备
 * @param    [in] p_buff    指向net_config_t的结构体
 * @return   int32_t        >= 0 成功   <0失败 不同值对应不 同的错误代码。 
 */
int32_t sdk_net_get(uint32_t nic, net_config_t *p_buff);  

/**
 * @brief    获取指定网卡的IP地址
 * @param    [in] nic       网卡设备
 * @param    [in] p_net_ip  指向uint8_t的字符串地址
 * @return   int32_t        >= 0 成功   <0失败 不同值对应不 同的错误代码。 
 */
int32_t sdk_net_ip_get(uint32_t nic, uint8_t *p_net_ip);  

/**
 * @brief    获取指定网卡的子网掩码地址
 * @param    [in] nic       网卡设备
 * @param    [in] p_subnet  指向uint8_t的字符串地址
 * @return   int32_t        >= 0 成功   <0失败 不同值对应不 同的错误代码。 
 */
int32_t sdk_net_subnetmask_get(uint32_t nic, uint8_t *p_subnet);//

/**
 * @brief    获取指定网卡的IP地址
 * @param    [in] nic       网卡设备
 * @param    [in] p_gateway  指向uint8_t的字符串地址
 * @return   int32_t        >= 0 成功   <0失败 不同值对应不 同的错误代码。 
 */
int32_t sdk_net_gateway_get(uint32_t nic, uint8_t *p_gateway);  //获取指定网卡的网关地址

/**
 * @brief    获取指定网卡的IP地址
 * @param    [in] nic       网卡设备
 * @param    [in] p_dns     指向uint8_t的字符串地址
 * @return   int32_t        >= 0 成功   <0失败 不同值对应不 同的错误代码。 
 */
int32_t sdk_net_dns_get(uint32_t nic, uint8_t *p_dns);   //获取指定网卡的DNS服务地址

/**
 * @brief    获取当前联网类型，如有线、WIFI、蜂窝网。
 * @return   int32_t  >0 联网类型 <0 获取失败
 */
int32_t sdk_net_type_get(void);//

/**************************以下与路由配置有关**********************************/

/**
 * @brief    添加路由到指定的网口,当p-gateway为空，表示不进行网关配置。
 * @param    [in] device_tye  net_device_type_e 设备类型
 * @param    [in] p_route     指向net_route_config_t类型的指针
 * @return   int32_t          >= 0 成功   <0失败 不同值对应不 同的错误代码。 
 */
int32_t sdk_net_route_add(net_device_type_e device_tye,net_route_config_t *p_route);

/**
 * @brief    删除指定网口的路由
 * @param    [in] device_tye  net_device_type_e 设备类型
 * @param    [in] p_route     指向net_route_config_t类型的指针
 * @return   int32_t          >= 0 成功   <0失败 不同值对应不 同的错误代码。 
 */
int32_t sdk_net_route_del(net_device_type_e *p_device_type, net_route_config_t *p_route);

/**
 * @brief    修改指定网口的路由
 * @param    [in] device_tye  net_device_type_e 设备类型
 * @param    [in] p_route     指向net_route_config_t类型的指针
 * @return   int32_t          >= 0 成功   <0失败 不同值对应不 同的错误代码。 
 */
int32_t sdk_net_route_revise(net_device_type_e *p_device_type, net_route_config_t *p_route);

/**
 * @brief    获取路由列表
 * @param    [in] p_route     指向net_route_config_t类型的指针
 * @param    [in] num         路由配置列表数量
 * @return   int32_t          >= 0 成功   <0失败 不同值对应不 同的错误代码。 
 */
int32_t sdk_net_route_get(net_route_config_t *p_route, uint32_t num);//应用暂时不用，开发优先级低


#endif








