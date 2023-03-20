//  gcc file_test.c ../sdk_file.c -o file_test -I ../../../include
//  arm-

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>
#include "../../../include/sdk_file.h"

typedef struct user_info
{
	char name[16];
	char password[16];
	char receMail[32]; 		//接收邮箱
	char permission[10];	//操作权限 [0]:0/1=管理员/
}user_t;

int read_file_to_struct(char *path, void *stc, int len, long int offset)
{
	FILE *fp=NULL;
	if((fp=fopen(path,"ab+")) == NULL)
	{
		printf("fopen error [%s:%s:%d] %s\n", __FILE__,__func__, __LINE__, strerror(errno));
		return -1;
	}

	// fseek() 一般用于二进制文件，在文本文件中由于要进行转换，计算的位置有时会出错
	fseek(fp, offset, SEEK_SET);//SEEK_CUR

	if(fread(stc, len, 1, fp) != 1)
	{
		printf("fread error [%s:%s:%d] %s\n", __FILE__,__func__, __LINE__, strerror(errno));
		fclose(fp);
		return -1;
	}

	fclose(fp);

	return 0;
}

int write_struct_to_file(char *path, void *stc, int len, long int offset)
{
    FILE *fp=NULL;
	if((fp=fopen(path,"rb+")) == NULL)
	{
		printf("fopen error [%s:%s:%d] %s\n", __FILE__,__func__, __LINE__, strerror(errno));
		return -1;
	}

	// fseek() 一般用于二进制文件，在文本文件中由于要进行转换，计算的位置有时会出错
	fseek(fp, offset, SEEK_SET);//SEEK_CUR

	if(fwrite(stc, len, 1, fp) != 1)
	{
		printf("fwrite error [%s:%s:%d] %s\n", __FILE__,__func__, __LINE__, strerror(errno));
		fclose(fp);
		return -1;
	}

	fclose(fp);

	return 0;
}


void main()
{
	user_t r_user={0};
	user_t w_user={"aaa", "bbb", "ccc", "ddd"};
	printf("user_t size is %ld\n", sizeof(w_user));
	write_struct_to_file("./userinfo", &w_user, sizeof(w_user), sizeof(r_user)*2);
	user_t w_user1={"111", "222", "333", "444"};
	write_struct_to_file("./userinfo", &w_user1, sizeof(w_user1), sizeof(r_user)*3);

	read_file_to_struct("./userinfo", &r_user, sizeof(r_user), sizeof(r_user)*0);
	printf("name=%s\n", r_user.name);
	printf("password=%s\n", r_user.password);
	read_file_to_struct("./userinfo", &r_user, sizeof(r_user), sizeof(r_user)*1);
	printf("name=%s\n", r_user.name);
	printf("password=%s\n", r_user.password);

	return;
}

