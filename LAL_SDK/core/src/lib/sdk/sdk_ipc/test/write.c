#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "sdk_ipc.h"
#include "data_shm.h"

int main(int argc, char *argv[])
{
	commem_data_t *shm = shmdata();
	if(!shm)
	{
		perror("read create shmdata failed");
		return -1;
	}
	
	while(1)
	{
		printf("ipc-write: %d\n", shm->ipcTestInt++);
		if(shm->ipcTestInt%2==1)
		{
			memcpy(shm->ipcTestTtr, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 32);
		}
		else
		{
			memcpy(shm->ipcTestTtr, "bbbbbbbbbbbbbbbbbbbbbbbbbbbbb", 32);
		}
		printf("ipc-write: %s\n", shm->ipcTestTtr);
		sleep(1);
	}
	
	
	return 0;
		
}

//gcc write.c ../lib_sdk_ipc.a -o write
//arm-linux-gnueabihf-gcc write.c -I../../../include -I/home/work/system/I.MX6ULL/ZLG-M6Y2C/buildroot-2020.02.1/output/staging/usr/include  --sysroot=/home/work/system/I.MX6ULL/ZLG-M6Y2C/buildroot-2020.02.1/output/staging/ -L../../ -l_sdk -lsqlite3 -o ipc_write