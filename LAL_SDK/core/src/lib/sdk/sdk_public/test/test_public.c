//arm-linux-gnueabihf-gcc test_public.c ../sdk_public.c -o test_public -L../../../ -l_lal -I../../../../include/sdk -I../../../../include/lal -I../../../../include
//gcc


#include "core.h"
#include "sdk_public.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define log_i(...)		printf(__VA_ARGS__)	    //fixme 后面easylog可以用后，屏蔽掉  



int main(int argc, char* argv[])
{
 

}
