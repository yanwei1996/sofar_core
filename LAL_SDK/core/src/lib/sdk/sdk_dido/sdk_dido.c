/**
* @file		sdk_dido.c
* @brief    DI/DO管理模块
* @copyright Shouhang
* @author   yangwenjuan
* @note     2023.01.05 新拟制
* @version  V1.0   
* @date     2023/01/05
*/

#include "core.h"
#include "sdk_dido.h"
#include "errors.h"
#include "lal_gpio.h"
#include <stdio.h>
#include <stddef.h>


#define log_e(...)		printf(__VA_ARGS__)	//fixme 后面easylog可以用后，屏蔽掉
#define __PROJECT_LEO5__

// DI、DO管脚定义(后续改为读配置文件？)
#ifdef __PROJECT_LEO5__	// leo 5   该编译宏开关，在sdk最外层的makefile 文件里定义。
	#define DI_NUM				4
	#define	DI_IN1				GPIO3_IO00
	#define	DI_IN2				GPIO3_IO02
	#define	DI_IN3				GPIO3_IO01
	#define	DI_IN4				GPIO3_IO03

	#define	DI_IN5				DI_IN1
	#define	DI_IN6				DI_IN1
	#define	DI_IN7				DI_IN1
	#define	DI_IN8				DI_IN1

	#define DO_NUM				4
	#define	DO_OUT1				GPIO3_IO08
	#define	DO_OUT2				GPIO3_IO07				
	#define	DO_OUT3				GPIO3_IO06				
	#define	DO_OUT4				GPIO3_IO05

	#define	DO_OUT5				DO_OUT1
	#define	DO_OUT6				DO_OUT1				
	#define	DO_OUT7				DO_OUT1				
	#define	DO_OUT8				DO_OUT1	
#else	// 集储
	#define DI_NUM				8
	#define	DI_IN1				GPIO3_IO00
	#define	DI_IN2				GPIO3_IO01
	#define	DI_IN3				GPIO4_IO19
	#define	DI_IN4				GPIO4_IO20
	#define	DI_IN5				GPIO3_IO02
	#define	DI_IN6				GPIO3_IO03
	#define	DI_IN7				GPIO3_IO04
	#define	DI_IN8				GPIO5_IO09

	#define DO_NUM				8
	#define	DO_OUT1				GPIO3_IO05
	#define	DO_OUT2				GPIO3_IO16				
	#define	DO_OUT3				GPIO3_IO07				
	#define	DO_OUT4				GPIO3_IO08
	#define	DO_OUT5				GPIO3_IO13
	#define	DO_OUT6				GPIO3_IO10				
	#define	DO_OUT7				GPIO3_IO15				
	#define	DO_OUT8				GPIO3_IO14	
#endif

static uint32_t g_di_pin[DI_NUM] = {DI_IN1, DI_IN2, DI_IN3, DI_IN4};	//后续需改为通过读取配置文件进行赋值
static uint32_t g_do_pin[DO_NUM] = {DO_OUT1, DO_OUT2, DO_OUT3, DO_OUT4};	//后续需改为通过读取配置文件进行赋值


/**
 * @brief	di/do初始化————(非SDK接口函数，在core层主进程初始化里调用)
 * @param	[in] void
 */
void dido_init(void)
{
	int32_t i, ret;

	//fixme, 补充根据配置文件对g_di_pin进行赋值
	
	for (i=0; i<DI_NUM; i++)
	{
		ret = lal_gpio_open(g_di_pin[i]);		
		if (ret < 0)
		{
			log_e("Function[%s] Operation “din----lal_gpio_open” failed, id = %d, ret = %d!\r\n", __func__, i, ret);
		}
		
		ret = lal_gpio_config(g_di_pin[i], 0);		//1 表示DO， 0：表示DI。建议使用宏，lal层定义宏？
		if (ret < 0)
		{
			log_e("Function[%s] Operation “din----lal_gpio_config” failed, id = %d, ret = %d, direct = 0!\r\n", __func__, i, ret);
		}	
	}

	for (i=0; i<DI_NUM; i++)
	{
		ret = lal_gpio_open(g_do_pin[i]);		
		if (ret < 0)
		{
			log_e("Function[%s] Operation “dout----lal_gpio_open” failed, id = %d, ret = %d!\r\n", __func__, i, ret);
		}
		
		ret = lal_gpio_config(g_do_pin[i], 1);		//1 表示DO， 0：表示DI。建议使用宏，lal层定义宏？
		if (ret < 0)
		{
			log_e("Function[%s] Operation “dout----lal_gpio_config” failed, id = %d, ret = %d, direct = 1!\r\n", __func__, i, ret);
		}	
	}		
}

