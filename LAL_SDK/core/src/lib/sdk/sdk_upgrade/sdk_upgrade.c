#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "stdint.h"
#include "sdk_upgrade.h"


#pragma pack(push,1)        //作用：是指把原来对齐方式设置压栈，并设新的对齐方式设置为一个字节对齐
typedef struct firmware_msg        // 各固件信息
{
	    uint8_t filetype[1]; //固件文件类型
    	uint8_t platform[1]; //固件芯片角色
        int8_t name[56];                //固件名称
        uint8_t addr[4];                //固件起始偏移地址
        uint8_t length[4];              //固件长度
        int8_t version[20];             //固件版本号
        int8_t reserved[18];         //预留
}firmware_msg_t;
typedef struct signing_msg        // 固件包签名信息
{
        int8_t protocol_version[1];        //协议版本号
        int8_t company[16];             //公司名称
        int8_t Product_type_code[1];		//产品类型编码
        int8_t Product_model_code[2];      //产品机型编码
        int8_t package_name[52];        //固件包名称
        uint8_t release_time[6];        //固件包打包时间日期
        uint8_t module_total[1];           //固件模块数量
        int8_t reserved1[58];           //预留
        firmware_msg_t firmware_msg[18];//固件类型、名称、起始偏移地址、长度、版本号
        int8_t reserved2[31];           //预留
        uint8_t package_length[4];      //固件包总长
        uint32_t package_crc32;         //CRC32
}package_signing_msg_t;
#pragma pack(pop)        //作用：恢复对齐状态




char Extract_the_file_contents(char *SourceFilePath,char *OutFilePath, long offset, long ContentLength)
{
	FILE *fp;
	long start=0,end=0,size = 0,fileSize=0;
	char *ar ;
 
	//二进制方式打开文件
	fp = fopen(SourceFilePath,"rb");
	if(NULL == fp)
	{
		printf("Error:Open input file fail!\n");
		return -1;
	}
 
	//求得文件的大小
	fseek(fp, 0, SEEK_END);
	fileSize = ftell(fp); printf("file size = %d \n",fileSize);
	rewind(fp);
 
	//求得size的大小
	fseek(fp, offset, SEEK_SET);
	start = ftell(fp);
	fseek(fp, ContentLength, SEEK_CUR);
	end = ftell(fp);
	if(end>fileSize) {printf("Error: input size too big !\n"); return -1;}
	size = end-start;
	//rewind(fp);
	fseek(fp, offset, SEEK_SET);
	//申请一块能装下整个文件的空间
	ar = (char*)malloc(sizeof(char)*size);
	//读文件
	fread(ar,1,size,fp);//每次读一个，共读size次
	fclose(fp);
	
	
	//二进制方式打开文件
	fp = fopen(OutFilePath,"wb");
	if(NULL == fp)
	{
		printf("Error:Open out file fail!\n");
		return -1;
	}
	fwrite(ar, 1, size, fp);
	fclose(fp);
	
	free(ar);
	
	return 0;
 
}

char ParsingInformation(char *PackPath, package_signing_msg_t *p_info)
{
	FILE *fp;
	long start=0,end=0,size = 0,fileSize=0;
	char *ar ;
 
	//二进制方式打开文件
	fp = fopen(PackPath,"rb");
	if(NULL == fp)
	{
		printf("Error:Open pack file fail!\n");
		return -1;
	}
 
	fseek(fp,-2048L,SEEK_END);
	
	//读文件
	fread(p_info,1,2048,fp);//每次读一个，共读size次
	fclose(fp);
}



