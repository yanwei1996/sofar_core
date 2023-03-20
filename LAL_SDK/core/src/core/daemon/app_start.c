/************************************************************************************
 *Project: Inverter-320kw-ARM
 *Description: app_init
 *Created on: 2022-08-10
 *Author: quliuliu
 ************************************************************************************/


#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "data_types.h"
#include "daemon.h"


int32_t main(int32_t argc, char *argv[])
{
	//看门狗
	

	//监控应用进程
	app_daemon();

    return 0;
}