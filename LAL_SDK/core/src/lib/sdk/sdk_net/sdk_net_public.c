/************************************************************************************
 *Description: sdk_net_public
 *Created on: 2023-2-16
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

#include "sdk_net_public.h"


//获取指定网卡的IP地址 
int32_t sdk_net_ip_get  (  net_device_type_e  nic,  uint8_t *  p_ip   ) 
{
	FILE *fp = NULL;
	char buffer[32]={0};
	char buffer2[32]={0};
	int8_t cmd[64]={0};
	


	switch(nic) 
	{ 
		case WLAN0:  
			   sprintf(cmd, "ifconfig %s | grep \"inet \" | awk '{print $2}'", "wlan0");
			   break; 
		case WLAN1:  
			   sprintf(cmd, "ifconfig %s | grep \"inet \" | awk '{print $2}'", "wlan1");
			   break; 
		case ETH0: 
			   sprintf(cmd, "ifconfig %s | grep \"inet \" | awk '{print $2}'", "eth0");
			   break; 
		case ETH1:  
			   sprintf(cmd, "ifconfig %s | grep \"inet \" | awk '{print $2}'", "eth1");
			   break;
		case PPP0:  
			   sprintf(cmd, "ifconfig %s | grep \"inet \" | awk '{print $2}'", "usb0");
			   break;

		default: 
			printf("input error\n");
			return -2;
			break; 
	}
	fp=popen(cmd, "r");
	fgets(buffer, sizeof(buffer), fp);
	if(memcmp(buffer, buffer2, sizeof(buffer))==0)
	{
		printf("SDK erro: this net device is no exit !!!\n");
		return -1;
	}
	memcpy(p_ip ,buffer+5,sizeof(buffer)-5);
	pclose(fp);
	return 0;
}


//set指定网卡的IP地址 
int32_t sdk_net_ip_set  ( net_device_type_e  nic,  uint8_t *  p_ip   ) 
{
	FILE *fp = NULL;
	char buffer[32]={0};
	int8_t cmd[128]={0};
	int8_t cmd2[64]={0};


	switch(nic) 
	{ 
		case WLAN0:  
			   sprintf(cmd, "ifconfig %s %s", "wlan0",p_ip );
			   break; 
		case WLAN1:  
			   sprintf(cmd, "ifconfig %s %s", "wlan1",p_ip );
			   break; 
		case ETH0: 
			   sprintf(cmd, "ifconfig %s %s", "eth0",p_ip );
			   break; 
		case ETH1:  
			   sprintf(cmd, "ifconfig %s %s", "eth1",p_ip );
			   break;
		case PPP0:  
			   sprintf(cmd, "ifconfig %s %s", "usb0",p_ip );
			   break;

		default: 
			printf("SDK input error\n");
			return -2;
			break; 
	}
	fp=popen(cmd, "r");
	pclose(fp);
	sprintf(cmd2, "echo %s> /etc/init.d/Net_sdk_set/sdk_net_ip_set.sh", cmd );
	fp=popen(cmd2, "r");
	pclose(fp);
	return 0;
}


//------------------------------fail
//获取指定网卡的子网掩码地址 
int32_t sdk_net_subnetmask_get  ( uint32_t  nic, uint8_t *  p_subnet) 
{
	FILE *fp = NULL;
	char buffer[32]={0};
	char buffer2[32]={0};
	int8_t cmd[64]={0}; //减小内存
	


	switch(nic) 
	{ 
		case WLAN0:  
			   sprintf(cmd, "ifconfig %s | grep \"inet \" | awk '{print $4}'", "wlan0");
			   break; 
		case WLAN1:  
			   sprintf(cmd, "ifconfig %s | grep \"inet \" | awk '{print $4}'", "wlan1");
			   break; 
		case ETH0: 
			   sprintf(cmd, "ifconfig %s | grep \"inet \" | awk '{print $4}'", "eth0");
			   break; 
		case ETH1:  
			   sprintf(cmd, "ifconfig %s | grep \"inet \" | awk '{print $4}'", "eth1");
			   break;
		case PPP0:  
			   sprintf(cmd, "ifconfig %s | grep \"inet \" | awk '{print $4}'", "usb0");//最好是用宏定义
			   break;

		default: 
			printf("input error\n");//最好是减少打印
			return -2;
			break; 
	}
	fp=popen(cmd, "r");
	fgets(buffer, sizeof(buffer), fp);
	if(memcmp(buffer, buffer2, sizeof(buffer))==0)
	{
		printf("SDK erro: this net device is no exit !!!\n");
		return -1;
	}
	memcpy(p_subnet,buffer+5,sizeof(buffer)-5);
	pclose(fp);
	return 0;
}
//--------------------------------------ok
//设置指定网卡的子网掩码地址 
int32_t sdk_net_subnetmask_set ( net_device_type_e nic, uint8_t *  p_subnet  ) 
{
	FILE *fp = NULL;
	char buffer[32]={0};
	int8_t cmd[64]={0};
	int8_t cmd2[64]={0};
	

	switch(nic) 
	{ 
		case WLAN0:  
			   sprintf(cmd, "ifconfig %s netmask %s", "wlan0",p_subnet );
			   break; 
		case WLAN1:  
			   sprintf(cmd, "ifconfig %s netmask %s", "wlan1",p_subnet );
			   break; 
		case ETH0: 
			   sprintf(cmd, "ifconfig %s netmask %s", "eth0",p_subnet );
			   break; 
		case ETH1:  
			   sprintf(cmd, "ifconfig %s netmask %s", "eth1",p_subnet );
			   break;
		case PPP0:  
			   sprintf(cmd, "ifconfig %s netmask %s", "usb0",p_subnet );
			   break;

		default: 
			printf("SDK input error\n");
			return -2;
			break; 
	}
	fp=popen(cmd, "r");
	pclose(fp);
	sprintf(cmd2, "echo %s> /etc/init.d/Net_sdk_set/sdk_net_subnetmask_set.sh", cmd );
	fp=popen(cmd2, "r");
	pclose(fp);
	return 0;
}


//------------------------------ok
//get 指定网卡的网关地址
int32_t  sdk_net_gateway_get ( net_device_type_e nic, uint8_t *p_gateway) 
{
	FILE *fp = NULL;
	char buffer[32]={0};
	char buffer2[32]={0};
	int8_t cmd[64]={0};
	


	switch(nic) 
	{ 
		case WLAN0:  
			   sprintf(cmd, "ip route show|grep %s|grep \"default via\" | awk '{print $3}'", "wlan0");
			   break; 
		case WLAN1:  
			   sprintf(cmd, "ip route show|grep %s|grep \"default via\" | awk '{print $3}'", "wlan1");
			   break; 
		case ETH0: 
			   sprintf(cmd, "ip route show|grep %s|grep \"default via\" | awk '{print $3}'", "eth0");
			   break; 
		case ETH1:  
			   sprintf(cmd, "ip route show|grep %s|grep \"default via\" | awk '{print $3}'", "eth1");
			   break;
		case PPP0:  
			   sprintf(cmd, "ip route show|grep %s|grep \"default via\" | awk '{print $3}'", "usb0");
			   break;

		default: 
			printf("SDK input error\n");
			return -2;
			break; 
	}
	fp=popen(cmd, "r");
	fgets(buffer, sizeof(buffer), fp);
	if(memcmp(buffer, buffer2, sizeof(buffer))==0)
	{
		printf("SDK erro: this net device is no exit !!!\n");
		return -1;
	}
	memcpy(p_gateway,buffer,sizeof(buffer));
	pclose(fp);
	return 0;
}

//--------------------------------ok
//set 指定网卡的网关地址
int32_t  sdk_net_gateway_set ( net_device_type_e nic, uint8_t *p_gateway) 
{
	FILE *fp = NULL;
	char buffer[32]={0};
	int8_t cmd[64]={0};
	int8_t cmd2[64]={0};
	


	switch(nic) 
	{ 
		case WLAN0:  
			   sprintf(cmd, "route add default gw %s dev %s",p_gateway,"wlan0");
			   break; 
		case WLAN1:  
			   sprintf(cmd, "route add default gw %s dev %s",p_gateway,"wlan1");
			   break; 
		case ETH0: 
			   sprintf(cmd, "route add default gw %s dev %s",p_gateway,"eth0");
			   break; 
		case ETH1:  
			   sprintf(cmd, "route add default gw %s dev %s",p_gateway,"eth1");
			   break;
		case PPP0:  
			   sprintf(cmd, "route add default gw %s dev %s",p_gateway,"usb0");
			   break;

		default: 
			printf("SDK input error\n");
			return -2;
			break; 
	}
	fp=popen(cmd, "r");
	pclose(fp);
	sprintf(cmd2, "echo %s> /etc/init.d/Net_sdk_set/sdk_net_gateway_set.sh", cmd );
	fp=popen(cmd2, "r");
	pclose(fp);
	return 0;

	
}

//---------------------------------ok
//set dns
int32_t  sdk_net_dns_set ( net_device_type_e nic, uint8_t *p_dns)
{
	FILE *fp = NULL;
	char buffer[32]={0};
	int8_t cmd[64]={0};
	int8_t cmd2[64]={0};
	


	switch(nic) 
	{ 
		case WLAN0:  
			   sprintf(cmd, "echo nameserver %s > /etc/resolv.conf",p_dns);
			   break; 
		case WLAN1:  
			   sprintf(cmd, "echo nameserver %s > /etc/resolv.conf",p_dns);
			   break; 
		case ETH0: 
			   sprintf(cmd, "echo nameserver %s > /etc/resolv.conf",p_dns);
			   break; 
		case ETH1:  
			   sprintf(cmd, "echo nameserver %s > /etc/resolv.conf",p_dns);
			   break;
		case PPP0:  
			   sprintf(cmd, "echo nameserver %s > /etc/resolv.conf",p_dns);
			   break;

		default: 
			printf("SDK input error\n");
			return -2;
			break; 
	}
	fp=popen(cmd, "r");
	pclose(fp);
	sprintf(cmd2, "echo \"%s\"> /etc/init.d/Net_sdk_set/sdk_net_dns_set.sh", cmd );
	fp=popen(cmd2, "r");
	pclose(fp);
	return 0;
}

//----------------------------------------ok
//get nic dns
int32_t  sdk_net_dns_get ( net_device_type_e nic, uint8_t *p_dns) 
{
	FILE *fp = NULL;
	char buffer[32]={0};
	char buffer2[32]={0};
	int8_t cmd[64]={0};
	


	switch(nic) 
	{ 
		case WLAN0:  
			   sprintf(cmd, "cat /etc/resolv.conf |grep nameserver|awk '{print $2}'");
			   break; 
		case WLAN1:  
			   sprintf(cmd, "cat /etc/resolv.conf |grep nameserver|awk '{print $2}'");
			   break; 
		case ETH0: 
			   sprintf(cmd, "cat /etc/resolv.conf |grep nameserver|awk '{print $2}'");
			   break; 
		case ETH1:  
			   sprintf(cmd, "cat /etc/resolv.conf |grep nameserver|awk '{print $2}'");
			   break;
		case PPP0:  
			   sprintf(cmd, "cat /etc/resolv.conf |grep nameserver|awk '{print $2}'");
			   break;

		default: 
			printf("SDK input error\n");
			return -2;
			break; 
	}
	fp=popen(cmd, "r");
	fgets(buffer, sizeof(buffer), fp);
	if(memcmp(buffer, buffer2, sizeof(buffer))==0)
	{
		printf("SDK erro: this net device is no exit !!!\n");
		pclose(fp);
		return -1;
	}
	memcpy(p_dns,buffer,sizeof(buffer));
	pclose(fp);
	return 0;
}


int32_t  sdk_net_mac_set ( net_device_type_e nic, uint8_t *p_mac) 
{
	
	FILE *fp = NULL;
	char buffer[32]={0};
	int8_t cmd[64]={0};
	int8_t cmd2[64]={0};
	


	switch(nic) 
	{ 
		case WLAN0:  
			   sprintf(cmd, "ip link set dev %s address %s","wlan0",p_mac);
			   break; 
		case WLAN1:  
			   sprintf(cmd, "ip link set dev %s address %s","wlan1",p_mac);
			   break; 
		case ETH0: 
			   sprintf(cmd, "ip link set dev %s address %s","eth0",p_mac);
			   break; 
		case ETH1:  
			   sprintf(cmd, "ip link set dev %s address %s","eth1",p_mac);
			   break;
		case PPP0:  
			   sprintf(cmd, "ip link set dev %s address %s","usb0",p_mac);
			   break;

		default: 
			printf("SDK input error\n");
			return -2;
			break; 
	}
	fp=popen(cmd, "r");
	pclose(fp);
	sprintf(cmd2, "echo %s> /etc/init.d/Net_sdk_set/sdk_net_mac_set.sh", cmd );
	fp=popen(cmd2, "r");
	pclose(fp);
	return 0;
}
//----------------------------ok
int32_t  sdk_net_mac_get ( net_device_type_e nic, uint8_t *p_mac) 
{
	FILE *fp = NULL;
	char buffer[32]={0};
	char buffer2[32]={0};
	int8_t cmd[64]={0};
	


	switch(nic) 
	{ 
		case WLAN0:  
			   sprintf(cmd, "ifconfig %s|grep HWaddr|awk '{print $5}'", "wlan0");
			   break; 
		case WLAN1:  
			   sprintf(cmd, "ifconfig %s|grep HWaddr|awk '{print $5}'", "wlan1");
			   break; 
		case ETH0: 
			   sprintf(cmd, "ifconfig %s|grep HWaddr|awk '{print $5}'", "eth0");
			   break; 
		case ETH1:  
			   sprintf(cmd, "ifconfig %s|grep HWaddr|awk '{print $5}'", "eth1");
			   break;
		case PPP0:  
			   sprintf(cmd, "ifconfig %s|grep HWaddr|awk '{print $5}'", "usb0");
			   break;

		default: 
			printf("SDK input error\n");
			return -2;
			break; 
	}
	fp=popen(cmd, "r");
	fgets(buffer, sizeof(buffer), fp);
	if(memcmp(buffer, buffer2, sizeof(buffer))==0)
	{
		printf("SDK erro: this net device is no exit !!!\n");
		return -1;
	}
	memcpy(p_mac,buffer,sizeof(buffer));
	pclose(fp);
	return 0;
}

//--------------------ok
//获取当前联网类型，如有线、WIFI、蜂窝网
int32_t sdk_net_type_get  (void) 
{
	FILE *fp = NULL;
	char buffer[32]={0};
	char buffer2[32]={0};
	int8_t cmd[64]={0},i=0;
	
	sprintf(cmd, "ip route show | sed -e 's/^.*dev \\([^ ]*\\).*$/\\1/' | head -n 1");
	fp=popen(cmd, "r");
	fgets(buffer, sizeof(buffer), fp);
	pclose(fp);
	if(memcmp(buffer, buffer2, sizeof(buffer))==0)
	{
		printf("SDK erro: get net_type fail !!!\n");
		return -1;
	}
	if(memcmp(buffer, "eth0", 5)==0)
	{
		return 1;//eth0
	}
	if(memcmp(buffer, "eth1", 4)==0)
	{
	return 2;//eth1
	}
	if(memcmp(buffer, "wlan0", 5)==0)
	{
	return 3;//wlan0
	}
	if(memcmp(buffer, "wlan1", 5)==0)
	{
		return 4;//wlan1
	}
	if(memcmp(buffer, "usb0", 4)==0)
	{
		return 5;//usb0
	}
	
	return 0;//other type
}




/*
touch /etc/init.d/Net_sdk_set/sdk_net_dns_set.sh
touch /etc/init.d/Net_sdk_set/sdk_net_gateway_set.sh
touch /etc/init.d/Net_sdk_set/sdk_net_ip_set.sh
touch /etc/init.d/Net_sdk_set/sdk_net_mac_set.sh
touch /etc/init.d/Net_sdk_set/sdk_net_subnetmask_set.sh
 
 */



