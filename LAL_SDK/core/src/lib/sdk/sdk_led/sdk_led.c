/**
 * @file		sdk_led.c
 * @brief    led控制模块
 * @copyright Shouhang
 * @author   yangwenjuan
 * @note     2022.12.28 新拟制
 * @version  V1.0   
 * @date     2022/12/28
 */

#include "sdk_led.h"
#include "errors.h"
#include "lal_gpio.h"
#include <stdio.h>
#include "core.h"


#define log_e(...)		printf(__VA_ARGS__)	//fixme 后面easylog可以用后，屏蔽掉


// LED名称对应管脚
// 320kw项目
#define LED_PIN_ALARM           137             // LED1/LED2---GPIO5_9
#define LED_PIN_POWER           66              // LED3/LED4---GPIO3_IO02
#define LED_PIN_FAULT           67              // LED5/LED6---GPIO3_IO03

// LEO5项目&集储项目，相同
#define LED_PIN_ERR           		110             // LEO5项目：LED2——ERR灯——GPIO4_14
#define LED_PIN_RUN           		112             // LEO5项目：LED2——RUN灯——GPIO4_16 


// 指示灯开/关的电平值(与硬件相关)
#define LED_ON					1
#define LED_OFF					0

#define PERIOD_STAGE_ON			1
#define PERIOD_STAGE_OFF		2

#if 0
typedef struct {
	uint32_t period;		// 周期
	uint32_t duty;			// 占空比	
	int32_t times;			// >0 循环次数；<0 无限次	
	
	uint32_t on_scan;		//单个周期内，on的扫描次数（辅助用于逻辑实现）
	uint32_t off_scan;		//单个周期内，off的扫描次数（辅助用于逻辑实现）
	uint32_t period_stage;	//单个周期内，当前处于ON还是OFF阶段（辅助用于逻辑实现）
}led_tick_manage_t;
#endif
static uint32_t g_led_pin[LED_ID_NUM] = {LED_PIN_ERR, LED_PIN_RUN, LED_PIN_ALARM};	//后续需改为通过读取配置文件进行赋值
//static led_tick_manage_t g_led_tick[LED_ID_NUM];  //陈晓2023.3.12注释，将该部分放置到core.h里面


//陈晓2023.3.12修改，添加一个获取shm实例的函数
shm_t *get_shm_instance(const uint8_t from)
{
	static shm_t *p_shm_from_api;
   static shm_t *p_shm_from_proc;
   if(from == GET_SHM_FROM_API)
   {
      if(p_shm_from_api == NULL)
      {
         int32_t shm_id;
         key_t key;
         key = ftok(CORE_SHM_PATH_VAR_FILE,0x03);
         shm_id = shmget(key,0,0);
         if(shm_id == -1)
         {
            printf("%s--%d,shmid failed.\n",__func__,__LINE__);
            return NULL;
         }
         p_shm_from_api = (shm_t *)shmat(shm_id,NULL,0);
         if(p_shm_from_api == NULL)
         {
            printf("%s %d shmat failed\n",__func__,__LINE__);
            return NULL;
         }
      }
      return p_shm_from_api;
   }
   if(from == GET_SHM_FROM_PROC)
   {
      if(p_shm_from_proc == NULL)
      {
         int32_t shm_id;
         key_t key;
         key = ftok(CORE_SHM_PATH_VAR_FILE,0x03);
         shm_id = shmget(key,0,0);
         if(shm_id == -1)
         {
            printf("%s--%d,shmid failed.\n",__func__,__LINE__);
            return NULL;
         }
         p_shm_from_proc = (shm_t *)shmat(shm_id,NULL,0);
         if(p_shm_from_proc == NULL)
         {
            log_e("%s %d shmat failed\n",__func__,__LINE__);
            return NULL;
         }
      }
      return p_shm_from_proc;
   }
}
/**
 * @brief	led控制接口——————内部函数，非sdk接口函数
 * @param	[in] id			led编号
 * @param	[in] cmd		命令码
 * @return	执行结果
 * @retval	0  成功
 * @retval	-1 失败
 */
static int32_t led_ctrl(uint32_t id, uint32_t cmd)
{
	int32_t ret = ERR_NONE;
	
	if ((id >= LED_ID_NUM) || ((cmd != LED_ON) && (cmd != LED_OFF)))
	{
		ret = ERR_PARA;	
	}
	else
	{
		ret = lal_gpio_write(g_led_pin[id], cmd);
	}

	if (ret < 0)
	{
		log_e("Function[%s] Operation failed,id = %d, cmd =%d, ret = %d!\r\n", __func__, id, cmd, ret);
	}

	return ret;	
}

/**
 * @brief	led闪烁
 * @param	[in] id			led编号
 * @param	[in] period		led闪烁周期
 * -# 最小周期为任务周期或扫描周期（建议10ms）,需是任务及扫描周期的倍数
 * @param	[in] duty		led闪烁占空比
 * -# 0-100,表示0%-100%
 * @param	[in] times		led闪烁次数
 * -# -1 持续闪烁
 * -# >=0 闪烁次数
 * @return	执行结果
 * @retval	0  成功
 * @retval	-1 失败
 */
int32_t sdk_led_flash(uint32_t id, uint32_t period, uint32_t duty, int32_t times)
{
	if ((id >= LED_ID_NUM) || (duty > 100))
	{
		return -1;
	}
	shm_t *p_shm;
    p_shm = get_shm_instance(GET_SHM_FROM_API);
    if(p_shm == NULL)
    {
        return;
    }

	p_shm->led_tick_manage[id].period = period;
	p_shm->led_tick_manage[id].duty = duty;	
	p_shm->led_tick_manage[id].times = times;	

	return 0;		
}
 
