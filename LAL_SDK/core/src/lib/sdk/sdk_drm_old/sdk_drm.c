 /**
* @file		sdk_drm.c
* @brief    与DRM有关的状态获取
* @copyright Shouhang
* @author   yangwenjuan
* @note     2023.01.05 新拟制
* @version  V1.0   
* @date     2023/01/05
*/

#include "sdk_drm.h"
#include "errors.h"
#include "lal_gpio.h"
#include "lal_adc.h"
#include "core.h"
#include <stdio.h>

#define log_e(...)		printf(__VA_ARGS__)	//fixme 后面easylog可以用后，屏蔽掉


//drms、drmn的触发电平定义（和硬件有关）
#define DRM_DRMS_TIGGER_VALUE 		0
#define DRM_DRMS_UNTIGGER_VALUE 	1
#define DRM_DRMN_TIGGER_VALUE 		1
#define DRM_DRMN_UNTIGGER_VALUE 	0

//DRM的编号
#define DRM_ID_DRMS0				0
#define DRM_ID_DRMN_1 				1
#define DRM_ID_DRMN_8 				8
#define DRM_NUM_DRMN_ALL 			(DRM_ID_DRMN_8)

enum drm_num
{
	DRM_NUM_DRM0 = 0,  	//drms0
	DRM_NUM_DRM1_5 = 1, //drmn 1-8
	DRM_NUM_DRM2_6 = 2,
	DRM_NUM_DRM3_7 = 3,
	DRM_NUM_DRM4_8 = 4	
};

typedef struct
{
	uint8_t drm[DRM_NUM_DRMN_ALL + 1];	//drm[0]为drms0, drm[1]为drmn1...drm[8]为drmn8
}drm_level_t;


//陈晓修改2023.3.13	放在了core.h头文件里面
//static int32_t g_drms0_value = -1;
//static int32_t g_drmn_value = -1;	//记录采样计算转化后的drms0和drmn的值

/** 
 * @brief        DRM0状态获取
 * @return       [int32_t] 返回执行结果
 * @note         补
 * @retval       0、1代表DRM0对应的状态，小于0对应的错误代码。
 */
int32_t sdk_drms0_get(void)
{	
	shm_t *p_shm;
	p_shm = get_shm_instance(GET_SHM_FROM_API);
	if(p_shm == NULL)
	{
		return -1;
	}
	return p_shm->drms0_value;
	//return g_drms0_value;
}

/** 
  * @brief        获取drm1-drm8的状态值（目前最大值数量是8个）
  * @param        [uint32_t] *p_drmn:转出参数，存放DRMN的有效值，不同的bit值组合对应的是功率输出值 bit0-bit7,对应DRM1-DRM8。
  * @return       [int32_t]
  * @note         
  * @retval       bit0 --bit8,对应编号为DRM1-DRM8的AFCI值（p_drmn）是否有效
 */
int32_t sdk_drmn_get(uint32_t *p_drmn)
{
	shm_t *p_shm;
	p_shm = get_shm_instance(GET_SHM_FROM_API);
	if(p_shm == NULL)
	{
		return -1;
	}
	*p_drmn = p_shm->drmn_value;
}




///////////////////////////////////////////////////
////////////以下为内部函数，非SDK接口函数
///////////////////////////////////////////////////

/**
* @brief    drm端口电压采样
* @param    [in]num ad通道号
* @return   端口电压采样计算转换结果
*/
static int32_t drm_voltage_value_get(uint32_t num)	//————不用浮点， 放大至mv处理；2023.01.07疑问，无加偏置，不会有负值?
{
	int32_t vref = 3300;  //电压参考值3.3V=3300mv
	int32_t voltage = 0;
	int32_t adc_value = 0;
	
	lal_adc_read(num, &adc_value);
	voltage = (vref * adc_value) >> 12; //单位mv	

	return voltage;
}

