#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#include "stdint.h"

#include "sdk_net_cellular.h"



int main()
{
	cellular_net_t apn_config_t;
	int16_t tmp;
	int8_t p_iccid[30]={0},p_imei[20]={0};
	int8_t len=20;
	
	
	printf("拨号蜂窝4G\r\n");
	sdk_cellular_net_start();
	printf("默认成功\r\n");
	printf("sdk_cellular_net_apn_set 暂不支持设置模块工作信息\r\n");
	sdk_cellular_net_apn_set(&apn_config_t);
	printf("获取蜂窝的信号强度默认返回%d,信号强度 %d\r\n",sdk_cellular_net_rssi_get(&tmp),tmp);
	printf("获取sim卡的ccid 默认返回0\r\n");
	sdk_cellular_sim_ccid_get(p_iccid,len);
	printf("sim p_iccid = %s,获取len = %d \n",p_iccid,len);
	printf("获取sim卡的状态 sim status is 返回 %d \n",sdk_cellular_sim_status_get());
	printf("获取模块的imei 默认返回0\n");
	sdk_cellular_imei_get(p_imei,15);
	printf("module_imei is %s \n",p_imei);
	printf("获取模块联网状态\n");
	printf("module_net_status is %d \n",sdk_cellular_net_status_get());
	printf("获取模块联网erro原因 如果模块已经连上网，将返回其他错误\n");
	printf("module_net_error_code is %d \n",sdk_cellular_net_error_code_get());
	
	return 0;
}
