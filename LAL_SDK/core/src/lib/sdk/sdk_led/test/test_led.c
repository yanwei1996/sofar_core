//arm-linux-gnueabihf-gcc test_led.c ../sdk_led.c -o test_led -L../../../ -l_lal -I../../../../include/sdk -I../../../../include/lal -I../../../../include
//gcc

#include "core.h"
#include "sdk_led.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int main(int argc, char* argv[])
{
    uint32_t cmd, id, period, duty;
    int32_t times;

    cmd = atoi(argv[1]);
    id = atoi(argv[2]);       //例子：0, 2000, 50, -1   代表：第1个指示灯，设置：周期2000ms, 占空比50%, -1无限循环
 
    led_init();


    while (1)
    {
        switch (cmd)
        {
            case 1: //闪烁测试
            {
                period = atoi(argv[3]); 
                duty = atoi(argv[4]);
                times = atoi(argv[5]); 	
                printf("id = %d, period = %d, duty = %d, times = %d\r\n", id, period, duty,times );

                sdk_led_flash(id, period, duty, times);
                break;
            }
 
            case 2:
            {
                sdk_led_on(id);
                break;
            }

            case 3:
            {
                sdk_led_off(id);
                break;
            }
        }
                  
		led_scan_process(500);   //扫描周期 500ms
        usleep(500000);
    }
}