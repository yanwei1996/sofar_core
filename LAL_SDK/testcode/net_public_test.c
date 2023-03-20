
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

void  main()
{
	uint8_t   p_ip[20]={0},p_subnet[20]={0},p_gateway[20]={0},p_dns[20]={0},p_mac[20]={0};

	printf("sdk_net_ip_get\r\n");
	sdk_net_ip_get(ETH0,p_ip);
	printf("sdk_net_ip_get is %s", p_ip);
	
	printf("sdk_net_subnetmask_get\r\n");
	sdk_net_subnetmask_get( ETH1, p_subnet) ;
	printf("sdk_net_subnetmask_get is %s", p_subnet);
	
	printf("sdk_net_ip_set\r\n");
	sdk_net_ip_set(ETH1,"192.168.2.100");
	
	printf("sdk_net_subnetmask_set\r\n");
	sdk_net_subnetmask_set(ETH1,"255.255.0.0");
	
	printf("sdk_net_gateway_set\r\n");
	sdk_net_gateway_set(ETH1,"192.168.2.4");
	
	printf("sdk_net_gateway_get\r\n");
	sdk_net_gateway_get(ETH1,p_gateway);
	printf("sdk_net_gateway_get is %s", p_gateway);
	
	printf("sdk_net_dns_set\r\n");
	sdk_net_dns_set(ETH1,"4.4.4.4");
	
	printf("sdk_net_dns_get\r\n");
	sdk_net_dns_get(ETH1,p_dns);
	printf("sdk_net_DNS_get is %s", p_dns);
	
	printf("sdk_net_mac_set\r\n");
	sdk_net_mac_set(ETH1,"00:0c:29:c0:af:88");
	
	printf("sdk_net_mac_get\r\n");
	sdk_net_mac_get(ETH1,p_mac);
	printf("sdk_net_MAC_get is %s", p_mac);
	printf("sdk_net_type_get %d\n",sdk_net_type_get());
	
}
