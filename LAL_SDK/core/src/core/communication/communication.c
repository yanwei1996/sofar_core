/**
* @file		
* @brief    
* @copyright Shouhang
* @author   
* @note     2023.01.11 新拟制
* @version  V1.0   
* @date     2023/01/11
*/


#include "core.h"
#include <time.h>
#include <sys/time.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

 #define log_i(...)		printf(__VA_ARGS__)	//fixme 

#define TIMING_PERIOD_MS        50      //50ms 
#define DBG_PRINTF_CNT          (1000 / TIMING_PERIOD_MS)

/**
 * @brief   设置定时器
 * @return  void
 */
void set_timer(void)
{
    struct itimerval itv;
    struct itimerval oldtv;

    itv.it_interval.tv_sec = 0;
    itv.it_interval.tv_usec = TIMING_PERIOD_MS * 1000;    //定时周期50ms （core设计说明书定义10~50ms）
    itv.it_value.tv_sec = 1;                              //1S后启动
    itv.it_value.tv_usec = 0;

    setitimer(ITIMER_REAL, &itv, &oldtv);  //真实时间间隔
}


/**
 * @brief   时间打印函数————（前期测试阶段用）
 * @return  void
 */
void time_printf(void)
{
    static uint32_t count = 0;

    if (++count >= DBG_PRINTF_CNT)    //控制1S打印一次数据
    {
        time_t now;
        //time(&now);	//获得时间戳：自1970-01-01起的秒数
        now = time(NULL);
        struct tm *p_tm = localtime(&now);	//localtime()获取系统时间(本地时间)
        log_i("timed_task_handle---%04d-%02d-%02d %02d:%02d:%02d\r\n", p_tm->tm_year, p_tm->tm_mon, p_tm->tm_mday, p_tm->tm_hour, p_tm->tm_min, p_tm->tm_sec);
        count = 0;
    }
 }


/**
 * @brief   定时处理函数
 * @return  void
 */
void timed_task_handle(int32_t sig)
{
    time_printf();
    led_scan_process(TIMING_PERIOD_MS);
    buzzer_scan_process(TIMING_PERIOD_MS);
    //drm_drmns_process();
    key_scan_process(TIMING_PERIOD_MS);
}

/**
 * @brief   共享内存初始化，陈晓2023.3.10添加
 * @param   [in] void *arg
 * @return  void
 */
static void shm_init()
{
    int32_t shm_id;
    key_t key;

    if(access(CORE_SHM_PATH_VAR_DIR,F_OK) != 0)  //目录不存在
    {
        mkdir(CORE_SHM_PATH_VAR_DIR,0755);
    }
    if(access(CORE_SHM_PATH_VAR_FILE,F_OK) != 0)  //文件不存在
    {
        FILE *fp;
        fp = fopen(CORE_SHM_PATH_VAR_FILE,"w");
        if(fp == NULL)
        {
            log_i("create file failed");
            return;
        }
        fclose(fp);
    }

    key = ftok(CORE_SHM_PATH_VAR_FILE,0x03);
    if(key == -1)
    {
        log_i("key is invalid\n");
        return ;
    }
    shm_id = shmget(key,sizeof(shm_t),IPC_CREAT | 0600);
    if(shm_id == -1)
    {
        log_i("create share memory failed.\n");
        return;
    }
    log_i("create share memory successfully.\n");
}

/**
 * @brief   定时线程
 * @param   [in] void *arg
 * @return  void
 */
void *thread_timed(void *arg)
{
    shm_init();
    led_init();                     
    buzzer_init();
    key_init();
    dido_init();

    signal(SIGALRM, timed_task_handle);  // 关联定时
    set_timer();                         // 设置启动定时器

    while (1)
    {
        sleep(10);   
    }
}

/**
 * @brief   定时线程任务启动——创建定时线程
 * @return  void
 */
void timed_task_start(void)
{
	pthread_t timed_task;
	pthread_create(&timed_task, NULL, &thread_timed, NULL);
}



int32_t main(int32_t argc, char **argv)
{
    record_path_init();

    timed_task_start();

    while (1)
    {
        sleep(10);
    }
}
