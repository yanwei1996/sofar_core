/************************************************************************************
 *Description: sdk_net_4G
 *Created on: 2023-2-28
 *Author: yanwei
 ************************************************************************************/

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



int32_t sdk_cellular_net_apn_set( cellular_net_t *p_apn)
{
	//strcpy(p_apn->user_name, "C Programming");
	//printf("%s\r\n",p_apn->user_name);
	return 0;
}

int32_t sdk_cellular_net_start(void)
{
	//system("echo 137 > /sys/class/gpio/export ");
	//system("echo out > /sys/class/gpio/gpio137/direction");
	//system("echo 1 > /sys/class/gpio/gpio137/value");
	//printf("hardware open\r\n");
	
	system("cat /dev/ttyUSB1 > tmp &");
	sleep(1);
	//system("echo -e \"AT+QCFG=\"usbnet\"\\r\\n\" > /dev/ttyUSB1 ");
	//sleep(1);
	//system("echo -e \"AT+COPS?\\r\\n\" > /dev/ttyUSB1 ");
	//sleep(1);
	//system("echo -e \"AT+CPIN?\\r\\n\" > /dev/ttyUSB1 ");
	//sleep(1);
	//system("echo -e \"AT+CFUN=0\\r\\n\" > /dev/ttyUSB1 ");
	//sleep(1);
	//system("echo -e \"AT+CFUN=1\\r\\n\" > /dev/ttyUSB1 ");
	//sleep(1);
	system("echo -e \"AT+QNETDEVCTL=1,1,1\\r\\n\" > /dev/ttyUSB1");
	sleep(5);
	printf("4G set\r\n");
	//system("ifconfig usb0 up ");
	//sleep(1);
	//system("udhcpc -i usb0 -fnq ");
	//printf("net set\r\n");
	return 0;
}

int32_t sdk_cellular_net_rssi_get(int16_t *rssi)
{
	FILE *fp = NULL;
	char buffer[32]={0};
	
	system("cat /dev/ttyUSB1 > tmp &");
	sleep(1);
	system("echo -e \"AT+CSQ\\r\\n\" > /dev/ttyUSB1");
	sleep(1);
	system("killall cat /dev/ttyUSB1");
	
	fp=popen("cat tmp | grep \"+CSQ:\" | awk '{print $2}'", "r");
	fgets(buffer, sizeof(buffer), fp); 
	
	
	pclose(fp);
	
	
	*rssi = atoi(strtok( buffer, "," ));
	return 0;
}


int32_t sdk_cellular_sim_ccid_get(int8_t *p_iccid,int8_t len)
{
	FILE *fp = NULL;
	int8_t buffer[32]={0};

	if(len > 32||len < 0) return -2;//len参数错误
	
	system("cat /dev/ttyUSB1 > tmp &");
	sleep(1);
	system("echo -e \"AT+QCCID\\r\\n\" > /dev/ttyUSB1");
	sleep(1);
	system("killall cat /dev/ttyUSB1");
	fp=popen("cat tmp | grep \"+QCCID:\" | awk '{print $2}'", "r");
	fgets(buffer, len, fp); 
	pclose(fp);
	//printf("%s\n",buffer);
	memcpy(p_iccid ,buffer,len);
	return 0;
	
}

int32_t sdk_cellular_sim_status_get(void)
{
	FILE *fp = NULL;
	int8_t buffer[20]={0};
	
	system("cat /dev/ttyUSB1 > tmp &");
	sleep(1);
	system("echo -e \"AT+CPIN?\\r\\n\" > /dev/ttyUSB1");
	sleep(1);
	system("killall cat /dev/ttyUSB1");
	
	fp=popen("cat tmp | grep \"+CPIN:\" | awk '{print $2}'", "r");
	fgets(buffer, sizeof(buffer), fp); 
	pclose(fp);
	
	if(memcmp(buffer, "READY", 5)==0)
	{
		//printf("sim OK\n");
		
		system("cat /dev/ttyUSB1 > tmp &");
		sleep(1);
		system("echo -e \"AT+CREG?\\r\\n\" > /dev/ttyUSB1");
		sleep(1);
		system("killall cat /dev/ttyUSB1");
		
		fp=popen("cat tmp | grep \"+CREG:\" | awk '{print $2}'", "r");
		fgets(buffer, sizeof(buffer), fp); 
		pclose(fp);
		
		strtok( buffer, "," );
		//printf("sim net is %d\n",atoi(strtok( NULL, "," )));
		return atoi(strtok( NULL, "," ));
		
	}
	else
	{
		//printf("sim not exit\n");
		return -1;
	}
}


int32_t sdk_cellular_imei_get(int8_t *p_imei,uint32_t len)
{
	FILE *fp = NULL;
	int8_t buffer[32]={0};

	if(len > 32||len < 0) return -2;//len参数错误
	
	system("cat /dev/ttyUSB1 > tmp &");
	sleep(1);
	system("echo -e \"AT+CGSN\\r\\n\" > /dev/ttyUSB1");
	sleep(1);
	system("killall cat /dev/ttyUSB1");
	
	fp=popen("cat tmp | sed -n 2p", "r");
	fgets(buffer, sizeof(buffer), fp); 
	pclose(fp);
	
	//printf("%s\n",buffer);
	memcpy(p_imei ,buffer,len);
	return 0;
}

int32_t sdk_cellular_net_status_get(void)
{
	
	FILE *fp = NULL;
	int8_t buffer[32]={0};
	
	system("cat /dev/ttyUSB1 > tmp &");
	sleep(1);
	system("echo -e \"AT+QNETDEVCTL?\\r\\n\" > /dev/ttyUSB1");
	sleep(1);
	system("killall cat /dev/ttyUSB1");
	
	fp=popen("cat tmp | grep \"+QNETDEVCTL:\" | awk '{print $2}'", "r");
	fgets(buffer, sizeof(buffer), fp); 
	pclose(fp);
	
	//printf("%s\n",buffer);
	strtok( buffer, "," );
	strtok( NULL, "," );
	strtok( NULL, "," );
	return atoi(strtok( NULL, "," ));
}

int32_t sdk_cellular_net_error_code_get(void)
{
	if( sdk_cellular_sim_status_get() == -1 )
	{
		printf("sim not exit!\n");
		return -1;
	}
	if( sdk_cellular_net_status_get() == 0 )
	{
		printf("ppp fail!\n");
		return -2;
	}
	
	printf("others erro\n");
	return -3;
}

/*
void main()
{
	cellular_net_t apn_config_t;
	int16_t tmp;
	int8_t p_iccid[30]={0},p_imei[20]={0};
	int8_t len=20;
	sdk_cellular_net_start();
	sdk_cellular_net_apn_set(&apn_config_t);
	sdk_cellular_net_rssi_get(&tmp);
	printf("4G rssi = %d\n",tmp);
	sdk_cellular_sim_ccid_get(p_iccid,len);
	printf("sim p_iccid = %s,len = %d \n",p_iccid,len);
	printf("sim status is %d \n",sdk_cellular_sim_status_get());
	sdk_cellular_imei_get(p_imei,15);
	printf("module_imei is %s \n",p_imei);
	printf("module_net_status is %d \n",sdk_cellular_net_status_get());
	printf("module_net_error_code is %d \n",sdk_cellular_net_error_code_get());
}
*/
