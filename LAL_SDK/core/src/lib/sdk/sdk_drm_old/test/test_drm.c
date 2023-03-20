
// gcc test_drm.c ../sdk_drm.c -o test_drm -L../../../ -l_lal -I../../../../include/sdk -I../../../../include/lal -I../../../../include
// arm-linux-gnueabihf-gcc test_drm.c ../sdk_drm.c -o test_drm -L../../../ -l_lal -I../../../../include/sdk -I../../../../include/lal -I../../../../include 


#include "core.h"
#include "data_types.h"
#include "sdk_drm.h"
#include "lal_adc.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>


void main()
{
   int32_t drms0, drmn, value;

    while (1)
    {
      drm_drmns_process();   
      drms0 = sdk_drms0_get();
      sdk_drmn_get(&drmn);

      printf("drms0 = %08x,  drmn = %08x\r\n", drms0, drmn);

      lal_adc_read(1, &value);

      sleep(1); //1s
    }
}