int Unpack(uint8_t *dir,uint8_t *type)
{
	package_signing_msg_t info;
	uint32_t a=0,b=0;
	uint8_t num=0,have=0;
	ParsingInformation(dir,&info);
	
//	printf("info.module_total[0] = %d !!!\r\n",info.module_total[0]);
//	printf("type is %s\n",type);
//	for(num=0;num<18;num++)
//		{
//			printf("name is %s\n",info.firmware_msg[num].name);
//		}
	for(num=0;num<info.module_total[0];num++)
	{
		/*if(memcmp(info.firmware_msg[num].name, type, strlen(type))==0)
		{ 
			have=1;
			break;
		}*/
		;
		if(strstr(info.firmware_msg[num].name, type)!=NULL)
		{ 
			have=1;
			break;
		}
		
	}
	if(have==0) 
	{
		printf("can't find updata file!!!\r\n");
		return -2;
	}
	else
	{
		printf("num = %d !!!\r\n",info.module_total[0]);
		a|=info.firmware_msg[num].addr[3];
		a=a<<8;
		a|=info.firmware_msg[num].addr[2];
		a=a<<8;
		a|=info.firmware_msg[num].addr[1];
		a=a<<8;
		a|=info.firmware_msg[num].addr[0];
		
		b|=info.firmware_msg[num].length[3];
		b=b<<8;
		b|=info.firmware_msg[num].length[2];
		b=b<<8;
		b|=info.firmware_msg[num].length[1];
		b=b<<8;
		b|=info.firmware_msg[num].length[0];
		
		printf("a=%x b=%x",a,b);
		return Extract_the_file_contents(dir,type, a, b);
		
	}
}


 



int32_t sdk_start_upgrade_form_package(uint8_t *dir,uint8_t flag)
{
	pid_t status;
	FILE *fp = NULL;
	int8_t buffer[128]={0};
	int8_t cmd[128]={0}; 
	
	system("mount -o remount,rw /");
	
	if(flag == UT_CORE)
	{
		system("echo \"UT_CORE_Upgrading\" > /opt/Upgrade_status");
		if(Unpack(dir,"_CORE.tar.gz")==0)
		{
			system("tar -zxvf _CORE.tar.gz -C /opt");
			system("echo \"UT_CORE_Upgrade_OK\" > /opt/Upgrade_status");
			return 0;
		}	
		system("echo \"UT_CORE_Upgrade_ERRO\" > /opt/Upgrade_status");
		return -1;
		
	}
	else if(flag == UT_APP)
	{
		system("echo \"UT_APP_Upgrading\" > /opt/Upgrade_status");
		if(Unpack(dir,"_APP.tar.gz")==0)
		{
			system("tar -zxvf _APP.tar.gz -C /app");
			system("echo \"UT_APP_Upgrade_OK\" > /opt/Upgrade_status");
			return 0;
		}
		system("echo \"UT_CORE_Upgrade_ERRO\" > /opt/Upgrade_status");
		return -1;
	}
	else
	{
		return -2; //参数错误
	}

	
}


/**
* @brief		升级状态获取
* @param		[in] type 升级设备类型
* @return		执行结果
* @retval		0 升级成功
* @retval       1 升级中
* @retval		<0 升级失败 
*/
int32_t sdk_upgrade_status_get(uint8_t flag)
{
	FILE *fp = NULL;
	char buffer[32]={0};
	
	if(flag == UT_CORE)
	{
		fp=popen("cat /opt/Upgrade_status","r");
		fgets(buffer, sizeof(buffer), fp);
		pclose(fp);
		if(memcmp(buffer, "UT_CORE_Upgrading", strlen("UT_CORE_Upgrading"))==0)
		{
			return 1;
		}
		else if(memcmp(buffer, "UT_CORE_Upgrade_OK", strlen("UT_CORE_Upgrade_OK"))==0)
		{
			return 0;
		}
		else if(memcmp(buffer, "UT_CORE_Upgrade_ERRO", strlen("UT_CORE_Upgrade_ERRO"))==0)
		{
			return -1;
		}
		else return -3;//获取失败
	}
	else if(flag == UT_APP)
	{
		fp=popen("cat /opt/Upgrade_status","r");
		fgets(buffer, sizeof(buffer), fp);
		pclose(fp);
		if(memcmp(buffer, "UT_APP_Upgrading", strlen("UT_APP_Upgrading"))==0)
		{
			return 1;
		}
		else if(memcmp(buffer, "UT_APP_Upgrade_OK", strlen("UT_APP_Upgrade_OK"))==0)
		{
			return 0;
		}
		else if(memcmp(buffer, "UT_APP_Upgrade_ERRO", strlen("UT_APP_Upgrade_ERRO"))==0)
		{
			return -1;
		}
		else return -3;//获取失败
	}
	else
	{
		return -2; //参数错误
	}

}


