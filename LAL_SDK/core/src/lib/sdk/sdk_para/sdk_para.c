/**
* @file		sdk_para.c
* @brief    参数存储关联模块
* @copyright Shouhang
* @author   yangwenjuan
* @note     2023.01.13 新拟制
* @version  V1.0   
* @date     2023/01/13
*/

#include "sdk_para.h"
#include <stdint.h>
#include <stdio.h>
#include<unistd.h>


#define SDK_PARA_DLOG(...) 		printf(__VA_ARGS__)		//后面改用统一的log

#define PARA_FILE_PATH_LEN		20	//实际长度19+1

typedef struct
{
    int32_t para_size; // 参数区大小(字节)
} para_attr_t;

// 参数属性列表
static para_attr_t g_para_attr[PARA_TYPE_NUM] = {0};

// 文件句柄列表
static FILE *gp_fs[PARA_TYPE_NUM] = {0};


/**
 * @brief  按参数区类型挂载参数
 *         多次挂载没有副作用
 * @param  [in] type 参数区类型（枚举中定义的值）
 * @param  [in] para_size 参数区大小（不能超过物理空间）
 * @retval 0  成功
 * @retval -1 参数区不存在
 * @retval -2 参数大小非法
 * @retval -3 文件系统错误
 */
int32_t sdk_para_init(uint32_t type, uint32_t para_size)	//linux平台，初始化时，①不需要限定文件参数字节大小; ② 文件不需要打开
{
	int32_t ret = 0;
	para_attr_t *p_para_attr;
	int8_t file_name[PARA_FILE_PATH_LEN];
	
	if (type >= PARA_TYPE_NUM)
	{
        ret = -1; // 传参错误
        goto __exit;		
	}

	if ((para_size > PARA_SIZE_LIMIT) || (para_size == 0))
	{
        ret = -2; 
        goto __exit;	
	}

	p_para_attr = &g_para_attr[type];
	
	if (gp_fs[type])
    { 
        SDK_PARA_DLOG("sdk_record_init() is multiple\r\n");
        goto __exit;	// 已经初始化，直接返回
    }

	sprintf(file_name, "/opt/para/para%03d", type);//sprintf(file_name, "\"/opt/para/para%03d\"", type);	//加"会出错
	if(access(file_name, F_OK) != 0)  	//判断对应参数文件是否存在，不存在则先创建文件 
	{
		gp_fs[type] = fopen(file_name, "wb+");
	}
	else
	{
		gp_fs[type] = fopen(file_name, "rb+");//sdk_fs_open(g_para_file[type], FS_OPEN_ALWAYS | FS_WRITE | FS_READ);		
	}

	if (gp_fs[type] == NULL)	//打开文件失败
	{
		ret = -3;
		goto __exit;
	}
	p_para_attr->para_size = para_size;
	
__exit:
	if (ret < 0)
	{
		SDK_PARA_DLOG("Function[%s] Operation failed, ret = %d!\r\n", __func__, ret);
	}

	return ret;	
}

/**
 * @brief  按参数区类型写内容
 * @param  [in] type 参数区类型（枚举中定义的值）
 * @param  [in] offset 参数中的位置 (offset+len < para_size)
 * @param  [in] buf 内容
 * @param  [in] len 内容长度 (offset+len < para_size)
 * @retval >=0  成功写入的字节数
 * @retval -1 参数区不存在
 * @retval -2 参数区未挂载
 * @retval -3 写入超出长度
 * @retval -4 -5 -6 文件系统错误
 */
int32_t sdk_para_write(uint32_t type, uint32_t offset, uint8_t *p_buf, uint32_t len)
{
	int32_t ret = 0;
	para_attr_t *p_para_attr;
	FILE *p_file = NULL;

	if (type >= PARA_TYPE_NUM)     				//参数区不存在
    {
        ret = -1; // 传参错误
        goto __exit;
    }
	
	p_para_attr = &g_para_attr[type];	
	if ((gp_fs[type] == NULL) || (p_buf == NULL) || (p_para_attr->para_size == 0))   //传参错误；参数区未挂载
    {
        ret = -2; 
        goto __exit;
    }	

	if ((offset + len) > p_para_attr->para_size)			//写入超出长度
    {
        ret = -3; 
        goto __exit;
    }
	
	ret = fseek(gp_fs[type], offset, SEEK_SET);
	if (ret != 0)
	{
		ret = -5;
		goto __exit;
	}	
	ret = fwrite(p_buf, 1, len, gp_fs[type]);
	if (ret != len)//(ret < 0)
	{
		ret = -6;
		goto __exit;
	}	

__exit:
	if (ret < 0)
	{
		SDK_PARA_DLOG("Function[%s] Operation failed, ret = %d!\r\n", __func__, ret);
	}
	return ret;		
}

/**
 * @brief  按参数区类型读参数
 * @param  [in]  type 参数区类型（枚举中定义的值）
 * @param  [in]  offset 参数中的位置 (offset+len < para_size)
 * @param  [out] buf 内容
 * @param  [in]  len 内容长度 (offset+len < para_size)
 * @retval =0  成功
 * @retval -1 参数区不存在
 * @retval -2 参数区未挂载
 * @retval -3 写入超出长度
 * @retval -4 -5 -6 文件系统错误
 */
int32_t sdk_para_read(uint32_t type, uint32_t offset, uint8_t *p_buf, uint32_t len)
{
	int32_t ret = 0;
	para_attr_t *p_para_attr;
	FILE *p_file = NULL;
	
	if (type >= PARA_TYPE_NUM)     				//参数区不存在
    {
        ret = -1; // 传参错误
        goto __exit;
    }
	p_para_attr = &g_para_attr[type];
	if ((gp_fs[type] == NULL) || (p_buf == NULL) || (p_para_attr->para_size == 0))        //传参错误；参数区未挂载
    {
        ret = -2; 
        goto __exit;
    }	
	if ((offset + len) > p_para_attr->para_size)	//超出长度
    {
        ret = -3; 
        goto __exit;
    }
	
	ret = fseek(gp_fs[type], offset, SEEK_SET);//sdk_fs_lseek(p_file, offset);
	if (ret != 0)
	{
		ret = -5;
		goto __exit;
	}	
	ret = fread(p_buf, 1, len, gp_fs[type]);//sdk_fs_read(p_file, p_buf, len);
	if (ret != len)//(ret < 0)
	{
		ret = -6;
		goto __exit;
	}	

__exit:
	if (ret < 0)
	{
		SDK_PARA_DLOG("Function[%s] Operation failed, ret = %d!\r\n", __func__, ret);
	}
	
	return ret;		
}

/**
 * @brief  同步参数的数据到flash
 * @param  [in] type 参数区类型（取值范围：<PARA_TYPE_NUM）
 * @retval 0  成功
 * @retval -1 失败
 */
int32_t sdk_para_sync(uint32_t type)
{
	int32_t ret = 0;

    if (type >= PARA_TYPE_NUM)
    {
        ret = -1; // 记录类型不存在，返回错误
        goto __exit;
    }

	sync();

__exit:
	if (ret < 0)
	{
		SDK_PARA_DLOG("Function[%s] Operation failed, ret = %d!\r\n", __func__, ret);
	}
	return ret;		
}



