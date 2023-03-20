/**
* @file		sdk_key.c
* @brief    按键模块
* @copyright Shouhang
* @author   yangwenjuan
* @note     2023.02.09 新拟制
* @version  V1.0   
* @date     2023/02/09
*/
#include "core.h"
#include "lal_gpio.h"
#include "sdk_key.h"
#include<stdio.h>

#define log_e(...)		printf(__VA_ARGS__)	//fixme 

#define KEY_NUM_MAX                     1              // fixme, 后续需要根据需要修改；按键个数最大值

#define KEY_PRESS_VALUE                 0                 // 按键按下时的值

#define KEY_SHORT_PRESS_TIME            200             // 按键短按持续时间(ms),   0.5S
#define KEY_LONG_PRESS_TIME             3000            // 按键长按持续时间(ms),   3S
#define KEY_SUPER_LONG_PRESS_TIME       10000           // 按键超长按持续时间(ms),   10S
#define __PROJECT_LEO5__

#ifdef __PROJECT_LEO5__   // LEO5  该编译宏开关，在sdk最外层的makefile 文件里定义。
    #define DI_IN_1           64            // leo5, DIN-1, A47 GPIO3_IO00      
    #define DI_IN_2           66            // leo5, DIN-2, A48 GPIO3_IO02   
    #define DI_IN_3           65            // leo5, DIN-3, A49 GPIO3_IO01   
    #define DI_IN_4           67            // leo5, DIN-4, A50 GPIO3_IO03     
    #define DI_IN_SW          68            // leo5, ---LEO5的SW2复位按键GPIO3_4     
#else   //集储
    #define DI_IN_1           64            // 集储, DIN-1, A47 GPIO3_IO00      
    #define DI_IN_2           65            // 集储, DIN-2, A49 GPIO3_IO01   
    #define DI_IN_3           115           // 集储, DIN-3, A61 GPIO4_IO19   
    #define DI_IN_4           116           // 集储, DIN-4, A63 GPIO4_IO20  
#endif 


//KEY对应管脚   // fixme 根据原理图改一下, 秦工27日确定暂leo5暂时需求，先按din1、din2实现
#define KEY_1_PIN           DI_IN_1            // leo5, DIN-1, A47 GPIO3_IO00 
#define KEY_2_PIN           DI_IN_2            // leo5, DIN-2, A49 GPIO3_IO02      // 集中式储能DIN-2, A49 GPIO3_IO02
#define KEY_3_PIN           DI_IN_3            // 
#define KEY_4_PIN           DI_IN_4            //
#define KEY_5_PIN           DI_IN_SW 


enum
{
    KEY_STATUS_NULL = 0,	// 无按
    KEY_STATUS_PRESS,	    // 按键按下
};


typedef enum
{
    KEY_SHORT_PRESS = 1,    //按键键值：短按
    KEY_LONG_PRESS,		    //按键键值：长按
}single_key_val_e;


typedef struct 
{
    uint16_t count;          // 按键持续周期计数参数
    uint16_t now_status;     // 按键当前状态参数
    uint16_t last_status;    // 按键上次状态参数
}key_para_t;
static key_para_t g_key_para[KEY_NUM_MAX] = {0};

static uint32_t g_key_pin[KEY_NUM_MAX] = {KEY_5_PIN };	//各按键管脚编号，后续需改为通过读取配置文件进行赋值??
//static single_key_val_e g_key_value[KEY_NUM_MAX] = {0};		//单个按键键值,chenxiao 2023.3.10注释，放到了core.h里面

/**
 * @brief			获取按键的信息 （不同按键对应的长按、短按、组合键状态 ）
 * @return			执行结果
 * @retval			按键值获取
 * @warning			无
 */
