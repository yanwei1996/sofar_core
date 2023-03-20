
// gcc test_dido.c ../sdk_dido.c -o test_dido -L../../../ -l_lal -I../../../../include/sdk -I../../../../include/lal -I../../../../include
// arm-linux-gnueabihf-gcc test_dido.c ../sdk_dido.c -o test_dido -L../../../ -l_lal -I../../../../include/sdk -I../../../../include/lal -I../../../../include 


#include "core.h"
#include "data_types.h"
#include "sdk_dido.h"
#include "lal_gpio.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern void dido_init(void);


#ifdef __PROJECT_LEO5__	// leo 5   该编译宏开关，在sdk最外层的makefile 文件里定义。
	#define DI_NUM				4
    #define DO_NUM				4
#else	// 集储
	#define DI_NUM				8
    #define DO_NUM				8
#endif


int main(int argc, char* argv[])
{
    int32_t i, ret, out_channel, val;

    out_channel = atoi(argv[1]);
    val = atoi(argv[2]);
    
    dido_init();

    while (1)
    {
        for (i=0; i<DI_NUM; i++)
        {
            ret = sdk_dido_read(i);
            printf("di[%d] = %d\r\n", i, ret);
        }
        ret = sdk_dido_write(out_channel, val);
        printf("dout[%d] = %d\r\n", out_channel, ret);

        printf("\r\n");
        
        sleep(2); //1s
}
}

