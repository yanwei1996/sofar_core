//arm-linux-gnueabihf-gcc test_key.c ../sdk_key.c -o test_key  -L../../../ -l_lal -I../ -I../../../../include/lal -I../ -I../../../../include


#include "lal_gpio.h"
#include "sdk_key.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

extern void key_scan_process(uint32_t scan_period);
extern void key_init(void);
extern void dbg_get_key_io_value(void);

#define TIME_SCAN_PERIOD        50      //50ms
#define TIME_TICK_1S            1000    //1S
#define TICK_1S            (TIME_TICK_1S / TIME_SCAN_PERIOD)

int main(int argc, char* argv[])
{
    int32_t value, i;
    static uint32_t tick = 0;

    key_init();
    
    while (1)
    {
        if (++tick >= TICK_1S)
        {
            tick = 0;
            value = sdk_key_get();
            printf("sdk_key_get, val = %d\r\n", value);
            dbg_get_key_io_value();
        }

        key_scan_process(TIME_SCAN_PERIOD);
        usleep(TIME_SCAN_PERIOD * 1000);  //50ms 一个扫描周期
    }
}