key_val_e sdk_key_get(void)
{
    uint32_t key_val = KEY_NO_PRESS;
    shm_t *p_shm;
    static uint8_t loop = 0;  //调用一次返回一个按键值，多个同时按，循环返回

   p_shm = get_shm_instance(GET_SHM_FROM_API);
   if(p_shm == NULL)
   {
        return KEY_NO_PRESS;
   }

    loop++;
    if(loop >= KEY_NUM_MAX)
    {
        loop = 0;
    }
    
    if (p_shm->key_value[loop] == KEY_LONG_PRESS)
    {

        key_val = KEY1_SHORT_PRESS * loop + 2; //偏移 2 是短按
        /*
        if((loop > 0) && (loop <KEY_NUM_MAX ))
        {
            key_val = KEY1_SHORT_PRESS * loop + 2; //偏移 2 是短按
            if(0)
            if (p_shm->key_value[loop - 1] == KEY_LONG_PRESS)
            {
                key_val = KEY1_2_PRESS;
    //            p_shm->key_value[0] = KEY_NO_PRESS;
    //           p_shm->key_value[1] = KEY_NO_PRESS;
            }
            else
            {
//               key_val = KEY1_LONG_PRESS;
                key_val = KEY1_SHORT_PRESS * loop + 1;  //偏移 1 是短按
//                key_val = KEY1_SHORT_PRESS
    //             p_shm->key_value[0] = KEY_NO_PRESS;
            }
        }
        */
        
    }
     else
     if (p_shm->key_value[loop] == KEY_SHORT_PRESS)
    {

        key_val = KEY1_SHORT_PRESS * loop + 1;  //偏移 1 是短按

    }
    else
    {
        key_val = p_shm->key_value[loop];
    }

   return key_val;
 #if 0
    if (g_key_value[0] == KEY_LONG_PRESS)
    {
        if (g_key_value[1] == KEY_LONG_PRESS)
        {
            key_val = KEY1_2_PRESS;
            g_key_value[0] = KEY_NO_PRESS;
            g_key_value[1] = KEY_NO_PRESS;
        }
        else
        {
            key_val = KEY1_LONG_PRESS;
            g_key_value[0] = KEY_NO_PRESS;
        }
    }
    else if (g_key_value[0] == KEY_SHORT_PRESS)
    {
        key_val = KEY1_SHORT_PRESS;
        g_key_value[0] = KEY_NO_PRESS;
    }
    else if (g_key_value[1] == KEY_LONG_PRESS)
    {
        key_val = KEY2_LONG_PRESS;
        g_key_value[1] = KEY_NO_PRESS;
    }
    else if (g_key_value[1] == KEY_SHORT_PRESS)
    {
        key_val = KEY2_SHORT_PRESS;
        g_key_value[1] = KEY_NO_PRESS;
    }
    else if (g_key_value[2] == KEY_LONG_PRESS)
    {
        key_val = KEY3_LONG_PRESS;
        g_key_value[2] = KEY_NO_PRESS;
    }
    else if (g_key_value[2] == KEY_SHORT_PRESS)
    {
        key_val = KEY3_SHORT_PRESS;
        g_key_value[2] = KEY_NO_PRESS;
    }
    else if (g_key_value[3] == KEY_LONG_PRESS)
    {
        key_val = KEY4_LONG_PRESS;
        g_key_value[3] = KEY_NO_PRESS;
    }
    else if (g_key_value[3] == KEY_SHORT_PRESS)
    {
        key_val = KEY4_SHORT_PRESS;
        g_key_value[3] = KEY_NO_PRESS;
    }
#endif
    return key_val;
}


/**
 * @brief	按键初始化————(非SDK接口函数，在core层主进程初始化里调用)
 * @param	[in] void
 */
void key_init(void)
{
	int32_t i, ret;

	//fixme, 补充根据配置文件对g_key_pin进行赋值
	
	for (i = 0; i < KEY_NUM_MAX; i++)
	{
		ret = lal_gpio_open(g_key_pin[i]);		
		if (ret < 0)
		{
			log_e("qms Function[%s] Operation “lal_gpio_open” failed, id = %d, ret = %d!\r\n", __func__, i, ret);
		}
		
		ret = lal_gpio_config(g_key_pin[i], 0);		//1 表示DO， 0：表示DI。建议使用宏，lal层定义宏？
		if (ret < 0)
		{
			log_e("Function[%s] Operation “lal_gpio_config” failed, id = %d, ret = %d!\r\n", __func__, i, ret);
		}	
     //   g_key_para[i].last_status = g_key_para[i].now_status = 1; //初始化
	}	
}

