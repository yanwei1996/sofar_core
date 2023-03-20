#include <stdio.h>
#include <unistd.h>
#include "sdk_led.h"

void main()
{
    sdk_led_init(1);

    while (1)
    {
        sdk_led_ioctl(1, ON);
        sleep(1);
        sdk_led_ioctl(1, OFF);
        sleep(1);
    }
    
    return;
}
