 /**
* @file		sdk_drm.c
* @brief    与DRM有关的状态获取
* @copyright Shouhang
* @author   yanwei
* @note     2023.03.17 新拟制
* @version  V1.0   
* @date     2023/03/17
*/

#include "sdk_drm.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#include "stdint.h"


int get_voltage()
{
    FILE *fp = NULL;
	char buffer[8]={0};
	char buffer2[8]={0};
    int adc_value=0;
    int voltage=0;
    fp=popen("cat /sys/devices/platform/soc/2100000.aips-bus/2198000.adc/iio:device0/in_voltage3_raw","r");
    fgets(buffer, sizeof(buffer), fp);
    pclose(fp);
    if(memcmp(buffer, buffer2, sizeof(buffer))==0)
	{
		printf("SDK erro: this net device is no exit !!!\n");
		return -1;
	}
    adc_value = atoi(buffer);
    voltage = adc_value*3360/4096;  //电压（mv）
    //voltage = adc_value*3000/4096;  //电压（mv）
    return voltage;
}


/** 
  * @brief        DRM0状态获取
  * @return       [int32_t] 返回执行结果
  * @note         
  * @retval       0、1代表DRM0对应的状态，小于0对应的错误代码。
  * @date         2022/12/20 08:39:17
  * @lastEditors  APOI
 */
int32_t sdk_drms0_get(void)
{
    int tmp=-1;
    system("echo 1 > /sys/class/gpio/gpio83/value"); 
    system("echo 0 > /sys/class/gpio/gpio81/value");
    system("echo 1 > /sys/class/gpio/gpio79/value");
    tmp = get_voltage();
    if(tmp > 4096 || tmp < 0) return -1 ;//错误
    
    //if(tmp > 900 || tmp < 200) return 1; //DRMs0响应
    if(tmp > 1000 || tmp < 200) return 1; //DRMs0响应
    else return 0; //DRMs0不响应

}

int32_t clear_bit(int32_t *data,int32_t bit)
{
    *data = *data & (~(0x01<<bit));
    //data &= ~(0x1 << bit);
    return *data;
}

int32_t set_bit(int32_t *data,int32_t bit)
{
    *data = *data | (0x01<<bit);
    return *data;
}

/** 
  * @brief        获取drm1-drm8的状态值（目前最大值数量是8个）
  * @param        [uint32_t] *p_drmn:转出参数，存放DRMN的有效值，不同的bit值组合对应的是功率输出值 bit0-bit7,对应DRM1-DRM8。
  * @return       [int32_t]
  * @note         
  * @retval       bit0 --bit8,对应编号为DRM1-DRM8的AFCI值（p_drmn）是否有效
  * @date         2022/12/20 08:39:30
  * @lastEditors  APOI
 */
int32_t sdk_drmn_get(int32_t *p_drmn)
{
    int tmp = -1;
    int32_t drmsn[1]={0};

    //printf("-----------drmsDRMS1/5\r\n");
    system("echo 0 > /sys/class/gpio/gpio83/value"); 
    system("echo 0 > /sys/class/gpio/gpio81/value");
    system("echo 0 > /sys/class/gpio/gpio79/value");
    tmp = get_voltage();
   // printf("电压 = %dmv\r\n",tmp);
    if(tmp >= 900) {clear_bit(drmsn,0); clear_bit(drmsn,4); }   //printf("无响应\r\n");
    else if(tmp >= 200) {clear_bit(drmsn,0); set_bit(drmsn,4); }  //printf("DRMS5\r\n");
    else {set_bit(drmsn,0); clear_bit(drmsn,4); }  //printf("DRMS1\r\n");

    //printf("-----------drmsDRMS2/6\r\n");
    system("echo 1 > /sys/class/gpio/gpio83/value"); 
    system("echo 0 > /sys/class/gpio/gpio81/value");
    system("echo 0 > /sys/class/gpio/gpio79/value");
    tmp = get_voltage();
   // printf("电压 = %dmv\r\n",tmp);
    if(tmp >= 900) {clear_bit(drmsn,1); clear_bit(drmsn,5); }   //printf("无响应\r\n");
    else if(tmp >= 200) {clear_bit(drmsn,1); set_bit(drmsn,5); }  //printf("DRMS6\r\n");
    else {set_bit(drmsn,1); clear_bit(drmsn,5); }  //printf("DRMS2\r\n");

   // printf("-----------drmsDRMS3/7\r\n");
    system("echo 0 > /sys/class/gpio/gpio83/value"); 
    system("echo 1 > /sys/class/gpio/gpio81/value");
    system("echo 0 > /sys/class/gpio/gpio79/value");
    tmp = get_voltage();
   // printf("电压 = %dmv\r\n",tmp);
    if(tmp >= 900) {clear_bit(drmsn,2); clear_bit(drmsn,6); }   //printf("无响应\r\n");
    else if(tmp >= 200) {clear_bit(drmsn,2); set_bit(drmsn,6); }  //printf("DRMS7\r\n");
    else {set_bit(drmsn,2); clear_bit(drmsn,6); }  //printf("DRMS3\r\n");

   // printf("-----------drmsDRMS4/8\r\n");
    system("echo 1 > /sys/class/gpio/gpio83/value"); 
    system("echo 1 > /sys/class/gpio/gpio81/value");
    system("echo 0 > /sys/class/gpio/gpio79/value");
    tmp = get_voltage();
   // printf("电压 = %dmv\r\n",tmp);
    if(tmp >= 900) {clear_bit(drmsn,3); clear_bit(drmsn,7); }   //printf("无响应\r\n");
    else if(tmp >= 200) {clear_bit(drmsn,3); set_bit(drmsn,7); }  //printf("DRMS8\r\n");
    else {set_bit(drmsn,3); clear_bit(drmsn,7); }  //printf("DRMS4\r\n");

    *p_drmn = *drmsn;

    return 0;
}

