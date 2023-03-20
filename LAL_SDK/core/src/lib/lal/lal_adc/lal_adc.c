/************************************************************************************
 *Description: lal_adc
 *Created on: 2022-12-10
 *Author: quliuliu
 ************************************************************************************/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "lal_adc.h"

int32_t lal_adc_read(uint32_t num, int32_t *p_value)
{
    if(num!=0 && num!=1 && num!=2 && num!=3 && num!=4)
    {
        printf("sdk_adc_num error\n");
        return -1;
    }

    uint8_t *path[5]={DRM0, DRM1, DRM2, DRM3, DRM4};

    if(access(path[num], F_OK) != 0)    //检查是否存在该目录,如果不存在就创建
    {
        printf("ADC %s does not exist\n", path[num]);
        return -1;
    }

    int8_t fd, ret;
    int8_t data[32]={0};
    fd = open(path[num], O_RDONLY);
    if(fd < 0) {
        printf("[%s:%s:%d] %s\n", __FILE__,__func__, __LINE__, strerror(errno));
        return -1;
    }
    ret = read(fd, data, sizeof(data));
    if(ret < 0) {
        printf("[%s:%s:%d] %s\n", __FILE__,__func__, __LINE__, strerror(errno));
        close(fd);
        return -1;
    }
    close(fd);

    *p_value = atoi(data);

    return atoi(data);
}


// double vref = 3.3;  //电压参考值
// double voltage, adc_value;
// adc_value = atof(data);
// voltage = vref * (adc_value / 4096);
// printf("voltage: %.3f\n", voltage);

//3.3/4096*1000
