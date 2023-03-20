/**
* @file		sdk_buzzer.c
* @brief    蜂鸣器管理模块
* @copyright Shouhang
* @author   yangwenjuan
* @note     2023.01.05 新拟制
* @version  V1.0   
* @date     2023/01/05
*/

#include "sdk_buzzer.h"
#include "errors.h"
#include "lal_gpio.h"
#include "core.h"
#include <stdio.h>


#define log_e(...)		printf(__VA_ARGS__)	//fixme 后面easylog可以用后，屏蔽掉


//蜂鸣器ID起始编号
#define BUZZER_ID_START 		0

//蜂鸣器IO管脚号(与硬件相关)				
#define BUZZER_PIN_SYS          133					// BUZZ---GPIO5_5    GPIO排列序号=(BANK-1)×32+N      133=(5-1)*32+5

//蜂鸣器响/关闭的电平值(与硬件相关)
#define BUZZER_ON				1
#define BUZZER_OFF				0

#define PERIOD_STAGE_ON			1
#define PERIOD_STAGE_OFF		2

#if 0
typedef struct {
	uint32_t period;			// 周期
	uint32_t duty;				// 占空比	
	int32_t times;				// >0 周期次数；<0 无限次	
	
	uint32_t on_scan;			//单个周期内，on的扫描次数（辅助用于逻辑实现）
	uint32_t off_scan;			//单个周期内，off的扫描次数（辅助用于逻辑实现）
	uint32_t period_stage;		//单个周期内，当前处于ON还是OFF阶段（辅助用于逻辑实现）
}buzzer_tick_manage_t;
#endif
static uint32_t g_buzzer_pin[BUZZER_ID_NUM] = {BUZZER_PIN_SYS};	//后续需改为通过读取配置文件然后对该数组进行赋值，注意数组元素最大数量
//static buzzer_tick_manage_t g_buzzer_tick[BUZZER_ID_NUM]; 


/**
 * @brief	buzzer控制接口——————内部函数，非sdk接口函数
 * @param	[in] id			buzzer编号
 * @param	[in] cmd		命令码
 * @return	执行结果
 * @retval	0  成功
 * @retval	-1 失败
 */
static int32_t buzzer_ctrl(uint32_t id, uint32_t cmd)
{
	int32_t ret = ERR_NONE;
	
	if ((id > BUZZER_ID_NUM) || ((cmd != BUZZER_ON) && (cmd != BUZZER_OFF)))
	{
		ret = ERR_PARA;	
	}
	else
	{
		ret = lal_gpio_write(g_buzzer_pin[id], cmd);
	}

	if (ret < 0)
	{
		log_e("Function[%s] Operation failed,id = %d, cmd =%d, ret = %d!\r\n", __func__, id, cmd, ret);
	}

	return ret;	
}

/**
 * @brief	蜂鸣器滴答
 * @param	[in] id	蜂鸣器编号
 * @param	[in] period		蜂鸣器鸣响周期
 * -# 最小周期为任务周期或扫描周期（建议10ms）,需是任务及扫描周期的倍数
 * @param	[in] duty		蜂鸣器鸣响占空比
 * -# 0-100,表示0%-100%
 * @param	[in] times		蜂鸣器鸣响次数
 * -# -1表示持续鸣响
 * -# >=0表示实际鸣响次数
 * @return	执行结果
 * @retval	0  设置成功
 * @retval	-1 设置失败
 */
int32_t sdk_buzzer_tick(uint32_t id, uint32_t period, uint32_t duty, int32_t times)
{
	if ((id > BUZZER_ID_NUM) || (duty > 100))
	{
		return -1;
	}	
	shm_t *p_shm;
    p_shm = get_shm_instance(GET_SHM_FROM_API);
    if(p_shm == NULL)
    {
        return;
    }
	p_shm->buzzer_tick_manage[id].period = period;
	p_shm->buzzer_tick_manage[id].duty = duty;	
	p_shm->buzzer_tick_manage[id].times = times;	
	return 0;
}

/**
 * @brief	蜂鸣器响
 * @param	[in] id	蜂鸣器标号
 * @return	执行结果
 * @retval	0  设置成功
 * @retval	-1 设置失败
 * @warning	无警告
 */
int32_t sdk_buzzer_on(uint32_t id)
{
	int32_t ret = ERR_NONE;
	
	shm_t *p_shm;
    p_shm = get_shm_instance(GET_SHM_FROM_API);
    if(p_shm == NULL)
    {
        return;
    }
	if (id > BUZZER_ID_NUM)
	{
		ret = ERR_PARA;	
	}
	else
	{
		p_shm->buzzer_tick_manage[id].times = 0;	//滴答切换次数置0，即关闭滴答		
		ret = buzzer_ctrl(id, BUZZER_ON);
	}

	if (ret < 0)
	{
		log_e("Function[%s] Operation failed, ret = %d!\r\n", __func__, ret);
	}

	return ret;
}

