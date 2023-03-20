/************************************************************************************
 *Description: lal_gpio
 *Created on: 2022-12-10
 *Author: quliuliu
 ************************************************************************************/

#ifndef __LAL_GPIO_H__
#define __LAL_GPIO_H__

#include "data_types.h"


// GPIO_03组 PIN值
#define GPIO3_IO00		64		// 32 * 2 = 64
#define GPIO3_IO01		65		// 32 * 2 + 1= 65
#define GPIO3_IO02		66		
#define GPIO3_IO03		67	
#define GPIO3_IO04		68	
#define GPIO3_IO05		69	
#define GPIO3_IO06		70	
#define GPIO3_IO07		71	
#define GPIO3_IO08		72
#define GPIO3_IO09		73
#define GPIO3_IO10		74
#define GPIO3_IO11		75
#define GPIO3_IO12		76
#define GPIO3_IO13		77
#define GPIO3_IO14		78	
#define GPIO3_IO15		79	
#define GPIO3_IO16		80	

// GPIO_04组 PIN值
#define GPIO4_IO19		115	
#define GPIO4_IO20		116	

// GPIO_05组 PIN值
#define GPIO5_IO09		137	


typedef void(*gpio_irq_callback) (void *args);
typedef enum{ 
   LAL_GPIO_IRQ_LOW = 0U , LAL_GPIO_IRQ_HIGH = 1U , LAL_GPIO_IRQ_FALLING = 2U , LAL_GPIO_IRQ_RISING = 3U , 
   LAL_GPIO_IRQ_RISING_FALLING = 4U 
}lal_gpio_irq_e ;


/**
  * @enum hal_gpio_direction_t
  * @brief GPIO方向
  */
enum
{
    LAL_GPIO_INPUT  = 0U,   	///< 输入
    LAL_GPIO_OUTPUT = 1U,    	///< 输出  
    LAL_GPIO_OUTPUT_OD = 2U,	///< 输出OD门
} ;

int32_t  lal_gpio_open (uint32_t pin);
int32_t  lal_gpio_config (uint32_t pin, uint32_t direction);
int32_t  lal_gpio_write (uint32_t pin, int32_t value) ;
int32_t  lal_gpio_read (uint32_t pin) ;
int32_t lal_gpio_set_irq(uint32_t pin, lal_gpio_irq_e mode, gpio_irq_callback p_fcallback, void *p_args);

#endif /* __LAL_GPIO_H__ */


// 指示灯
// int32_t lal_led_init(int32_t num);
// int32_t lal_led_ctrl(int32_t num,int32_t value);
