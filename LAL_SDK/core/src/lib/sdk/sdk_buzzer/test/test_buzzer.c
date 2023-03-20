
//arm-linux-gnueabihf-gcc test_buzzer.c ../sdk_buzzer.c -o test_buzzer -L../../../ -l_lal -I../../../../include/sdk -I../../../../include/lal -I../../../../include
//gcc

#include "core.h"
#include "sdk_buzzer.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int main(int argc, char* argv[])
{
    uint32_t id, period, duty;
    int32_t times;

    id = atoi(argv[1]);       //例子：0, 2000, 50, -1   代表：第1个蜂鸣器，设置：周期2000ms, 占空比50%, -1无限循环
    period = atoi(argv[2]); 
    duty = atoi(argv[3]);
    times = atoi(argv[4]);   

    buzzer_init();
	sdk_buzzer_tick(id, period, duty, times);  

    while (1)
    {
		buzzer_scan_process(500);   //扫描周期 500ms
        usleep(500000);
    }
}
