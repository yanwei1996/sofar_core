
/**
* @file		sdk_fs.c
* @brief    文件操作有关的函数
* @copyright Shouhang
* @author   yangwenjuan
* @note     2023.01.29 新拟制
* @version  V1.0   
* @date     2023/01/29
*/
 
#include "sdk_fs.h"
#include "errors.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>

#define log_e(...)		printf(__VA_ARGS__)	    //fixme 后面easylog可以用后，屏蔽掉  
#define FS_USR_PATH_NAME      "/user/"                       //2023.02.14评审会议确定，应用层仅限定在/user/路径下操作
#define FS_PATH_NAME_LEN       (PATH_NAME_LEN_LIMIT + 5)     //文件路径名最大长度限制
#define FS_USR_PATH_NAME_LEN    PATH_NAME_LEN_LIMIT      //允许用户传入的文件路径名的最大长度限制  

//内部函数声明
static int32_t fs_mkdir_multi_level(const char *path, mode_t mode);


// typedef enum
//  {
//      FS_READ = (1 << 0),            //读操作
//      FS_WRITE = (1 << 1),           //写操作

//      FS_OPEN_EXISTING = (1 << 2),   //打开文件，如果文件不存在返回失败             PS:文件存在，按rt+/rt打开；文件不存在，返回NULL；      
//      FS_OPEN_ALWAYS = (1 << 5),     //打开文件，如果文件不存在则创建该文件         PS:文件存在，按rt+/rt打开；文件不存在，按wt+/wt 打开     
//      FS_OPEN_APPEND = (1 << 6),     //打开文件，和FS_OPEN_ALWAYS一样，但是每次写头跳转到文件尾部    PS:文件存在，按rt+/rt打开；文件不存在，按at+/at打开

