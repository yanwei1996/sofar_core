
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

int  main()
{
	int8_t path[20]={'n','o'};
	
	printf("检测是否存在U盘: %d\n",sdk_store_is_exist(USB_FLSH));
	
	printf("检测U盘PATH: %d,  %s\n",sdk_store_path_get(USB_FLSH,path, 20),path);
	
	printf("\n---------------\n");
	
	printf("检测是否存在SD: %d\n",sdk_store_is_exist(TF_CARD));
	
	printf("检测U盘PATH: %d,  %s\n",sdk_store_path_get(TF_CARD,path, 20),path);
	
	
	return 0;
}