/*
void  main()
{
	uint8_t   p_ip[20]={0},p_subnet[20]={0},p_gateway[20]={0},p_dns[20]={0},p_mac[20]={0};

	printf("my test\r\n");
	sdk_net_ip_get(ETH0,p_ip);
	printf("sdk_net_ip_get is %s", p_ip);
	sdk_net_subnetmask_get  ( ETH0, p_subnet) ;
	printf("sdk_net_subnetmask_get is %s", p_subnet);
	sdk_net_ip_set(ETH0,"192.168.1.100");
	sdk_net_subnetmask_set(ETH0,"255.255.255.0");
	sdk_net_gateway_set(ETH0,"192.168.1.4");
	sdk_net_gateway_get(ETH0,p_gateway);
	printf("sdk_net_gateway_get is %s", p_gateway);
	sdk_net_dns_set(ETH0,"4.4.4.4");
	sdk_net_dns_get(ETH0,p_dns);
	printf("sdk_net_DNS_get is %s", p_dns);
	sdk_net_mac_set(ETH0,"00:0c:29:c0:af:88");
	sdk_net_mac_get(ETH0,p_mac);
	printf("sdk_net_MAC_get is %s", p_mac);
	
	printf("sdk_net_type_get %d\n",sdk_net_type_get());
	
}
*/