/**
* @brief    根据电压获取电平
* @param    [out]p_drm_level:	获取的结果存放到结构体中
* @return   执行结果
* @retval	0  正常
* @retval	-1 异常
*/
static int32_t drm_level_get(drm_level_t *p_drm_level)
{
	int32_t i;
	int32_t drms_voltage_min = 200;		//200mv  = 0.2v	//这里后续需改为，根据机型/配置参数？取不同值
	int32_t drms_voltage_max = 1000;	//1000mv = 1.0v
	int32_t drmn_voltage_min = 200;
	int32_t drmn_voltage_max = 1000;
	int32_t drm_voltage = 0;
	
	if(p_drm_level == NULL)
	{
		return -1;
	}
	
	p_drm_level->drm[DRM_ID_DRMS0] = DRM_DRMS_UNTIGGER_VALUE;		//0
	for (i = DRM_ID_DRMN_1; i <= DRM_NUM_DRMN_ALL; i++)	//1~8
	{
		p_drm_level->drm[i] = DRM_DRMN_UNTIGGER_VALUE;	
	}
	
	//DRM0 <0.2V 或者>1.0V DRM0
	drm_voltage = drm_voltage_value_get(DRM_NUM_DRM0);
	if((drm_voltage < drms_voltage_min) || (drm_voltage > drms_voltage_max))
	{
		p_drm_level->drm[DRM_ID_DRMS0] = DRM_DRMS_TIGGER_VALUE;
	}
	else
	{
		p_drm_level->drm[DRM_ID_DRMS0] = DRM_DRMS_UNTIGGER_VALUE;
	}

	//DRM1/5, <0.2V DRM1  ,<0.9V DRM5;	DRM2/6、DRM3/7、DRM4/8相同判断
	for (i = DRM_ID_DRMN_1; i <= (DRM_NUM_DRMN_ALL/2); i++)	//i=1~4
	{
		drm_voltage = drm_voltage_value_get(i);
		//这里是否需要加滤波处理？
		if (drm_voltage < drmn_voltage_min)
		{
			p_drm_level->drm[i] = DRM_DRMN_TIGGER_VALUE;
		}
		else if ((drm_voltage > drmn_voltage_min) && (drm_voltage < drmn_voltage_max))
		{
			p_drm_level->drm[i + (DRM_NUM_DRMN_ALL/2)] = DRM_DRMN_TIGGER_VALUE;
		}
		else
		{
			p_drm_level->drm[i] = DRM_DRMN_UNTIGGER_VALUE;							//1,2,3,4
			p_drm_level->drm[i + (DRM_NUM_DRMN_ALL/2)] = DRM_DRMN_UNTIGGER_VALUE;	//5,6,7,8
		}		
	}
	return 0;
}

/**
* @brief    根据每路的电平合成drms drmn的值
* @param    [in]p_drm_level: 各路电平值结果
* @param    [out]drms_value: drms0的值
* @param    [out]drmn_value: drm1-8的值，每位代表1路，低位是1，高位是8路
* @return   执行结果
* @retval	0  正常
* @retval	-1 异常
*/
static int32_t drm_value_get(const drm_level_t *p_drm_level, int32_t *drms_value, int32_t *drmn_value)
{
	int32_t i;
		
	if( (p_drm_level == NULL) ||
	  (drms_value == NULL)||
	  (drmn_value == NULL))
	{
		log_e("Function[%s] Operation failed: illegal input parameter\r\n", __func__);
		return -1;
	}
	
	//DRMS0状态：Bit0：DRMS0状态值 1：开机 0：关机
	*drms_value = p_drm_level->drm[DRM_ID_DRMS0];	//drm0触发了，drmS0给0了，DRM1-8都会被设置为0（见drm_level_priority_deal）

	//设置drmn电平位，为1置对应位置1
	*drmn_value = 0;
	for (i = DRM_ID_DRMN_1; i <= DRM_NUM_DRMN_ALL; i++)
	{
		if(p_drm_level->drm[i])
		{
			*drmn_value |= (1 << (i - 1));
		}
	}	
	return 0;
}

/**
* @brief    drms、drmn采样计算状态获取处理————————PS:core的主进程的定时线程里定时调用
* @param    void
*/
void drm_drmns_process(void)	
{
	shm_t *p_shm;
	p_shm = get_shm_instance(GET_SHM_FROM_API);
	if(p_shm == NULL)
	{
		return ;
	}
	drm_level_t drm_level;
	
	//根据电压获取电平
	drm_level_get(&drm_level);
	
	//合成drms/drmn的状态值
	drm_value_get(&drm_level, &p_shm->drms0_value, &p_shm->drmn_value);	
	//2023.01.06评审：开关量，直接调用lal获取状态；adc模式的，按上述方法计算;
	//目前只处理adc模式的，其他后期完善：配置文件读取，判断是adc模式、开关量模式；
	//core层需考虑内部可配参数：和硬件相关；	
	
	/*  
	 * PS:集储项目，可能是开关量模式；leo项目，明确是adc模式。320项目，是监控直接发送drms/drmn状态给逆变器
	 *	leo项目是监控根据drms/drmn值计算功率发给逆变器;
	 */
}
