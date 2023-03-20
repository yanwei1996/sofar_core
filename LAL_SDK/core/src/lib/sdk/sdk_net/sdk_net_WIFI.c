/************************************************************************************
 *Description: sdk_net_WIFI
 *Created on: 2023-2-28
 *Author: yanwei
 ************************************************************************************/
#include "sdk_net_WIFI.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#include "stdint.h"

//获取WIFI接收信号强度。
int32_t sdk_wifi_rssi_get(int16_t *p_rssi)
{
	FILE *fp = NULL;
	int8_t buffer[8]={0};

	  //获取信号强度
	fp=popen("cat /proc/net/wireless |grep wlan0 |awk '{print $3}'", "r");
	fgets(buffer, sizeof(buffer), fp); 
	pclose(fp);

	if(*buffer == 0 ) return -1;
	*p_rssi = atoi(buffer);
	return 0;
	
}


int32_t sdk_wifi_mode_set(wifi_mode_e mode)
{
	system("killall wpa_supplicant");
	system("killall udhcpc");
	system("killall hostapd");
	system("killall udhcpd");
	switch (mode)
		{
			case MODE_RF_CLOSE:
						printf("wifi close\r\n");
						return -1;	 //目前没有实现关闭wifi
					break;
			case MODE_STA:
						system("wpa_supplicant -Dnl80211 -iwlan0 -c /etc/wpa_supplicant.conf &");
						sleep(1);
						system("udhcpc -i wlan0 &"); 
						return 0;
					break;
			case MODE_AP:
						system("hostapd /opt/conf/hostapd.conf &");
						sleep(1);
						system("ifconfig wlan0 192.168.200.1");
						sleep(1);
						system(" udhcpd /opt/conf/udhcpd.conf &"); 
						return 0;
					break;
			case MODE_AP_STA:
						printf("wifi mode erro\r\n");
						return -2;
					break;
			default:
						return -3; //mode 不支持
					break;
		}
}



/**
 * @brief    设置模块的工作模式STA、AP+STA下的AP参数
 * @param    [in] p_ap       wifi_ap_config_t类型，所要设置的参数数据
 * @return   int32_t        >= 0 成功   <0失败 不同值对应不 同的错误代码。
 * @note     调用此函数前确定模块是工作在AP、AP+STA模式下。
 */
int32_t sdk_wifi_ap_set(wifi_ap_config_t *p_ap)
{
	FILE *fp = NULL;
	int8_t buffer[128]={0}; 
	int8_t cmd[128]={0},i=0;
	
	
	system("killall wpa_supplicant");
	system("killall udhcpc");
	system("killall hostapd");
	system("killall udhcpd");
	
	system("cp /opt/conf/hostapd.conf_bak /opt/conf/hostapd.conf");
	
	fp=popen("sed -n -e \"/ssid/=\" /opt/conf/hostapd.conf", "r");
	fgets(buffer, sizeof(buffer), fp); 
	pclose(fp);
	sprintf(cmd, "sed  -i  \"%d  d\"   /opt/conf/hostapd.conf",atoi(buffer));
	system(cmd);
	sprintf(cmd, "sed -i 'N;%dassid=%s' /opt/conf/hostapd.conf ",atoi(buffer),p_ap->ssd.str_buf);
	system(cmd);
	
	fp=popen("sed -n -e \"/wpa_passphrase/=\" /opt/conf/hostapd.conf", "r");
	fgets(buffer, sizeof(buffer), fp); 
	pclose(fp);
	sprintf(cmd, "sed  -i  \"%d  d\"   /opt/conf/hostapd.conf",atoi(buffer));
	system(cmd);
	//sprintf(cmd, "sed -i 'N;%dawpa_passphrase=%s' /opt/conf/hostapd.conf",atoi(buffer)-1,p_ap->password.str_buf);
	sprintf(cmd, "sed -i 'N;6awpa_passphrase=%s' /opt/conf/hostapd.conf",p_ap->password.str_buf);
	printf("%s\n",cmd);
	system(cmd);
	
	
	system("hostapd /opt/conf/hostapd.conf &");
	sleep(1);
	system("ifconfig wlan0 192.168.200.1");
	sleep(1);
	system(" udhcpd /opt/conf/udhcpd.conf &"); 
	
	return 0;
}

/**
 * @brief    设置STA参数，模块工作在STA或AP+STA模式下有效
 * @param    [in] p_sta     wifi_sta_config_t类型，所要设置的参数数据
 * @return   int32_t        >= 0 成功   <0失败 不同值对应不 同的错误代码。
 * @note     调用此函数前确定模块是工作在STA或AP+STA模式下。
 */
