/************************************************************************************
 *Project: Inverter-320kw-ARM
 *Description: update_server
 *Created on: 2022-08-08
 *Author: quliuliu
 ************************************************************************************/
#include "globa.h"

#include "update_file_receive.h"
#include "update_file_check.h"


//定义commem_data_t类型的全局变量
commem_data_t *shm=NULL;


int main(int argc, char *argv[])
{
	//映射共享内存
	shm = shmdata();
	if(!shm)
	{
		ERROR("read create shmdata failed\n");
		return -1;
	}

	//接收升级文件
	pthread_t file_receive;
	pthread_create(&file_receive,NULL,&update_file_receive,NULL);

	//检查升级文件
	pthread_t file_check;
	pthread_create(&file_check,NULL,&update_file_check,NULL);

	while(1)
    {
	    sleep(1);
    }

	return 0;
}