//      FS_CREATE_NEW = (1 << 3),      //创建新文件，如果文件已存在则返回失败         PS:文件存在，返回NULL；文件不存在，按wt+/wt 打开
//      FS_CREATE_ALWAYS = (1 << 4),   //创建新文件，如果文件已存在则清空该文件内容    PS:按wt+/wt 打开
//  }fs_open_mode_e;


 /** 
  * @brief        对文件系统初始化进行相关操作，如：挂载文件系统。 
  * @return       [int32_t] 执行结果
  * @retval       >=0成功
  * @retval       <0失败
  */ 
 int32_t sdk_fs_init(void)
 {
	return 0;	 
 }

 /** 
  * @brief        对指定路经的文件按照设定的模式进行打开操作
  * @param        [in] path 文件所在的路经及文件名，可包含盘符(路径名称不超过123个字符)
  * @param        [in] mode 打开模式，不同模式可进行位或操作
  * @return       [fs_t] 执行结果
  * @retval       >0 成功，返回指向该流的文件指针
  * @retval       <0 失败，
  */ 
 fs_t *sdk_fs_open(const int8_t *path, uint16_t mode)	// 限定路径—— 2023.02.14评审决定强制加/usr   //open()函数， ioctrl 打开缓存？暂不采用。
 {
    FILE *fp=NULL;
    int32_t ret = 0;
    int8_t path_name[FS_PATH_NAME_LEN] = FS_USR_PATH_NAME;
    int8_t open_mode[16];
  
    // 判断参数、路径合法性，不合法则返回错误；
	if ((path == NULL) || (mode == 0))
    {
		goto __exit;
    }

    if (strlen(path) > FS_USR_PATH_NAME_LEN)    // 路径名太长
    {
        goto __exit;
    }
    strncat(path_name, path, FS_USR_PATH_NAME_LEN); // 强制限定，用户传入的路径前，再加/usr


    if (mode & FS_CREATE_ALWAYS)     // 创建新文件，文件已存在，清空内容
    {
        if (mode & FS_READ)
        {
            strcpy(open_mode, "wt+");
        }
        else
        {
           strcpy(open_mode, "wt");          
        }
    }
    else if (mode & FS_CREATE_NEW)   // 创建新文件，文件已存在，返回失败 NULL  
    {
        if (access(path,  F_OK) == -1)  
        {
            if (mode & FS_READ)
            {
                strcpy(open_mode, "wt+");
            }
            else
            {
               strcpy(open_mode, "wt");        
            }
        }
    }
    else if (mode & FS_OPEN_APPEND)   // 打开文件(有则打开，无则创建)，但是每次写头跳转到文件尾部 
    {
        if (mode & FS_READ)
        {
            strcpy(open_mode, "at+");  // 读写模式————读可以任意位置开始读，写则必然写到文件末尾
        }
        else
        {
            strcpy(open_mode, "at");   // 只写模式————只能写到文件末尾         
        }
    }
    else if (mode & FS_OPEN_ALWAYS)   // 打开文件(有则打开，无则创建)
    {
        if (access(path,  F_OK) != -1)  // 文件存在
        {
            if (mode & FS_WRITE)
            {
                strcpy(open_mode, "rt+");
            }
            else
            {
                strcpy(open_mode, "rt");           
            }
        }
        else
        {
            if (mode & FS_READ)
            {
                strcpy(open_mode, "wt+");  // 读写模式
            }
            else
            {
                strcpy(open_mode, "wt");   // 只写模式         
            }
        }
    }
    else if (mode & FS_OPEN_EXISTING)      // 打开文件，如果文件不存在则返回失败
    {
        if (access(path,  F_OK) != -1)  
        {
            if (mode & FS_WRITE)
            {
                strcpy(open_mode, "rt+");    // 读写模式
            }
            else
            {
                strcpy(open_mode, "rt");     // 只读模式         
            }
        }
    }
    else if (mode & FS_READ)
    {
        if (access(path,  F_OK) == -1)  
        {
            if (mode & FS_WRITE)
            {
                strcpy(open_mode, "rt+");
            }
            else
            {
                strcpy(open_mode, "rt");        
            }
        }
    }
    else if (mode & FS_WRITE)
    {
        if (access(path,  F_OK) == -1)  
        {
            if (mode & FS_READ)
            {
                strcpy(open_mode, "wt+");
            }
            else
            {
                strcpy(open_mode, "wt");        
            }
        }
    }

    if (strlen(open_mode))  // 模式有效
    {
       fp = fopen(path_name, open_mode);     // 按对应模式打开文件
    }

__exit:
    if (fp == NULL)
    {
        log_e("Function[%s] Operation failed, line = %d!\r\n", __func__, __LINE__);
    }
    return (fs_t *)fp;	 
 }

 /** 
  * @brief        关闭文件指针把指向的文件
  * @param        [in] p_fs 需要关闭文件的文件指针
  * @return       [int32_t] 执行结果
  * @retval       =0 关闭成功
  * @retval       < 关闭失败，返回错误代码
  */
 int32_t sdk_fs_close(fs_t *p_fs)
 {
	int32_t ret = ERR_NONE;
    int32_t fd;

	if (&p_fs->file == NULL)
	{
		ret = ERR_PARA;
	}
    else
    {
        fd = fileno(&p_fs->file);
        printf("fd = %d\r\n", fd);
        if (fd > 0)
        {
            ret = fcntl(fd, F_GETFL);   // F_GETFL 取得文件描述符状态旗标，此旗标为open（）的参数flags
            if (ret > 0)
            {
                ret = fclose(&p_fs->file);      
            }
            else
            {
                ret = ERR_PARA; // 文件未打开，报错
            }
        }
        else
        {
            ret = ERR_PARA;
        }	    
    }

    if (ret < 0)
    {
    	log_e("Function[%s] Operation failed, ret = %d!\r\n", __func__, ret);
    }
	return ret;	 
 }

 /** 
  * @brief        向文件指针所指向的流中读取指定长度的数据
  * @param        [in] p_fs 已打开的文件指针
  * @param        [out] buff 指向欲存放读取进来的数据空间
  * @param        [in] len 欲读取数据长度
  * @return       [int32_t] 执行结果
  * @retval       >=0 返回实际读取到的数据长度，单位字节
  * @retval       <0 读取失败
  */  
 int32_t sdk_fs_read(fs_t *p_fs, void *buff, uint32_t len)
 {
	int32_t ret = ERR_NONE;
	
	if ((&p_fs->file == NULL) || (buff == NULL) || (len == 0))
	{
		ret = ERR_PARA;
	}
    else 
    {
        ret = fileno(&p_fs->file);
        if (ret < 0)   // 转换成文件描述符失败，说明文件流指针无效
        {
            ret = ERR_PARA;
        }
        else
        {
            ret = fread(buff, 1, len, &p_fs->file);	//预读len个字节，read_num为实际读到的字节数(遇文件尾或出错，ret<len)
            // if (read_num != len)
            // {
                // ret = ferror(&p_fs->file);
                // if (ret)
                // {
                    // goto __exit;
                // }
            // }
        }
    }

    if (ret < 0)
    {
    	log_e("Function[%s] Operation failed, ret = %d!\r\n", __func__, ret);
    }
	return ret;	 	 
 }

 /** 
  * @brief        将数据写入到文件指针所指向的数据流中
  * @param        [in] p_fs 已打开的文件指针
  * @param        [in] buff 指向欲写入的数据地址
  * @param        [in] len 欲写入数据的长度
  * @return       [int32_t]执行结果
  * @retval       >=0 返回实际写入的数据长度，单位字节
  * @retval       <0 读取失败
  */  
 int32_t sdk_fs_write(fs_t *p_fs, void *buff, uint32_t len)
 {
	int32_t ret = ERR_NONE;
	
	if (&p_fs->file == NULL || buff == NULL || len == 0)   //256m
	{
		ret = ERR_PARA;
	}
    else
    {
        ret = fileno(&p_fs->file);
        if (ret < 0)   // 转换成文件描述符失败，说明文件流指针无效
        {
            ret = ERR_PARA;
        }
        else
        {
        	ret = fwrite(buff, 1, len, &p_fs->file);	
        }
    }

    if (ret < 0)
    {
    	log_e("Function[%s] Operation failed, ret = %d!\r\n", __func__, ret);
    }
	return ret;	  
 }
  
 /** 
  * @brief        移动文件流的读写位置
  * @param        [in] p_fs 已打开的文件指针
  * @param        [in] offset 从文件头部起始的偏移地址
  * @return       [int32_t] 执行结果
  * @retval       =0 成功
  * @retval       <0 错误，值对应的是错误代码
  * @note         从文件头部为参考进行偏移
  */  
 int32_t sdk_fs_lseek(fs_t *p_fs, uint32_t offset)
 {
	int32_t ret = ERR_NONE;
	
	if (&p_fs->file == NULL)
	{
		ret = ERR_PARA;
	} 
	else
    {
        ret = fileno(&p_fs->file);
        if (ret < 0)   // 转换成文件描述符失败，说明文件流指针无效
        {
            ret = ERR_PARA;
        }
        else 
        {
            ret = fseek(&p_fs->file, offset, SEEK_SET);	// 定位到文件头开始偏移offset字节的位置      
            if (ret != 0)	// 失败
            {
                ret = ferror(&p_fs->file);   //返回值为0，未出错；非0，出错
            }
        }
    }

    if (ret != 0)
    {
    	log_e("Function[%s] Operation failed, ret = %d!\r\n", __func__, ret);
    }
	return ret;		 
 }
  
 /** 
  * @brief        读取文件的大小
  * @param        [in] p_fs 已打开的文件指针
  * @return       [int32_t] 执行结果
  * @retval       >0 获取成功，返回文件大小，单位字节
  * @retval       <0 获取失败
  */  
 int32_t sdk_fs_get_size(fs_t *p_fs)
 {
	int32_t ret = ERR_NONE;
    int32_t fd;
    struct stat file_stat;
	
	if (&p_fs->file == NULL)
	{
		ret = ERR_PARA;
	}  
    else
    {
        // //  法1：该法不采用，因为会改变文件指针位置
        // fseek(&p_fs->file, 0, SEEK_END);
        // ret = ftell(&p_fs->file);	//获取文件尾指针偏移量，即文件大小

        // 法2
        ret = fileno(&p_fs->file);
        if (ret < 0)   // 转换成文件描述符失败，说明文件流指针无效
        {
            ret = ERR_PARA;
        }
        else
        {
            fd = ret;
            ret = fstat(fd, &file_stat);
            if (ret == 0)
            {
                ret = file_stat.st_size;
            }
        }
    }

    if (ret < 0)
    {
    	log_e("Function[%s] Operation failed, ret = %d!\r\n", __func__, ret);
    }

	return ret;		
 }
  
 /** 
  * @brief        保存文件流数据到存储介质
  * @param        [in] p_fs 已打开的文件指针
  * @return       [int32_t] 执行结果
  * @retval       =0成功
  * @retval       <0失败，值对应不同的错误代码
  */  
 int32_t sdk_fs_file_sync(fs_t *p_fs)
 {
	int32_t ret = ERR_NONE;
    int32_t fd;
	
	if (&p_fs->file == NULL)
	{
		ret = ERR_PARA;
	}  
    else
    {
        fd = fileno(&p_fs->file);
        if (fd < 0)
        {
            ret = ERR_PARA;
        }
        else
        {
            ret = fsync(fd);	//将系统缓冲区（内存中）的数据写入到文件系统（磁盘）中           
        }
    }
	
    if (ret < 0)
    {
    	log_e("Function[%s] Operation failed, ret = %d!\r\n", __func__, ret);
    }
	return ret;		 
 }
  
 /** 
  * @brief        删除路经中指定的文件
  * @param        [in] path 文件所在的路经及文件名，可包含盘符
  * @return       [int32_t] 执行结果
  * @retval       =0 成功
  * @retval       非0 失败，值对应不同的错误代码
  */  
 int32_t sdk_fs_remove(const int8_t *path)
 {
	int32_t ret;
    int8_t path_name[FS_PATH_NAME_LEN] = FS_USR_PATH_NAME;
	
	if (path == NULL)
	{
		ret = ERR_PARA;
	}  
    else
    {
        strncat(path_name, path, FS_USR_PATH_NAME_LEN); //强制限定，用户传入的路径前，再加/usr
        ret = remove(path);	
        if (!ret)
        {
           ret = errno;  
           printf("errno = %d, %s\n",errno, strerror(errno));           
        }
    }
	
    if (ret != 0)
    {
    	log_e("Function[%s] Operation failed, ret = %d!\r\n", __func__, ret);
    }
	return ret;	 	 
 }

 /** 
  * @brief        移动目录或者修改文件名字（目录必须存在）
  * @param        [in] oldpath 文件所在的旧路经及文件名，可包含盘符
  * @param        [in] newpath 新路经及文件名，可包含盘符
  * @return       [int32_t]执行结果
  * @retval       =0 成功
  * @note         <0 失败，值对应不同的错误代码
  */  
 int32_t sdk_fs_rename(const int8_t *oldpath, const int8_t *newpath)
 {
	int32_t ret = ERR_NONE;
    int8_t old_path_name[FS_PATH_NAME_LEN] = FS_USR_PATH_NAME;
    int8_t new_path_name[FS_PATH_NAME_LEN] = FS_USR_PATH_NAME;

	if((oldpath == NULL) || (newpath == NULL))
	{
		ret = ERR_PARA;
	}  
    else 
    {
        strncat(old_path_name, oldpath, FS_USR_PATH_NAME_LEN); 
        strncat(new_path_name, newpath, FS_USR_PATH_NAME_LEN); 
        ret = rename(old_path_name, new_path_name);	
        if (!ret)   // 失败
        {
            ret = errno;
            log_e("rename() failed--- errno = %d, %s; old_path = %s, new_path = %s\r\n",errno, strerror(errno), old_path_name, new_path_name);
        }
    }
	
    if( ret != ERR_NONE)
    {
    	log_e("Function[%s] Operation failed, ret = %d!\r\n", __func__, ret);
    }
	return ret; 
 }

