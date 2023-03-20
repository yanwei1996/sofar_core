/** 
 * @file          Do not edit
 * @author        qinmingsheng
 * @date          2023/01/06 17:51:30
 * @lastAuthor    Please set LastEditors
 * @lastTime      2023/02/01
 * @filePath      /Linux_SDK/src/main.c
 * @brief         
 * @copyright Copyright (c) 2023 by SofarSolar, All Rights Reserved. 
 */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "sdk_log.h"
#include "zlog.h"
#include "core.h"

int32_t main(int32_t argc, char** argv)
{
    int8_t buf[MAX_DATA_LEN] = {0};
//    uint32_t buf_len = 0;
//    int32_t ret = 0;
    uint16_t i = 0;

    sdk_log_init();
    sdk_log_set_level(6);
       
    log_a((int8_t *)"no test");
    log_a((int8_t *)"test=%s %d %f","ss",123,0.56);

    log_e((int8_t *)"no test");
    log_e((int8_t *)"test=%s %d %f","error",123,0.56);

    log_w((int8_t *)"no test");
    log_w((int8_t *)"test=%s %d %f","warn",123,0.56);

    log_i((int8_t *)"no test");
    log_i((int8_t *)"test=%s %d %f","info",123,0.56);

    log_d((int8_t *)"no test");
    log_d((int8_t *)"test=%s %d %f","debug",123,0.56);
    if (argc == 1)
    {
        log_a((int8_t *)"no test");
        log_a((int8_t *)"test=%s %d %f","ss",123,0.56);

        log_e((int8_t *)"no test");
        log_e((int8_t *)"test=%s %d %f","error",123,0.56);

        log_w((int8_t *)"no test");
        log_w((int8_t *)"test=%s %d %f","warn",123,0.56);

        log_i((int8_t *)"no test");
        log_i((int8_t *)"test=%s %d %f","info",123,0.56);

        log_d((int8_t *)"no test");
        log_d((int8_t *)"test=%s %d %f","debug",123,0.56);

        for ( i = 0; i < MAX_DATA_LEN; i++)
        {
            buf[i] = i;
        }
    
        sdk_log_hexdump((const int8_t *)"const char *p_name", 16, (uint8_t *)buf, MAX_DATA_LEN);
        sdk_log_hexdump((const int8_t *)"const char *p_name", 32, (uint8_t *)buf, MAX_DATA_LEN);
        sleep(1);
    }
    else
    {
       
    }
    
    while (1)
    {   
        memset(buf,0,MAX_DATA_LEN);
        if (argc == 1)
        {
            
        }else
        {

        }
    }

   sdk_log_finish(); 
    
}

//gcc write.c ../lib_sdk_ipc.a -o write
//arm-linux-gnueabihf-gcc write.c -I../../../include -I/home/work/system/I.MX6ULL/ZLG-M6Y2C/buildroot-2020.02.1/output/staging/usr/include  --sysroot=/home/work/system/I.MX6ULL/ZLG-M6Y2C/buildroot-2020.02.1/output/staging/ -L../../ -l_sdk -lsqlite3 -o ipc_write