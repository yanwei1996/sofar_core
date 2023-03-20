/************************************************************************************
 *Description: sdk_file
 *Created on: 2022-06-20
 *Author: quliuliu
 ************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>
#include "data_types.h"
#include "sdk_file.h"

/****************************************************************************
* 功  能   : 获取文件长度
* 输  入   : path为文件路径或名称
* 输  出   : 
* 返  回   ：文件长度
*/
uint64_t sdk_file_get_size(const int8_t *path)
{
	uint64_t filesize = -1;	
	struct stat statbuff;
	if(stat(path, &statbuff) < 0){
		return filesize;
	}else{
		filesize = statbuff.st_size;
	}
	return filesize;
}

/****************************************************************************
* 功  能   : 每次从文件中任意地方读取一块数据到对应的数据类型对象中。
* 输  入   : path为文件路径或名称，stc为空指针，length为结构体的长度， offset为从文件起始位移大小
* 输  出   : data为获取到数据的指针
* 返  回   ：0为正常，-1为异常
*/
int32_t sdk_file_read(const int8_t *path, uint64_t offset, int32_t length, void *data)
{
	FILE *fp=NULL;
	if((fp=fopen(path,"r")) == NULL)
	{
		printf("%s----[%s:%s:%d] %s\r\n", path, __FILE__,__func__, __LINE__, strerror(errno));
		return -1;
	}

	fseek(fp, offset, SEEK_SET);//fseek() 一般用于二进制文件，在文本文件中由于要进行转换，计算的位置有时会出错

	if(fread(data, length, 1, fp) != 1)
	{
		printf("%s----[%s:%s:%d] %s\r\n", path, __FILE__,__func__, __LINE__, strerror(errno));
		fclose(fp);
		return -1;
	}

	fclose(fp);

	return 0;
}

/****************************************************************************
* 功  能   : 如果文件存在，不清空文件；如果文件不存在，就创建文件。并向文件中写数据到任意位置
* 输  入   : path为文件路径或名称，offset为从文件起始位移大小，length为数据的长度，data为空指针
* 输  出   : 
* 返  回   ：0为正常，-1为异常
*/
int32_t sdk_file_write(const int8_t *path, uint64_t offset, int32_t length, void *data)
{
    FILE *fp=NULL;

	if((fp=fopen(path,"ab+")) == NULL)
	{
		printf("%s----[%s:%s:%d] %s\r\n", path, __FILE__,__func__, __LINE__, strerror(errno));
		return -1;
	}
	fclose(fp);

	if((fp=fopen(path,"rb+")) == NULL)
	{
		printf("%s----[%s:%s:%d] %s\r\n", path, __FILE__,__func__, __LINE__, strerror(errno));
		return -1;
	}

	fseek(fp, offset, SEEK_SET);// fseek() 一般用于二进制文件，在文本文件中由于要进行转换，计算的位置有时会出错

	if(fwrite(data, length, 1, fp) != 1)
	{
		printf("%s----[%s:%s:%d] %s\r\n", path, __FILE__,__func__, __LINE__, strerror(errno));
		fclose(fp);
		return -1;
	}

	fclose(fp);

	return 0;
}



/****************************************************************************
* 功  能   : 从文件中某行"名称=xxx"中读取xxx
* 输  入   : path为文件路径或名称，row要读的第几行
* 输  出   : value为读到的值
* 返  回   ：0为正常，-1为异常
*/
int32_t sdk_file_read_row(const int8_t *path, int32_t row, int8_t *value)
{
	int8_t cmd[128]={0};
	int8_t tmp[128]={0};
	sprintf(cmd, "sed -n '%dp' %s | cut -d= -f2", row, path);

	FILE *fp = NULL;
	if((fp=popen(cmd, "r")) == NULL)
	{
		printf("%s----[%s:%s:%d] %s\r\n", path, __FILE__,__func__, __LINE__, strerror(errno));
		return -1;
	}

	if(fgets(tmp, 32, fp) == NULL)
	{
		printf("%s----[%s:%s:%d] %s\r\n", path, __FILE__,__func__, __LINE__, strerror(errno));
		pclose(fp);
		return -1;
	}

	//去除名称中的\n
	int32_t len = strlen(tmp);
	if((tmp[len-1] == '\r')||(tmp[len-1] == '\n'))
	{
		tmp[len-1]='\0';
	}

	strcpy(value, tmp);
	len = strlen(value);
	pclose(fp);
	return 0;
}

/****************************************************************************
* 功  能   : 向文件中某行"名称=xxx"中写入xxx
* 输  入   : path为文件路径或名称，row要写的第几行，str写入的内容
* 输  出   : 无
* 返  回   ：0为正常，-1为异常
*/
int32_t sdk_file_write_row(const int8_t *path, int32_t row, int8_t *str)
{
	int8_t cmd[128]={0};
	int8_t tmp[128]={0};
	int32_t line=0;
	
	//检查文件行数
	sprintf(cmd, "wc -l %s | cut -d ' ' -f 1", path);
	FILE *fp = NULL;
    if((fp=popen(cmd, "r")) == NULL)
    {
        printf("%s----[%s:%s:%d] %s\r\n", path, __FILE__,__func__, __LINE__, strerror(errno));
        return -1;
    }
	if(fgets(tmp, 32, fp) == NULL)
	{
		printf("%s----[%s:%s:%d] %s\r\n", path, __FILE__,__func__, __LINE__, strerror(errno));
		pclose(fp);
		return -1;
	}
	line=atoi(tmp);
    pclose(fp);

	//如果写入行为空,在使用sed指令时,则需要先添加非空行
	while(line<row+1)
	{
		memset(cmd, 0, sizeof(cmd));
		sprintf(cmd, "echo >> %s", path);
		fp = NULL;
		if((fp=popen(cmd, "r")) == NULL)
		{
			printf("%s----[%s:%s:%d] %s\r\n", path, __FILE__,__func__, __LINE__, strerror(errno));
			return -1;
		}
		pclose(fp);

		line++;
	}

	//向指定行写入数据
	memset(cmd, 0, sizeof(cmd));
	sprintf(cmd, "sed -i '%dc%s' %s", row, str, path);	 //sprintf(cmd, "sed -i '%ds/=.*/=%s/' %s", row, str, path);
	fp = NULL;
    if((fp=popen(cmd, "r")) == NULL)
    {
        printf("%s----[%s:%s:%d] %s\r\n", path, __FILE__,__func__, __LINE__, strerror(errno));
        return -1;
    }
    pclose(fp);

	return 0;
}

/****************************************************************************
* 功  能: 删除数据库
* 输  入: 文件路径
* 输  出: 
* 返  回：0为正常，-1为异常
****************************************************************************/
int32_t sdk_file_remove(int8_t *path)
{
	int32_t ret = 0;

	ret = remove(path);	
	if(ret)
	{
		printf("%s----[%s:%s:%d] %s\r\n", path, __FILE__,__func__, __LINE__, strerror(errno));
	}
	return ret;
}