/**
 * @brief	LED灯亮
 * @param	[in] id 	led编号
 * @return	执行结果
 * @retval	0  成功
 * @retval	-1 失败
 */
int32_t sdk_led_on(uint32_t id)
{
	int32_t ret = ERR_NONE;

	shm_t *p_shm;
    p_shm = get_shm_instance(GET_SHM_FROM_API);
    if(p_shm == NULL)
    {
        return;
    }
	if (id >= LED_ID_NUM)
	{
		ret = ERR_PARA;	
	}
	else
	{
		ret = led_ctrl(id, LED_ON);	
		p_shm->led_tick_manage[id].times = 0;	// 滴答切换次数置0，即关闭滴答	
	}

	if (ret < 0)
	{
		log_e("Function[%s] Operation failed, ret = %d!\r\n", __func__, ret);
	}
	return ret;	
}

/**
 * @brief	LED灯灭
 * @param	[in] id 	led编号
 * @return	执行结果
 * @retval	0  成功
 * @retval	-1 失败
 */
int32_t sdk_led_off(uint32_t id)
{
	int32_t ret = ERR_NONE;
	
	shm_t *p_shm;
    p_shm = get_shm_instance(GET_SHM_FROM_API);
    if(p_shm == NULL)
    {
        return;
    }

	if (id >= LED_ID_NUM)
	{
		ret = ERR_PARA;	
	}
	else
	{	
		ret = led_ctrl(id, LED_OFF);
		p_shm->led_tick_manage[id].times = 0;	//滴答切换次数置0，即关闭滴答
	}	

	if (ret < 0)
	{
		log_e("Function[%s] Operation failed, ret = %d!\r\n", __func__, ret);
	}
	return ret;	
}

/**
 * @brief	LED初始化(非SDK接口函数，在core层主进程初始化里调用)
 * @param	[in] void
 */
void led_init(void)
{
	int32_t i, ret = ERR_NONE;

	shm_t *p_shm;
    p_shm = get_shm_instance(GET_SHM_FROM_PROC);
    if(p_shm == NULL)
    {
        return;
    }

	//补充根据配置文件？对g_led_pin进行赋值。疑问，数组维度怎么定？
	
	for (i=0; i<LED_ID_NUM; i++)
	{
		ret = lal_gpio_open(g_led_pin[i]);		
		if (ret < 0)
		{
			log_e("Function[%s] Operation “lal_gpio_open” failed, id = %d, ret = %d!\r\n", __func__, i, ret);
			continue;
		}
		
		ret = lal_gpio_config(g_led_pin[i], 1);		//1 表示DO， 0：表示DI。建议使用宏，lal层定义宏？
		if (ret < 0)
		{
			log_e("Function[%s] Operation “lal_gpio_config” failed, id = %d, ret = %d!\r\n", __func__, i, ret);
			continue;
		}	
	}	
}

/**
 * @brief	LED扫描任务处理(非SDK接口函数，在core层主进程的10~50ms定时线程里定时调用)
 * @param	[in] scan_period 扫描周期ms（即该函数的外部调用周期）
 */
void led_scan_process(uint32_t scan_period)
{
	int32_t i;
	int32_t on_ticks, off_ticks;

	shm_t *p_shm;
    p_shm = get_shm_instance(GET_SHM_FROM_PROC);
    if(p_shm == NULL)
    {
        return;
    }

	
	for (i=0; i<LED_ID_NUM; i++)
	{
		//滴答次数已达0，退出滴答
		if (p_shm->led_tick_manage[i].times == 0)
		{
			p_shm->led_tick_manage[i].on_scan = 0;
			p_shm->led_tick_manage[i].off_scan = 0;
			p_shm->led_tick_manage[i].period_stage = PERIOD_STAGE_ON;
			continue;
		}	

		on_ticks = (p_shm->led_tick_manage[i].period * p_shm->led_tick_manage[i].duty / (scan_period * 100));
		off_ticks = (p_shm->led_tick_manage[i].period * (100 - p_shm->led_tick_manage[i].duty)/ (scan_period * 100));	

		//单个周期管理
		if (p_shm->led_tick_manage[i].period_stage == PERIOD_STAGE_ON)
		{
			if (p_shm->led_tick_manage[i].on_scan < on_ticks)
			{
				p_shm->led_tick_manage[i].on_scan++;
				led_ctrl(i, LED_ON);
			}
			else
			{
				p_shm->led_tick_manage[i].period_stage = PERIOD_STAGE_OFF;
				p_shm->led_tick_manage[i].on_scan = 0;
			}
		}
		else
		{
			if (p_shm->led_tick_manage[i].off_scan < off_ticks)
			{
				p_shm->led_tick_manage[i].off_scan++;
				led_ctrl(i, LED_OFF);
			}
			else
			{
				p_shm->led_tick_manage[i].period_stage = PERIOD_STAGE_ON;
				p_shm->led_tick_manage[i].off_scan = 0;				
				if (p_shm->led_tick_manage[i].times > 0)	//times = -1， 则无限循环
				{
					p_shm->led_tick_manage[i].times--;
				}
			}	
		}

		printf("%s--- i = %d, period=%d, duty=%d, times=%d, on_scan=%d, off_scan=%d, period_stage=%d, on_ticks=%d, off_ticks=%d\r\n",\
		__func__, i, p_shm->led_tick_manage[i].period, p_shm->led_tick_manage[i].duty, p_shm->led_tick_manage[i].times, p_shm->led_tick_manage[i].on_scan, p_shm->led_tick_manage[i].off_scan, p_shm->led_tick_manage[i].period_stage, on_ticks, off_ticks);		
	}	
}