int32_t sdk_wifi_sta_set(wifi_sta_config_t *p_sta)
{
	FILE *fp = NULL;
	int8_t buffer[128]={0}; 
	int8_t cmd[128]={0},i=0;
	
	system("killall wpa_supplicant");
	system("killall udhcpc");
	system("killall hostapd");
	system("killall udhcpd");
	
	system("cp /opt/conf/wpa_supplicant.conf /etc/");
	fp=popen("sed -n -e \"/ssid/=\" /etc/wpa_supplicant.conf", "r");
	fgets(buffer, sizeof(buffer), fp); 
	pclose(fp);
	sprintf(cmd, "sed  -i  \"%d  d\"   /etc/wpa_supplicant.conf",atoi(buffer));
	system(cmd);
	sprintf(cmd, "sed -i 'N;%dassid=\"%s\"' /etc/wpa_supplicant.conf ",atoi(buffer),p_sta->ssd.str_buf);
	system(cmd);
	
	fp=popen("sed -n -e \"/psk/=\" /etc/wpa_supplicant.conf", "r");
	fgets(buffer, sizeof(buffer), fp); 
	pclose(fp);
	sprintf(cmd, "sed  -i  \"%d  d\"   /etc/wpa_supplicant.conf",atoi(buffer));
	system(cmd);
	sprintf(cmd, "sed -i 'N;%dapsk=\"%s\"' /etc/wpa_supplicant.conf ",atoi(buffer)-1,p_sta->password.str_buf);
	system(cmd);
	
	
	system("wpa_supplicant -Dnl80211 -iwlan0 -c /etc/wpa_supplicant.conf &");
	sleep(1);
	system("udhcpc -i wlan0 &"); 
	
	return 0;
}

/**
 * @brief    获取周围热点名称
 * @param    [in] p_sta     指向wifi_sta_config_t类型的地址
 * @param    [in] num       热点数量
 * @return   int32_t        >= 0 成功   <0失败 不同值对应不 同的错误代码。
 * @note     调用此函数前确定模块是工作在STA或AP+STA模式下。
 */
int32_t sdk_wifi_hotspots(wifi_sta_config_t *p_sta,uint8_t num)
{
	FILE *fp = NULL;
	int8_t buffer[128]={0};
	char *tmp;
	int i = 0 ,j;
	char *p,*p2;
	char result[64] = {0};
	
	fp=popen("iwlist wlan0  scan | grep \"ESSID\"", "r");
	while(fgets(buffer, sizeof(buffer), fp))
	{
		p = strstr(buffer,"ESSID") + 7;
		p2 = strchr(p,'"');
		memcpy(result,p,p2 - p);
		//printf("ssid: %s\n",result);
		memcpy(p_sta[i].ssd.str_buf,result,strlen(result));
		p_sta[i].ssd.str_len = strlen(result);
		i++;
		if(i>=num) break;
	}
	pclose(fp);
	

	
	return 0;
}

/**
 * @brief    当做AP或AP+STA功能时，获取已连接sta的信息
 * @param    [in] p_sta     MyParamDoc
 * @return   int32_t        >= 0 成功 结果存放到 *p_sta中 <0失败 不同值对应不 同的错误代码。
 * @note     调用此函数前确定模块是工作在AP或AP+STA模式下。
 */
int32_t sdk_wifi_sta_info_get(wifi_connect_sta_list_t **p_sta)
{
	//iw dev wlan0 station dump
	
	FILE *fp = NULL;
	int8_t buffer[32]={0};


	fp=popen("iw dev wlan0 station dump | grep Station | awk '{print $2}'", "r");
	fgets(buffer, sizeof(buffer), fp); 
	pclose(fp);

	if(*buffer == 0 ) return -1;
    memcpy(p_sta,buffer,sizeof(buffer));
	return 0;
}

/**
 * @brief    Wi-Fi事件回调注册
 * @param    [in] event     Wi-Fi事件类型
 * @param    [in] event_cb  事件回调函数指针
 * @return   int32_t >= 0 成功   <0失败 不同值对应不 同的错误代码。
 * @warning  禁止注册回调包含阻塞操作
 */
int32_t sdk_wifi_evt_regist(wifi_evnet_e event, p_wifi_event_cb event_cb)
{
	return -1;
}

/*
void main()
{
	int16_t p_rssi=0;
	uint8_t num=10;
	wifi_sta_config_t sta[32];
	wifi_sta_config_t p_sta;
	wifi_ap_config_t p_ap;
	
	memcpy(p_sta.password.str_buf,"dst888999",9);
	memcpy(p_sta.ssd.str_buf,"iPhone",10);
	
	memcpy(p_ap.password.str_buf,"123456789",9);
	memcpy(p_ap.ssd.str_buf,"LEO5",10);
	
	sdk_wifi_rssi_get(&p_rssi);
	sdk_wifi_mode_set(MODE_AP_STA);
	sdk_wifi_hotspots(sta ,num);
	printf("%d\n",num);
	while(num--)
	{
		printf("------%s\n",sta[num].ssd.str_buf);
	}
	sdk_wifi_sta_set(&p_sta);
	sleep(30);
	sdk_wifi_ap_set(&p_ap);
	
}
*/
