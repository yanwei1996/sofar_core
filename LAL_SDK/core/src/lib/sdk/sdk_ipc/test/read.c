#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "sdk_ipc.h"
#include "data_shm.h"

int main()
{
	commem_data_t *shm = shmdata();
	if(!shm)
	{
		perror("read create shmdata failed");
		return 0;
	}
	
	while(1)
	{
		sleep(1);
		printf("ipc-read: %d\n", shm->ipcTestInt);
		printf("ipc-read: %s\n", shm->ipcTestTtr);
	}
		
}

//arm-linux-gnueabihf-gcc read.c -I../../../include -I/home/work/system/I.MX6ULL/ZLG-M6Y2C/buildroot-2020.02.1/output/staging/usr/include  --sysroot=/home/work/system/I.MX6ULL/ZLG-M6Y2C/buildroot-2020.02.1/output/staging/ -L../../ -l_sdk -lsqlite3 -o ipc_read

//arm-linux-gnueabihf-gcc read.c -I../../../include -L../../ -l_sdk  -o ipc_read

//gcc read.c ../sdk_ipc.c  -o ipc_read -I ../../../include/
