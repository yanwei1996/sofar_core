//arm-linux-gnueabihf-gcc test_version.c ../sdk_version.c -o test_version  -I../ -I../../../../include
//gcc test_version.c ../sdk_version.c -o test_version  -I../ -I../../../../include


#include "sdk_version.h"
#include <stdlib.h>
#include <stdio.h>

#define TEST_VER_LEN    15

int main(int argc, char* argv[])
{
    int32_t cmd;
    int32_t ret;
    int8_t ver[TEST_VER_LEN]={0};

    cmd = atoi(argv[1]);

    ret = sdk_version_get(cmd, ver, TEST_VER_LEN);

    printf("version_test, type = %d, ret = %d, ver = %s\r\n", cmd, ret, ver);

}