int32_t sdk_upgrade_progress_get(uint8_t flag)
{
	return 0;
}







int32_t sdk_start_upgrade(uint8_t *dir,uint8_t flag)
{
	pid_t status;
	FILE *fp = NULL;
	int8_t buffer[128]={0};
	int8_t cmd[128]={0}; 
	
	system("mount -o remount,rw /");
	
	
	if(flag == UT_CORE)
	{
		if(strstr(dir, "_CORE.tar.gz")!=NULL)
		{ 
			system("echo \"UT_CORE_Upgrading\" > /opt/Upgrade_status");
			sprintf(cmd, "tar -zxvf %s -C /opt",dir);
			system(cmd);
			system("echo \"UT_CORE_Upgrade_OK\" > /opt/Upgrade_status");
			return 0;
		}
		else 
		{
			system("echo \"UT_CORE_Upgrade_ERRO\" > /opt/Upgrade_status");
			return -1; //升级失败
		}
	}
	if(flag == UT_APP)
	{
		if(strstr(dir, "_APP.tar.gz")!=NULL)
		{ 
			system("echo \"UT_APP_Upgrading\" > /opt/Upgrade_status");
			sprintf(cmd, "tar -zxvf %s -C /app",dir);
			system(cmd);
			system("echo \"UT_APP_Upgrade_OK\" > /opt/Upgrade_status");
			return 0;
		}
		else 
		{
			system("echo \"UT_APP_Upgrade_ERRO\" > /opt/Upgrade_status");
			return -1; //升级失败
		}
	}
	else
	{
		return -2; //参数错误
	}

	
}




























/*
void main()
{

	//Unpack("leo5.sofar","opt.tar.gz");
	//system("tar xvf opt.tar.gz -C /opt");
	sdk_start_upgrade("leo5.sofar",0);
	printf("status %d\n",sdk_upgrade_status_get(0));
	
   
}*/

/*void BigEndiantoLittleEndian(package_signing_msg_t *p_info,uint32_t *p_a,uint32_t *p_b)
{
	uint32_t a=0,b=0;
	
	a|=p_info->firmware_msg[0].addr[3];
	a=a<<8;
	a|=p_info->firmware_msg[0].addr[2];
	a=a<<8;
	a|=p_info->firmware_msg[0].addr[1];
	a=a<<8;
	a|=p_info->firmware_msg[0].addr[0];
	
	b|=p_info->firmware_msg[0].length[3];
	b=b<<8;
	b|=p_info->firmware_msg[0].length[2];
	b=b<<8;
	b|=p_info->firmware_msg[0].length[1];
	b=b<<8;
	b|=p_info->firmware_msg[0].length[0];
	
	*p_a=a;
	*p_b=b;
}*/
/*
void BigEndiantoLittleEndian(package_signing_msg_t *p_info,uint32_t *p_a,uint32_t *p_b)
{
	uint32_t a=0,b=0;
	
	a|=p_info->firmware_msg[0].addr[0];
	a=a<<8;
	a|=p_info->firmware_msg[0].addr[1];
	a=a<<8;
	a|=p_info->firmware_msg[0].addr[2];
	a=a<<8;
	a|=p_info->firmware_msg[0].addr[3];
	
	b|=p_info->firmware_msg[0].length[0];
	b=b<<8;
	b|=p_info->firmware_msg[0].length[1];
	b=b<<8;
	b|=p_info->firmware_msg[0].length[2];
	b=b<<8;
	b|=p_info->firmware_msg[0].length[3];
	
	*p_a=a;
	*p_b=b;
}*/