/**
 * @brief	按键扫描任务处理————(非SDK接口函数，在core层主进程的10~50ms定时线程里定时调用)
 * @param	[in] scan_period 扫描周期ms（即该函数的外部调用周期）
 */
void key_scan_process(uint32_t scan_period)
{
 	uint16_t i = 0;
	int32_t value;
    shm_t *p_shm;
    p_shm = get_shm_instance(GET_SHM_FROM_PROC);
    if(p_shm == NULL)
    {
        log_e("get share memory failed\n");
        return;
    }
    for (i = 0; i < KEY_NUM_MAX; i++)
    {
        g_key_para[i].last_status = g_key_para[i].now_status;
		value = lal_gpio_read(g_key_pin[i]);
        if (value == KEY_PRESS_VALUE)
        {
            if(g_key_para[i].now_status != KEY_STATUS_PRESS)
            {
                log_e(" key[%d] = %d down\r\n", i, value);
            }

            g_key_para[i].count++;
            g_key_para[i].now_status = KEY_STATUS_PRESS;

            if (g_key_para[i].count >= 0xFFFF)  // 按键卡住，需处理
            {
                g_key_para[i].count = 0xFFFE;   //10分钟算卡键
            }

            if (g_key_para[i].last_status == KEY_STATUS_NULL)  // 按键刚按下
            {
                p_shm->key_value[i] = KEY_NO_PRESS; //每次按键新按下时，需清除上次的键值，等待本次键值判断
            }

            if ((g_key_para[i].count * scan_period) >= KEY_LONG_PRESS_TIME)
                {
                    if(p_shm->key_value[i] != KEY_LONG_PRESS)
                    {
                        log_e(" key[%d] = %d \r\n", i, KEY_LONG_PRESS);
                        p_shm->key_value[i] = KEY_LONG_PRESS;
                    }

                }
                else if ((g_key_para[i].count * scan_period) >= KEY_SHORT_PRESS_TIME)
                {
                    if(p_shm->key_value[i] != KEY_SHORT_PRESS)
                    {
                        log_e(" key[%d] = %d \r\n", i, KEY_SHORT_PRESS);
                        p_shm->key_value[i] = KEY_SHORT_PRESS;
                    }
                    
                }
				else
				{
                    p_shm->key_value[i] = KEY_NO_PRESS;
				}      
        }
        else 
        {
            g_key_para[i].now_status = KEY_STATUS_NULL;

            if (g_key_para[i].last_status == KEY_STATUS_PRESS)	// 按键松开时刻，进行键值判断	
            {
/*               
                if ((g_key_para[i].count * scan_period) >= KEY_LONG_PRESS_TIME)
                {
                    p_shm->key_value[i] = KEY_LONG_PRESS;
                }
                else if ((g_key_para[i].count * scan_period) >= KEY_SHORT_PRESS_TIME)
                {
                    p_shm->key_value[i] = KEY_SHORT_PRESS;
                }
				else
				{
                    p_shm->key_value[i] = KEY_NO_PRESS;
				}
*/ 
                printf(" key[%d] = %d up key=%d \r\n", i, value,p_shm->key_value[i]);
                p_shm->key_value[i] = KEY_NO_PRESS;
                g_key_para[i].count = 0;
            }
        }
    }
}

// fixme 下面为开发测试的打印接口，后续删掉
void dbg_get_key_io_value(void)
{
    int32_t i=3;
    int32_t value;
    static int32_t old_value = 0;

//    for (i=0; i<KEY_NUM_MAX; i++)
    {
        value = lal_gpio_read(g_key_pin[i]);
        if(old_value != value)
        {
            printf(" key[%d] = %d\r\n", i, value);
            old_value = value;
        }
        
    }
}