/** 
 * @brief        写数据到指定的DO口
 * @param        [in] do_id  DO的编号（索引）值 （范围：0~7）
 * @param        [in] value 传入参数，所要写入的值：false或true。
 * @return       [int32_t] 执行结果
 * @retval       =0 成功
 * @retval       <0 失败 -1：索引不存在或超过最大值
 * @note         
 * @date         2022/12/20 13:30:47
 * @lastAuthor   APOI
 */
int32_t  sdk_dido_write (uint8_t do_id, bool value)
{
	int32_t ret;
	
	if (do_id >= DO_NUM)
	{
		ret = ERR_PARA;	
		goto __exit;
	}
	ret = lal_gpio_write(g_do_pin[do_id], value);

__exit:
	if (ret < 0)
	{
		log_e("Function[%s] Operation __exit, ret = %d!\r\n", __func__, ret);
	}
	return ret;		
}

/** 
  * @brief        读取指定索引值的 DI的数值
  * @param        [in] di_id 传入参数，索引值（范围：0~7）
  * @return       [int32_t] 执行结果
  * @retval       0或1 成功，表示对应的数值
  * @retval       <0 失败，如：-1 索引不存在或超过最大值
  * @note         
  * @date         2022/12/20 13:30:53
  * @lastAuthor   APOI
 */
int32_t  sdk_dido_read (uint8_t di_id)
{
	int32_t ret;
	
	if (di_id >= DI_NUM)
	{
		ret = ERR_PARA;	
		goto __exit;
	}
	ret = lal_gpio_read(g_di_pin[di_id]);	//是否需要加滤波？

__exit:
	if (ret < 0)
	{
		log_e("Function[%s] Operation __exit, ret = %d!\r\n", __func__, ret);
	}
	return ret;		
}

/** 
 * @brief        设置接口的中断方式和中断后的回调函数
 * @param        [in] di_id 传入参数，输入接口的索引
 * @param        [in] mode 传入参数，中断模式，
 * @param        [in] p_fcallback  传入参数，回调函数入口地址，当有中断产生时调用此函数。
 * @return       [int32_t] 执行结果
 * @retval       =0 成功
 * @retval       <0 失败，如：-1 索引不存在或超过最大值
 * @note         回调函数中禁止使用延时
 * @date         2022/12/20 13:31:01
 * @lastAuthor   APOI
 */
int32_t  sdk_dido_set_irq (uint8_t di_id, dido_irq_e mode, f_dido_irq_callback p_fcallback)
{
	int32_t ret;

	if ((di_id >= DI_NUM) || ((mode < DI_IRQ_LOW) || (mode > DI_IRQ_RISING_FALLING )) || (p_fcallback == NULL))
	{
		ret = ERR_PARA;	
		goto __exit;
	}
	
	ret = lal_gpio_set_irq(di_id, mode, p_fcallback, NULL);	

__exit:
	if (ret != 0)
	{
		log_e("Function[%s] Operation __exit, ret = %d!\r\n", __func__, ret);	
	}
	return ret;
}

/** 
 * @brief        释放输入口的中断
 * @param        [in] do_id 传入参数，输入接口的索引
 * @return       [int32_t] 执行结果
 * @retval       =0 成功
 * @retval       <0 失败，如：-1 索引不存在或超过最大值
 * @note         释放中断后，此接口恢复为普通的输入端口。
 * @date         2022/12/20 13:31:06
 * @lastAuthor   APOI
 */
int32_t  sdk_dido_free_irq (uint8_t di_id)
{
	return (-1);
}
