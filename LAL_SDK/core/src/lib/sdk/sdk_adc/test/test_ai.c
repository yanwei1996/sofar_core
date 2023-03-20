//arm-linux-gnueabihf-gcc test_ai.c ../sdk_ai.c -o test_ai  -L../../../ -l_lal -I../ -I../../../../include/lal -I../ -I../../../../include


#include "lal_adc.h"
#include "sdk_ai.h"
#include <stdlib.h>
#include <stdio.h>

#define TEST_VER_LEN    15

int main(int argc, char* argv[])
{
    int32_t channel;
    int32_t value;
    int8_t ver[TEST_VER_LEN]={0};

    channel = atoi(argv[1]);

    value = sdk_ai_adc_get(channel);

    printf("sdk_ai_adc_get, channel = %d, val = %d\r\n", channel, value);

}
