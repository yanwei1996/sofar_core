
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#include "stdint.h"

#include "sdk_upgrade.h"

void main()
{
	char path[64]={0};
	printf("输入升级文件绝对路径:\n");
	scanf("%s",path);
	printf("输入升级文件绝对路径:%s\n",path); 
	
	printf("sdk_start_upgrade status %d\n",sdk_start_upgrade_form_package(path,UT_CORE));
	printf("sdk_upgrade_status_get status %d\n",sdk_upgrade_status_get(UT_CORE));
	
	printf("sdk_start_upgrade status %d\n",sdk_start_upgrade_form_package(path,UT_APP));
	printf("sdk_upgrade_status_get status %d\n",sdk_upgrade_status_get(UT_APP));
	
	
	printf("输入升级文件绝对路径:\n");
	scanf("%s",path);
	printf("输入升级文件绝对路径:%s\n",path); 
	printf("sdk_start_upgrade status %d\n",sdk_start_upgrade(path,UT_CORE));
	printf("sdk_upgrade_status_get status %d\n",sdk_upgrade_status_get(UT_CORE));
	printf("输入升级文件绝对路径:\n");
	scanf("%s",path);
	printf("输入升级文件绝对路径:%s\n",path); 
	printf("sdk_start_upgrade status %d\n",sdk_start_upgrade(path,UT_APP));
	printf("sdk_upgrade_status_get status %d\n",sdk_upgrade_status_get(UT_APP));
	
   
}