/**
 * @brief	蜂鸣器灭
 * @param	[in] id	蜂鸣器标号
 * @return	执行结果
 * @retval	0  设置成功
 * @retval	-1 设置失败
 * @warning	无警告
 */
int32_t sdk_buzzer_off(uint32_t id)
{
	int32_t ret;
	
	shm_t *p_shm;
    p_shm = get_shm_instance(GET_SHM_FROM_API);
    if(p_shm == NULL)
    {
        return -1;
    }
	if (id > BUZZER_ID_NUM)
	{
		ret = ERR_PARA;	
	}
	else
	{
		p_shm->buzzer_tick_manage[id].times = 0;	//滴答切换次数置0，即关闭滴答	
		ret = buzzer_ctrl(id, BUZZER_OFF);
	}

	if (ret < 0)
	{
		log_e("Function[%s] Operation failed, ret = %d!\r\n", __func__, ret);
	}

	return ret;	
}

/**
* @brief	蜂鸣器初始化(非SDK接口函数，在core层主进程初始化里调用)
* @param	[in] void
*/
int32_t buzzer_init(void)	//疑问："echo %d > /sys/class/gpio/export" 调用lal层哪个函数会执行？
{
	int32_t i;
	int32_t ret = 0;
	int32_t result ;

	shm_t *p_shm;
	p_shm = get_shm_instance(GET_SHM_FROM_PROC);
	if(p_shm == NULL)
	{
			return -1;
	}
	//补充根据配置文件？还是型号？对g_buzzer_pin[]数组进行赋值(buzzer、led的硬件随单板硬件而定)。疑问，数组维度怎么定？
	
	for (i=0; i<BUZZER_ID_NUM; i++)
	{
		result = lal_gpio_open(g_buzzer_pin[i]);		//需lal层再确定一下，是否以下顺序操作，即完成了对应iO初始化
		if (result < 0)
		{
			log_e("Function[%s] Operation “lal_gpio_open” __exit, id = %d, result = %d!\r\n", __func__, i, result);
			ret = -1;
			continue;
		}
		
		result = lal_gpio_config(g_buzzer_pin[i], 1);		//1 表示DO， 0：表示DI。建议使用宏，lal层定义宏？
		if (result < 0)
		{
			log_e("Function[%s] Operation “lal_gpio_config” __exit, id = %d, result = %d!\r\n", __func__, i, result);
			ret = -1;
			continue;
		}		
		// rec = lal_gpio_close(g_buzzer_pin[i]);		//lal_gpio_close()接口基本不需用到；比如在需动态改变IO方向时会用到；
	}

	return ret;
}

/**
* @brief	蜂鸣器扫描任务处理(非SDK接口函数，在core层主进程的10~50ms定时线程里定时调用)
* @param	[in] scan_period 扫描周期ms（即该函数的外部调用周期）
*/
void buzzer_scan_process(uint32_t scan_period)
{
	int32_t i;
	int32_t on_ticks, off_ticks;
	
	shm_t *p_shm;
	p_shm = get_shm_instance(GET_SHM_FROM_PROC);
	if(p_shm == NULL)
	{
			return ;
	}
	for (i=0; i<BUZZER_ID_NUM; i++)
	{
		//滴答次数已达0，退出滴答
		if (p_shm->buzzer_tick_manage[i].times == 0)
		{
			p_shm->buzzer_tick_manage[i].on_scan = 0;
			p_shm->buzzer_tick_manage[i].off_scan = 0;
			p_shm->buzzer_tick_manage[i].period_stage = PERIOD_STAGE_ON;
			continue;
		}		
		
		on_ticks = (p_shm->buzzer_tick_manage[i].period * p_shm->buzzer_tick_manage[i].duty / (scan_period * 100));
		off_ticks = (p_shm->buzzer_tick_manage[i].period* (100 - p_shm->buzzer_tick_manage[i].duty) / (scan_period * 100));
		//单个周期管理
		if (p_shm->buzzer_tick_manage[i].period_stage == PERIOD_STAGE_ON)
		{
			if (p_shm->buzzer_tick_manage[i].on_scan < on_ticks)	
			{
				p_shm->buzzer_tick_manage[i].on_scan++;
				buzzer_ctrl(i, BUZZER_ON);
			}
			else
			{
				p_shm->buzzer_tick_manage[i].period_stage = PERIOD_STAGE_OFF;
				p_shm->buzzer_tick_manage[i].on_scan = 0;
			}
		}
		else
		{
			if (p_shm->buzzer_tick_manage[i].off_scan < off_ticks)
			{
				p_shm->buzzer_tick_manage[i].off_scan++;
				buzzer_ctrl(i, BUZZER_OFF) ;
			}
			else
			{
				p_shm->buzzer_tick_manage[i].period_stage = PERIOD_STAGE_ON;
				p_shm->buzzer_tick_manage[i].off_scan = 0;				
				if (p_shm->buzzer_tick_manage[i].times > 0)	//times = -1， 则无限循环
				{
					p_shm->buzzer_tick_manage[i].times--;
				}
			}	
		}		
	}	
}

