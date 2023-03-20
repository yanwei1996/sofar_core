/************************************************************************************
 *Description: sdk_adc
 *Created on: 2022-6-20
 *Author: quliuliu
 ************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "sdk_adc.h"

int main(int argc, char* argv[])
{
    // double voltage, get_value;
    printf("adc test %d\n", atoi(argv[1]));

    while(1)
    {
       printf("voltage=%d\n", sdk_adc_read(atoi(argv[1])));
       sleep(1);
    }
    

    // printf("get value: %f\n", get_value);
    // printf("voltage: %.3f\n", voltage);

    return 0;
}

//arm-linux-gnueabihf-gcc adc-test.c -I../../../include -I/home/work/system/I.MX6ULL/ZLG-M6Y2C/buildroot-2020.02.1/output/staging/usr/include  --sysroot=/home/work/system/I.MX6ULL/ZLG-M6Y2C/buildroot-2020.02.1/output/staging/ -L../../ -l_sdk -lsqlite3 -o adc-test
