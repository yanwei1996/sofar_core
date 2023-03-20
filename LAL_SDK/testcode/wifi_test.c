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

void main()
{
	int16_t rssi=-100;
	uint8_t num=10;
	wifi_sta_config_t sta[32];
	wifi_sta_config_t p_sta;
	wifi_ap_config_t p_ap;
	wifi_connect_sta_list_t Sta;
	
	
	
	
	printf("启动wifi工作模式: MODE_AP_STA\r\n");
	sdk_wifi_mode_set(MODE_AP_STA);
	printf("暂不支持MODE_AP_STA\r\n");
	
	printf("启动wifi工作模式: MODE_STA\r\n");
	sdk_wifi_mode_set(MODE_STA);
	printf("default succeed\n");
	printf("获取周围%d个热点名称\r\n",num);
	sdk_wifi_hotspots(sta ,num);
	printf("default succeed\n");
	while(num--)
	{
		printf("热点名称------%s\n",sta[num].ssd.str_buf);
	}
	printf("default succeed\n");
	printf("设置模块STA工作模式的参数\n");
	memcpy(p_sta.password.str_buf,"dst888999",9);
	memcpy(p_sta.ssd.str_buf,"iPhone",10);
	printf("要接入的 STA name: %s\r\n",p_sta.ssd.str_buf);
	printf("要接入的 STA passwd: %s\r\n",p_sta.password.str_buf);
	sdk_wifi_sta_set(&p_sta);
	printf("default succeed\nIf the ping fails, check the routing table\n");
	
	
	printf("启动wifi工作模式: MODE_AP\r\n");
	sdk_wifi_mode_set(MODE_AP);
	printf("default succeed\n");
	printf("设置模块AP工作模式的参数\n");
	memcpy(p_ap.password.str_buf,"123456789",9);
	memcpy(p_ap.ssd.str_buf,"LEO5",10);
	printf("要接出的 AP name: %s\r\n",p_ap.ssd.str_buf);
	printf("要接出的 AP passwd: %s\r\n",p_ap.password.str_buf);
	sdk_wifi_ap_set(&p_ap);
	printf("default succeed\nIf the ping fails, check the routing table\n");
	
	printf("获取已连接sta的信息返回%d,   %s\r\n",sdk_wifi_sta_info_get(&(Sta.sta_info[0])),&(Sta.sta_info[0])); 
	printf("暂不支持Wi-Fi事件回调注册\r\n"); 
	printf("获取WIFI接收信号强度:retun %d,rssi = %d\n",sdk_wifi_rssi_get(&rssi),rssi);
	
}
