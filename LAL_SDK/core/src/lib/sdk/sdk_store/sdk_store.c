/************************************************************************************
 *Description: sdk_store
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

#include "sdk_store.h"

#include "malloc.h"


int32_t  sdk_store_path_get(store_device_type_e type,int8_t *p_path, uint32_t len)
{
	FILE *fp = NULL;
	char buffer[128]={0};
	char buffer2[8]={0};
	int8_t cmd[128]={0};

	if(len>128||len<0) return -2; //len参数错误
	//if(_msize(p_path)<len) return -2; //p_path参数错误

	switch(type)
	{
		case USB_FLSH:
			 sprintf(cmd, "ls /media/sd*");
			 break;
		case TF_CARD:
			 sprintf(cmd, "ls /media/mm*");
			 break;
		
		default: 
			printf("type error\n");
			return -2;
			break; 
	}
	
	fp=popen(cmd, "r");
	fgets(buffer, sizeof(buffer), fp);
	pclose(fp);
	if(memcmp(buffer, buffer2, 8)==0)
	{
		printf(" device is no exit !!!\n");
		return -1;
	}
	else
	{
		switch(type)
		{
			case USB_FLSH:
			fp=popen("find /media/sd*", "r");
			fgets(buffer, sizeof(buffer), fp);
			pclose(fp);
			break;
			case TF_CARD:
			fp=popen("find /media/mm*", "r");
			fgets(buffer, sizeof(buffer), fp);
			pclose(fp);
			break;
			default: break;
		}
		
		memcpy(p_path,buffer,len);
		return 0;
	}
	
	
}


int32_t  sdk_store_is_exist(store_device_type_e type)
{
	return  sdk_store_path_get(type,NULL,0);
}