/**
* @brief		创建目录（支持多级目录递归创建)
* @param		[in] path 创建的目录路径
* @param		[in] mode 要创建的目录权限
* @return		结果
* @retval		0 成功
* @retval		< 0 失败
*/
 int32_t sdk_fs_mkdir(const char *path, uint32_t mode)
 {
    int32_t ret = ERR_NONE;
    int8_t path_name[FS_PATH_NAME_LEN] = FS_USR_PATH_NAME;

    if (path == NULL)
    {
		ret = ERR_PARA;
		goto __exit;
    }

    strncat(path_name, path, FS_USR_PATH_NAME_LEN); 

    ret = access(path_name, F_OK);
    if (ret != -1)  //路径存在，不需要重复创建
    {
        ret = ERR_NONE;
    }
    else
    {
        ret = fs_mkdir_multi_level(path_name, mode);     //自查 库函数递归  
    }

__exit:
    if (ret < 0)
    {
    	log_e("Function[%s] Operation failed, ret = %d!\r\n", __func__, ret);
    }
	return ret; 
 }

 /**
* @brief		确定文件或者文件夹的访问权限
* @param		[in] path 文件的目录路径
* @param		[in] mode 要判断的模式(具体见fs_test_mode_e，可以是各种值组合)
* @return		结果
* @retval		0 指定的访问权限存在
* @retval		-1 指定的访问权限不存在
* @retval		ERR_PARA 参数错误
*/
 int32_t sdk_fs_access(const int8_t *p_path, int32_t mode)
 {
    int32_t ret = ERR_NONE;

    if ((p_path == NULL) || (mode > (F_OK|X_OK|R_OK|W_OK)))
    {
		ret = ERR_PARA;
		log_e("Function[%s] Operation failed, ret = %d!\r\n", __func__, ret);
    }
    else
    {
        ret = access(p_path, mode);
    }
	return ret; 
 }
  

