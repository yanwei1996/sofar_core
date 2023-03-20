#ifndef __CORE_H__
#define __CORE_H__

/**
* @file		core.h
* @brief    core层共用接口
* @copyright Shouhang
* @author   yangwenjuan
* @note     2023.01.07 新拟制
* @version  V1.0   
* @date     2023/01/07
*/

#include "data_types.h"

//#define __PROJECT_LEO5__   // 宏编译开关：启用该行，则GPIO为LEO5的配置；屏蔽该行，则GPIO为集储的配置

/*=========================IPC相关定义=============================*/
#define PROCESS_EVENT_NUM MAX_EVENT_NUM

#define MSG_HEAD_LEN       4  //2 * sizeof(uint16_t);   //消息头有2个uint16_T 类型数，所以乘以2
#define IPC_MAX_DATA_LEN   4096
#define SERVER_PORT        32505 //连接时使用的端口

typedef enum
 {
    EVENT_REGIST     = 0,      //事件注册
    EVENT_RESPONSE   = 1, 
    EVENT_SEND_DATA  = 2,      //发送数据   
    EVENT_CONNECT    = 3,
 }ipc_msg_type_e;

 typedef struct 
 {
    uint16_t type;
    uint16_t data_len;
    int8_t data[IPC_MAX_DATA_LEN];
 }ipc_msg_t;

/**
 * 日期：2023.3.10
 * 添加一个共享内存的结构，扫描到的数据都放在共享内存
 */

//陈晓2023.3.10添加
#include <stdio.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>
#define CORE_SHM_PATH_VAR_DIR "/tmp/core"
#define CORE_SHM_PATH_VAR_FILE "/tmp/core/var_shm"

#define GET_SHM_FROM_API 0
#define GET_SHM_FROM_PROC 1
//这部分是led闪烁的结构体
typedef struct {
	uint32_t period;		// 周期
	uint32_t duty;			// 占空比	
	int32_t times;			// >0 循环次数；<0 无限次	
	
	uint32_t on_scan;		//单个周期内，on的扫描次数（辅助用于逻辑实现）
	uint32_t off_scan;		//单个周期内，off的扫描次数（辅助用于逻辑实现）
	uint32_t period_stage;	//单个周期内，当前处于ON还是OFF阶段（辅助用于逻辑实现）
}led_buzzer_tick_manage_t;
typedef struct
{
   uint16_t key_value[4];  //按键
   led_buzzer_tick_manage_t led_tick_manage[3]; //led逻辑
   led_buzzer_tick_manage_t buzzer_tick_manage[1];
   int32_t drms0_value;
   int32_t drmn_value;
}shm_t;

//陈晓2023.3.12修改，添加一个获取shm实例的函数
shm_t *get_shm_instance(const uint8_t from);
//////陈晓 ，添加完

/*=========================IPC相关定义=============================*/

/**
* @brief	蜂鸣器初始化(非SDK接口函数，在core层主进程初始化里调用)
* @param	[in] void
*/
int32_t buzzer_init(void);

/**
* @brief	蜂鸣器扫描任务处理(非SDK接口函数，在core层主进程的10~50ms定时线程里定时调用)
* @param	[in] scan_period 扫描周期ms（即该函数的外部调用周期）
*/
void buzzer_scan_process(uint32_t scan_period);




/**
 * @brief	LED初始化(非SDK接口函数，在core层主进程初始化里调用)
 * @param	[in] void
 */
void led_init(void);

/**
 * @brief	LED扫描任务处理(非SDK接口函数，在core层主进程的10~50ms定时线程里定时调用)
 * @param	[in] scan_period 扫描周期ms（即该函数的外部调用周期）
 */
void led_scan_process(uint32_t scan_period);




/**
* @brief    drms、drmn采样计算状态获取处理————————PS:core的主进程的定时线程里定时调用
* @param    void
*/
void drm_drmns_process(void);	



/**
 * @brief	按键初始化————(非SDK接口函数，在core层主进程初始化里调用)
 * @param	[in] void
 */
void key_init(void);


/**
 * @brief	按键扫描任务处理————(非SDK接口函数，在core层主进程的10~50ms定时线程里定时调用)
 * @param	[in] scan_period 扫描周期ms（即该函数的外部调用周期）
 */
void key_scan_process(uint32_t scan_period);


/**
 * @brief  记录的默认路径初始化函数————非sdk接口函数，在core进程初始化时调用
 * @return void
 */
void record_path_init(void);


/**
 * @brief	di/do初始化————(非SDK接口函数，在core层主进程初始化里调用)
 * @param	[in] void
 */
void dido_init(void);

#endif