/*
void main()
{
    int tmp = -1;
    int32_t p_drmn[1]={0};
    printf("-----------drmsDRMS1/5\r\n");
    system("echo 0 > /sys/class/gpio/gpio83/value"); 
    system("echo 0 > /sys/class/gpio/gpio81/value");
    system("echo 0 > /sys/class/gpio/gpio79/value");
    tmp = get_voltage();
    printf("电压 = %dmv\r\n",tmp);
    if(tmp >= 900) printf("无响应\r\n");
    else if(tmp >= 200) printf("DRMS5\r\n");
    else printf("DRMS1\r\n");

    printf("-----------drmsDRMS2/6\r\n");
    system("echo 1 > /sys/class/gpio/gpio83/value"); 
    system("echo 0 > /sys/class/gpio/gpio81/value");
    system("echo 0 > /sys/class/gpio/gpio79/value");
    tmp = get_voltage();
    printf("电压 = %dmv\r\n",tmp);
    if(tmp >= 900) printf("无响应\r\n");
    else if(tmp >= 200) printf("DRMS6\r\n");
    else printf("DRMS2\r\n");

    printf("-----------drmsDRMS3/7\r\n");
    system("echo 0 > /sys/class/gpio/gpio83/value"); 
    system("echo 1 > /sys/class/gpio/gpio81/value");
    system("echo 0 > /sys/class/gpio/gpio79/value");
    tmp = get_voltage();
    printf("电压 = %dmv\r\n",tmp);
    if(tmp >= 900) printf("无响应\r\n");
    else if(tmp >= 200) printf("DRMS7\r\n");
    else printf("DRMS3\r\n");

    printf("-----------drmsDRMS4/8\r\n");
    system("echo 1 > /sys/class/gpio/gpio83/value"); 
    system("echo 1 > /sys/class/gpio/gpio81/value");
    system("echo 0 > /sys/class/gpio/gpio79/value");
    tmp = get_voltage();
    printf("电压 = %dmv\r\n",tmp);
    if(tmp >= 900) printf("无响应\r\n");
    else if(tmp >= 200) printf("DRMS8\r\n");
    else printf("DRMS4\r\n");

    printf("-----------drmsDRMS0/9\r\n");
    system("echo 1 > /sys/class/gpio/gpio83/value"); 
    system("echo 0 > /sys/class/gpio/gpio81/value");
    system("echo 1 > /sys/class/gpio/gpio79/value");
    tmp = get_voltage();
    printf("电压 = %dmv\r\n",tmp);
    if(tmp > 900 || tmp < 200) printf("DRMS0\r\n");
    else printf("无响应\r\n");

    printf("DRMS0 = %d\n",sdk_drms0_get());
    printf("sdk_drmn_get retrun %d, *p_drmn = %x \r\n",sdk_drmn_get(p_drmn),p_drmn[0]);

    
	
}*/