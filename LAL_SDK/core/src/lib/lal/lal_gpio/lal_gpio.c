/************************************************************************************
 *Description: lal_gpio
 *Created on: 2022-12-10
 *Author: quliuliu
 ************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#include "lal_gpio.h"


//蜂鸣器
#define BUZZER              133             // BUZZ---GPIO5_5    GPIO排列序号=(BANK-1)×32+N      133=(5-1)*32+5

//LED名称
#define LED_ALARM           137             // LED1/LED2---GPIO5_9
#define LED_POWER           66              // LED3/LED4---GPIO3_IO02
#define LED_FAULT           67              // LED5/LED6---GPIO3_IO03

//PID
#define PID_ADJ_0_GPIO      52              // ADJ_0---GPIO2_IO20
#define PID_ADJ_1_GPIO      53              // ADJ_1---GPIO2_IO21
#define PID_FAIL_GPIO       50              // PID_FAIL---GPIO2_IO18 //PID_FAIL_GPIO方向设置为输入，默认没有外接东西读到的是1 高电平，接VCC读到也是1高电平，给它接地GND读到的是0低电平。
#define PID_ON_GPIO         51              // PID_ON---GPIO2_IO19

//PID
#define SVG_GPIO      34              // SVG---GPIO2_IO02

//GPIO方向
#define DIRECTION_IN        "in"
#define DIRECTION_OUT       "out"


static int32_t lal_system_cmd(int8_t * cmd)
{
    FILE *fp = NULL;

    if((fp=popen(cmd, "r")) == NULL)
    {
        printf("[%s:%s:%d] %s, cmd:%s\n", __FILE__,__func__, __LINE__, strerror(errno), cmd);
        return -1;
    }
    pclose(fp);

    return 0;
}


int32_t sys_gpio_init(int32_t num)
{
    int8_t order[128]={0};
    sprintf(order, "/sys/class/gpio/gpio%d", num);

    if(access(order, F_OK) != 0)    //检查是否存在该gpio目录,如果不存在就创建
    {
        memset(order, 0, sizeof(order));
        sprintf(order, "echo %d > /sys/class/gpio/export", num);
        lal_system_cmd(order);
        return 0;
    }
    else
    {
        printf("gpio%d  exist\n", num);
        return -1;
    }
}

int32_t sys_gpio_set_direction(int32_t num, const int8_t *dir)
{
    if(strcmp(dir, "in")==0 || strcmp(dir, "out")==0)
    {
        int8_t order[128]={0};
        sprintf(order, "/sys/class/gpio/gpio%d", num);
        if(access(order, F_OK) != 0)    //检查是否存在该gpio目录,如果不存在就返回错误
        {
            printf("gpio%d does not exist\n", num);
            return -1;
        }
        else
        {
            memset(order, 0, sizeof(order));
            sprintf(order, "echo %s > /sys/class/gpio/gpio%d/direction", dir, num);
            lal_system_cmd(order);
            return 0;
        }
    }
    else
    {
        printf("parameter errer\n");
        return -1;
    }
}

int32_t sys_gpio_set_value(int32_t num, int32_t value)
{
    if(value == 0 || value == 1)
    {
        int8_t order[128]={0};
        sprintf(order, "/sys/class/gpio/gpio%d", num);
        if(access(order, F_OK) != 0)    //检查是否存在该gpio目录,如果不存在就返回错误
        {
            printf("gpio%d does not exist\n", num);
            return -1;
        }
        else
        {
            memset(order, 0, sizeof(order));
            sprintf(order, "echo %d > /sys/class/gpio/gpio%d/value", value, num);
            lal_system_cmd(order);
            return 0;
        }
    }
    else
    {
        printf("parameter errer\n");
        return -1;
    }
}

int32_t sys_gpio_get_direction(int32_t num, int8_t *dir) //获取GPIO的方向，dir: 输入in，输出out， 失败-1.
{
	int32_t fd = -1;
    int8_t order[128]={0};
    sprintf(order, "/sys/class/gpio/gpio%d", num);
    if(access(order, F_OK) != 0)    //检查是否存在该gpio目录,如果不存在就返回错误
    {
        return -1;
    }
    else
    {
        memset(order, 0, sizeof(order));
        sprintf(order, "/sys/class/gpio/gpio%d/direction", num);
        if((fd=open(order, O_RDWR)) < 0)
        {
            printf("open direction error, order:%s\n", order);
            return -1;
        }
        
        if(read(fd, dir, 3)<0)
        {
            printf("read direction error, order:%s\n", order);
            close(fd);
            return -1;
        }
    }
	close(fd);
    return 0;
}

int32_t lal_gpio_get_value(int32_t num) //获取GPIO的值(对于输入方向的引脚有效)，返回低电平 0， 高电平 1， 失败-1.
{
	int32_t fd = -1;
    int8_t order[128]={0};
    sprintf(order, "/sys/class/gpio/gpio%d", num);
    if(access(order, F_OK) != 0)    //检查是否存在该gpio目录,如果不存在就返回错误
    {
        return -1;
    }
    else
    {
        memset(order, 0, sizeof(order));
        sprintf(order, "/sys/class/gpio/gpio%d/value", num);
        if((fd=open(order, O_RDWR)) < 0)
        {
            printf("lal_gpio_get_value open %s error!\n", order);
            return -1;
        }
        memset(order, 0, sizeof(order));
        if(read(fd, order, 3)<0)
        {
            printf("lal_gpio_get_value read fd:%d error!!\n", fd);
            close(fd);
            return -1;
        }
        close(fd);
        return atoi(order);
    }
}


//蜂鸣器
int32_t lal_buzzer_init()
{
    if(sys_gpio_init(BUZZER) != 0)
    {
        return -1;
    }
    if(sys_gpio_set_direction(BUZZER, (int8_t *)DIRECTION_OUT) != 0)
    {
        printf("buzzer gpio set direction failed\n");
        return -2;
    }

    return 0;
}
int32_t lal_buzzer_ctrl(int32_t value)
{
    if(value != 0 && value != 1)
    {
       printf("parameter errer\n");
        return -1;
    }

    if(sys_gpio_set_value(BUZZER, value) != 0)
    {
        printf("buzzer control failed\n");
        return -2;
    }

    return 0;
}


//Power LED
int32_t lal_led_power_init()
{
    if(sys_gpio_init(LED_POWER) != 0)
    {
        return -1;
    }
    if(sys_gpio_set_direction(LED_POWER, (int8_t *)DIRECTION_OUT) != 0)
    {
        printf("power LED set direction failed\n");
        return -2;
    }

    return 0;
}
int32_t lal_led_power_ctrl(int32_t value)
{
    if(value != 0 && value != 1)
    {
        printf("parameter errer\n");
        return -1;
    }

    if(sys_gpio_set_value(LED_POWER, value) != 0)
    {
        printf("power LED control failed\n");
        return -2;
    }

    return 0;
}
//Alarm LED
int32_t lal_led_alarm_init()
{
    if(sys_gpio_init(LED_ALARM) != 0)
    {
        return -1;
    }
    if(sys_gpio_set_direction(LED_ALARM, (int8_t *)DIRECTION_OUT) != 0)
    {
        printf("alarm LED set direction failed\n");
        return -2;
    }

    return 0;
}
int32_t lal_led_alarm_ctrl(int32_t value)
{
    if(value != 0 && value != 1)
    {
        printf("parameter errer\n");
        return -1;
    }

    if(sys_gpio_set_value(LED_ALARM, value) != 0)
    {
        printf("alarm LED control failed\n");
        return -2;
    }

    return 0;
}
//Fault LED
int32_t lal_led_fault_init()
{
    if(sys_gpio_init(LED_FAULT) != 0)
    {
        return -1;
    }
    if(sys_gpio_set_direction(LED_FAULT, (int8_t *)DIRECTION_OUT) != 0)
    {
        printf("fault LED set direction failed\n");
        return -2;
    }

    return 0;
}
int32_t lal_led_fault_ctrl(int32_t value)
{
    if(value != 0 && value != 1)
    {
        printf("parameter errer\n");
        return -1;
    }

    if(sys_gpio_set_value(LED_FAULT, value) != 0)
    {
        printf("fault LED control failed\n");
        return -2;
    }

    return 0;
}


//PID硬件设计有4根线，其中：一故障线、一功能开关、两档位线；档位线未用；
int32_t lal_PID_init()
{
    sys_gpio_init(PID_FAIL_GPIO);
	sys_gpio_set_direction(PID_FAIL_GPIO, (int8_t *)DIRECTION_IN); //PID_FAIL_GPIO:故障状态，检测输入状态。高：故障，低：正常。	
	sys_gpio_init(PID_ON_GPIO);
	sys_gpio_set_direction(PID_ON_GPIO, (int8_t *)DIRECTION_OUT); //PID_ON_GPIO:控制PID继电器。触发输出1，不触发输出0.
	sys_gpio_init(PID_ADJ_0_GPIO);
	sys_gpio_set_direction(PID_ADJ_0_GPIO, (int8_t *)DIRECTION_OUT);
	sys_gpio_init(PID_ADJ_1_GPIO);
	sys_gpio_set_direction(PID_ADJ_1_GPIO, (int8_t *)DIRECTION_OUT);

    return 0;
}
int32_t lal_PID_status()
{
    int32_t ret = -1;
    ret = lal_gpio_get_value(PID_FAIL_GPIO);
    if(ret == -1)
    {
        printf("PID status error\n");
        return -1;
    }
    else
    {
        return ret;
    }
}
int32_t lal_PID_ctrl(int32_t value)
{
    if(value != 0 && value != 1)
    {
        printf("parameter errer\n");
        return -1;
    }

    if(sys_gpio_set_value(PID_ON_GPIO, value) != 0)
    {
        printf("PID control failed\n");
        return -2;
    }

    return 0;
}



int32_t lal_SVG_init()
{
    sys_gpio_init(SVG_GPIO);
	sys_gpio_set_direction(SVG_GPIO, (int8_t *)DIRECTION_OUT); 
    lal_SVG_ctrl(0);

    return 0;
}
int32_t lal_SVG_status()
{
    int32_t ret = -1;
    ret = lal_gpio_get_value(SVG_GPIO);
    if(ret == -1)
    {
        printf("SVG status error\n");
        return -1;
    }
    else
    {
        return ret;
    }
}
int32_t lal_SVG_ctrl(int32_t value)
{
    if(value != 0 && value != 1)
    {
        printf("parameter errer\n");
        return -1;
    }

    if(sys_gpio_set_value(SVG_GPIO, value) != 0)
    {
        printf("SVG control failed\n");
        return -2;
    }

    return 0;
}

int32_t  lal_gpio_open (uint32_t pin)
{
    int32_t ret = 0;

    ret = sys_gpio_init(pin);  

    return ret;
}

int32_t lal_gpio_config(uint32_t pin, uint32_t direction)
{
    int32_t ret = 0;

    if (direction == LAL_GPIO_INPUT)
    {
        sys_gpio_set_direction(pin, "in");
    }
    else if (direction == LAL_GPIO_OUTPUT)
    {
        sys_gpio_set_direction(pin, "out");
    }
    else
    {
        ret = -1;
    } 
    return ret;   
}

int32_t lal_gpio_write(uint32_t pin, int32_t value)
{
    int32_t ret;

    ret = sys_gpio_set_value(pin, value);
}

int32_t lal_gpio_read(uint32_t pin)
{
    lal_gpio_get_value(pin);
} 

int32_t lal_gpio_set_irq(uint32_t pin, lal_gpio_irq_e mode, gpio_irq_callback p_fcallback, void *p_args)
{

}