/**
* @brief		创建多级目录——非SDK接口函数，内部子函数
* @param		[in] path 文件路径
* @return		结果
* @retval		0 成功
* @retval		< 0 失败
*/
static int32_t fs_mkdir_multi_level(const char *path, uint32_t mode)
{
    int32_t i = 0;
    int32_t len = 0;
    int32_t ret = ERR_NONE;
    int8_t buff[PATH_NAME_LEN_LIMIT];

    //判断路径长度大小合法性
    if(strlen(path) > sizeof(buff))
    {
        log_e("%s %d: pathname %s to long.\n", __func__, __LINE__, path);
        return -1;
    }

    memset(buff, 0, sizeof(buff));
    len = strlen(path);
    for(i = 0; i < len; i++)
    {
        buff[i] = path[i];
        if(buff[i] == '/')
        {
            if(access(buff, F_OK) != 0)
            {
                umask(0);   //
                ret = mkdir(buff, mode);
                if(ret == -1)
                {
                    log_e("%s %d: mkdir %s fail.\n", __func__, __LINE__, buff);
                    return ret;
                }
            }
        }
    }

    if(access(buff, F_OK) != 0)
    {
        ret = mkdir(buff, mode);
        if(ret == -1)
        {
            log_e("%s %d: mkdir %s fail.\n", __func__, __LINE__, buff);
            return ret;
        }
    }
    return ret;
}
