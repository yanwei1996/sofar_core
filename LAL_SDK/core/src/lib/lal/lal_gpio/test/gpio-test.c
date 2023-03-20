#include <stdio.h>
#include <unistd.h>
#include "sdk_gpio.h"

//arm-linux-gnueabihf-gcc gpio-test.c -I../../../include -I/home/work/system/I.MX6ULL/ZLG-M6Y2C/buildroot-2020.02.1/output/staging/usr/include  --sysroot=/home/work/system/I.MX6ULL/ZLG-M6Y2C/buildroot-2020.02.1/output/staging/ -L../../ -l_sdk -lsqlite3 -o gpio-test

void main()
{
	sdk_led_power_init();
	while(1)
	{
		sdk_led_power_ctrl(LED_ON);
		usleep(500*1000);
		sdk_led_power_ctrl(LED_OFF);
		usleep(500*1000);
	}
	
    
    return;
}
