/************************************************************************************
 *Project: Inverter-320kw-ARM
 *Description: daemon
 *Created on: 2022-10-10
 *Author: quliuliu
 ************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "sdk_print.h"


// 配置信息
#define GUARD_CONFIG_FILE   "/opt/conf/daemon.conf"
#define GET_LINE_SIZE 1024
#define PRINT_BUF_LEN 1024
#define MAX_SUPPORT_NUMBER 64


// bin文件路径, 支持控制最大64 
char *g_proc_array[MAX_SUPPORT_NUMBER] = {0};
pid_t g_proc_pid_array[MAX_SUPPORT_NUMBER] = {0};

//关闭、重定向文件描述符
void close_standard_io()
{
    int fd = -1;
    if ((fd = open("/dev/null", O_RDWR)) == -1) 
    {
        return;
    }
    // 标准输入重定向
    dup2(fd, STDIN_FILENO);     //STDIN_FILENO表示标准输入,对应的文件描述符分别是0
    dup2(fd, STDOUT_FILENO);    //STDOUT_FILENO表示标准输出,对应的文件描述符分别是1
    dup2(fd, STDERR_FILENO);    //STDERR_FILENO表示标准出错,对应的文件描述符分别是2
    close(fd);
    return;
}


// 创建子进程
void fork_sub_process(char *proc_file,  int i)
{
	struct stat st; //struct stat这个结构体是用来描述一个系统文件系统中的文件属性的结构
   
	if (lstat(proc_file, &st) < 0)  // 通过文件描述符获取文件对应的属性
    {
		print_error("%s", strerror(errno));
		return ;
	}

    pid_t pid = fork();
    if (pid > 0) 
    {
        g_proc_pid_array[i] = pid;
        return;
    } 
    else if (pid == 0) 
    {
        close_standard_io();
    
        execv(proc_file, NULL);
        // if (execv(proc_file, NULL) < 0) 
        // {
        //     print_error("execve %s - %s", proc_file,  strerror(errno));
        // }
    }
	else 
    {
        print_error("create sub process %s - %s", proc_file,  strerror(errno));
    }
    return;
}


// 检查子进程是否存在 
void check_proc_status()
{
    int i = 0;
    int ret = 0;

    while (1) 
    {
        for (i = 0; i < MAX_SUPPORT_NUMBER; i++) 
        {
            if (g_proc_array[i] != NULL) 
            {
                ret = kill(g_proc_pid_array[i], 0);//检验进程是否仍在执行
                if (ret != 0) 
                {
                   // print_debug("proc pid %d %d - %s is not exist", i, g_proc_pid_array[i], g_proc_array[i]);
					fork_sub_process(g_proc_array[i], i);
                } 
                else 
                {
                    // print_debug("proc pid %d %d - %s is exist", i, g_proc_pid_array[i], g_proc_array[i]);
                }
            }
            usleep(30 * 1000);
        }
    }
}


// 读取配置文件和启动进程
void start_sub_process()
{
    int     i    = 0;
    FILE   *fp   = NULL;
    char   *line = NULL;
    char   *ptr  = NULL;
    size_t  len  = 0;
    ssize_t read = 0;
    struct stat st;
    
    if (lstat(GUARD_CONFIG_FILE, &st) < 0)  //获取一些文件相关的信息
    {
        print_error("%s", strerror(errno));
        return;
    }

    fp = fopen(GUARD_CONFIG_FILE, "r+");
    if (!fp) 
    {
        print_error("%s", strerror(errno));
        return;
    }

    line = (char *)malloc(GET_LINE_SIZE);
    if (!line) 
    {
        print_error("%s", strerror(errno));
		fclose(fp);
        return;
    }

    bzero(line, GET_LINE_SIZE);

    while ((read = getline(&line, &len, fp)) != -1) 
    {
        ptr = strchr(line, '\n');
        if (ptr) 
        {
            *ptr = '\0';
        }
		print_debug("file name : %s", line);
        if (lstat(line, &st) < 0) 
        {
            print_error("%s - %s", line, strerror(errno));
            continue;
        } 
        else 
        {
            int  t_len  = strlen(line);
            char *t_proc = (char *)malloc(t_len + 1);
            if (!t_proc) 
            {
                continue;
            }
            memset(t_proc, 0, t_len + 1);
            strncpy(t_proc, line, t_len);
            if (i >= MAX_SUPPORT_NUMBER) 
            {
                print_debug("daemon.conf support max 64");
                free(t_proc);
            } 
            else 
            {
                g_proc_array[i] = t_proc;
                fork_sub_process(line, i);
				print_debug("start - pid=%d, %s", g_proc_pid_array[i], line);
                i++;
            }
        }
        bzero(line, len);
		usleep(30*1000);
    }
    free(line);
    fclose(fp);
    return;
}


void wait_sub_proc(int signo)
{
    int stat = 0;
    pid_t pid = 0;
    pid = wait(&stat);

    print_debug("child process  %d stop, pid=%d\n", signo, pid);  
    return;
}


int app_daemon()
{
    signal(SIGCHLD,  SIG_IGN);//忽略信号； signal(SIGCHLD,  &wait_sub_proc);//<**不要删除**区别SIG_IGN，wait为阻塞函数，在子进程中不能完整执行shell命令，需要等待信号返回>设置信号的对应动作；在一个进程终止或者停止时，将SIGCHLD信号发送给其父进程。系统默认将忽略此信号。如果父进程希望被告知其子系统的这种状态，则应捕捉此信号。信号的捕捉函数中通常调用wait函数以取得进程ID和其终止状态。

    start_sub_process();

    check_proc_status();

    return 0;
}

