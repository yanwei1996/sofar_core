/************************************************************************************
 *Project: Inverter-320kw-ARM
 *Description: update_server
 *Created on: 2022-08-08
 *Author: quliuliu
 ************************************************************************************/
#include <dirent.h>
#include "globa.h"
#include "update_file_check.h"

// 每行最大长度
#define LINE_MAX 1024
// 固件包签名信息长度
#define SIGNING_MSG_LENGTH 1024
// 读取固件每帧数据长度
#define DATA_LENGTH 1024

uint32_t crc32 = 0xFFFFFFFF;

static const uint32_t crc32_table[] = {
    0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f,
    0xe963a535, 0x9e6495a3, 0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
    0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
    0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
    0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9,
    0xfa0f3d63, 0x8d080df5, 0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
    0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b, 0x35b5a8fa, 0x42b2986c,
    0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
    0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423,
    0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
    0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d, 0x76dc4190, 0x01db7106,
    0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
    0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d,
    0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
    0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
    0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
    0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7,
    0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
    0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa,
    0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
    0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
    0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
    0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84,
    0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
    0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
    0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
    0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e,
    0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
    0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55,
    0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
    0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28,
    0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
    0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
    0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
    0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
    0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
    0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69,
    0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
    0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc,
    0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
    0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693,
    0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
    0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};

void sdk_check_crc32(uint8_t *buf, int len) 
{
    uint8_t *p=NULL;
    
    for ( p = buf; len > 0; ++p, --len ) 
    {
        crc32 = (uint32_t)(crc32_table[(crc32 ^ *p ) & 0xFF] ^ (crc32 >> 8));
    }
}

//获取U盘设备名称
int mount_U_disk(char *dev_name)
{
	int i=0;
	char buf[LINE_MAX]={0};

	FILE *fp = NULL;
	if((fp=popen("fdisk -l", "r")) == NULL)
	{
		ERROR("%s\n", strerror(errno));
		return -1;
	}

	while(fgets(buf, LINE_MAX, fp));

	for(i=0; i<strlen(buf); i++)
	{
		if(buf[i]==' ')
		{
			break;
		}
		*(dev_name+i)=buf[i];	
	}
	
	pclose(fp);
	return 0;
}

//从U盘中获取固件
int32_t get_firmware_from_U_disk()
{
	int8_t cmd[128]={0};

	/**********U盘自动挂载失灵时放开下面代码********************************
	char dev_name[32]={0};
	sdk_system_cmd((int8_t *)"umount /mnt");
	mount_U_disk(dev_name);
	memset(cmd, 0, sizeof(cmd));
	snprintf(cmd, sizeof(cmd), "mount %s /mnt", dev_name);
	sdk_system_cmd(cmd);
	memset(cmd, 0, sizeof(cmd));
	snprintf(cmd, sizeof(cmd), "cp /mnt/firmware/%s /opt/update", ptr->d_name);
	sdk_system_cmd(cmd);
	****************************************************************/

	//获取目录中固件包文件名称
	DIR *dir;
    struct dirent *ptr;
    dir = opendir("/media/sda1/firmware/");//firmware
    while((ptr = readdir(dir))!=NULL)
    {
        if(strstr(ptr->d_name,".sofar"))
		{
			DEBUG("d_name:%s\n",ptr->d_name);
			
		}
    }
    closedir(dir);

	strncpy((char *)shm->update.package_name, ptr->d_name, sizeof(shm->update.package_name)-1);

	//自动挂载
	snprintf((char *)cmd, sizeof(cmd)-1, "cp /media/sda1/firmware/%s /opt/update", ptr->d_name);
	sdk_system_cmd(cmd);

	return 0;
}

#pragma pack(push,1)	//作用：是指把原来对齐方式设置压栈，并设新的对齐方式设置为一个字节对齐
typedef struct firmware_msg	// 各固件信息
{
	int8_t object[8];
	int8_t name[32];
	uint8_t addr[4];
	uint8_t length[4];
	int8_t version[20];
}firmware_msg_t;
typedef struct signing_msg	// 固件包签名信息
{
	int8_t protocol_version;
	int8_t company[16];
	int8_t type[16];
	int8_t package_name[32];
	uint8_t release_time[4];
	uint8_t module_total;
	int8_t module_config[2];
	int8_t reserved1[64];
	firmware_msg_t firmware_msg[12];
	int8_t reserved2[64];
	uint8_t package_length[4];
	uint32_t package_crc32;
}package_signing_msg_t;
#pragma pack(pop)	//作用：恢复对齐状态

static package_signing_msg_t package={0};

// 对固件包进行CRC32校验
uint32_t firmware_check(const int8_t *path, uint64_t length)
{
	crc32 = 0xFFFFFFFF;
	uint32_t i=0;
	uint32_t check_value;
	uint8_t data[1050]={0};

	FILE *fp=NULL;
	if((fp=fopen((char *)path,"r")) == NULL)
	{
		ERROR("%s\n", strerror(errno));
		return 1;
	}

	for(i=0; i<(length>>10)+1; i++)	// length>>10 <==> length/1024; 注意：打印uint64_t的数值时要用%lld
	{
		memset(data, 0, sizeof(data));
		
		if(i==(length>>10))
		{
			if(fread(data, length&0x3FF, 1, fp) != 1)	// length&0x3FF <==> length%1024; 注意：打印uint64_t的数值时要用%lld
			{
				ERROR("%s\n", strerror(errno));
				fclose(fp);
				return 1;
			}
			sdk_check_crc32(data, length&0x3FF);//减去签名信息中CRC32字段
		}
		else
		{
			if(fread(data, SIGNING_MSG_LENGTH, 1, fp) != 1)
			{
				ERROR("%s\n", strerror(errno));
				fclose(fp);
				return 1;
			}
			sdk_check_crc32(data, SIGNING_MSG_LENGTH);
		}
	}
	
	fclose(fp);

	return check_value = ~crc32;
}

// 对固件包进行检验
int32_t firmware_package_parse(int8_t *firmware_package_name)
{
	int8_t path[64]={0};
	int8_t data[SIGNING_MSG_LENGTH]={0};
	uint64_t package_length=0;
	
	snprintf((char *)path, sizeof(path)-1, "/opt/update/%s", firmware_package_name);

	uint64_t file_length=sdk_file_get_size(path);

	//提取固件包签名信息
	FILE *fp=NULL;
	if((fp=fopen((char *)path,"r")) == NULL)
	{
		ERROR("%s\n", strerror(errno));
		return -1;
	}
	if(fseek(fp, file_length-1024, SEEK_SET) != 0)
	{
		ERROR("%s\n", strerror(errno));
		fclose(fp);
		return -1;
	}
	if(fread(data, SIGNING_MSG_LENGTH, 1, fp) != 1)
	{
		ERROR("%s\n", strerror(errno));
		fclose(fp);
		return -1;
	}
	fclose(fp);

	memcpy(&package, data, SIGNING_MSG_LENGTH);

	//校验固件包的长度
	package_length = (package.package_length[0]<<24)+(package.package_length[1]<<16)+(package.package_length[2]<<8)+package.package_length[3];
	if(package_length != file_length-SIGNING_MSG_LENGTH)
	{
		ERROR("Failed to verify the firmware package length. Procedure: package_length=%ud\n", package_length);
		return -1;
	}
	
	//校验固件包的CRC32
	if(package.package_crc32 != firmware_check(path, file_length-4))
	{
		ERROR("Firmware package length CRC32 failed to be verified\n");
		return -1;
	}
	//校验其他信息
	// ...code...

	return 0;
}

// 提取各模块固件
int32_t module_firmware_extraction(int8_t *firmware_package_name)
{
	int32_t i=0, j=0;
	int32_t addr=0;
	int32_t frame_num=0;
	int8_t path[64]={0};
	int8_t data[DATA_LENGTH]={0};
	FILE *fp_read=NULL;
	FILE *fp_write=NULL;
	
	snprintf((char *)path, sizeof(path)-1, "/opt/update/%s", firmware_package_name);

	if((fp_read=fopen((char *)path,"r")) == NULL)
	{
		ERROR("%s\n", strerror(errno));
		return -1;
	}

	// 分解为各个固件
	for(i=0; i<package.module_total; i++)
	{
		strncpy((char *)shm->update.module_object[i], (char *)package.firmware_msg[i].object, sizeof(shm->update.module_object[i])-1);
	
		memset(path, 0, sizeof(path));
		snprintf((char *)path, sizeof(path)-1, "/opt/update/%s", package.firmware_msg[i].name);
		if((fp_write=fopen((char *)path,"w+")) == NULL)
		{
			ERROR("%s\n", strerror(errno));
			fclose(fp_read);
			return -1;
		}
		addr = (package.firmware_msg[i].addr[0]<<24)+(package.firmware_msg[i].addr[1]<<16)+(package.firmware_msg[i].addr[2]<<8)+package.firmware_msg[i].addr[3];
		if(fseek(fp_read, addr, SEEK_SET) != 0)
		{
			ERROR("%s\n", strerror(errno));
			goto error;
		}
		frame_num = (package.firmware_msg[i].length[0]<<24)+(package.firmware_msg[i].length[1]<<16)+(package.firmware_msg[i].length[2]<<8)+package.firmware_msg[i].length[3];
		for(j=0; j<(frame_num>>10); j++)
		{
			if(fread((char *)data, DATA_LENGTH, 1, fp_read) != 1)
			{
				ERROR("%s\n", strerror(errno));
				goto error;
			}
			if(fwrite((char *)data, DATA_LENGTH, 1, fp_write) != 1)
			{
				ERROR("%s\n", strerror(errno));
				goto error;
			}
		}
		fclose(fp_write);
		strncpy((char *)shm->update.module_name[i], (char *)package.firmware_msg[i].name, sizeof(shm->update.module_name[i])-1);
	}

	fclose(fp_read);
	return 0;

error:
	fclose(fp_read);
	fclose(fp_write);
	return -1;
}

// 升级任务
void *update_file_check(void *arg)
{
	int8_t i, n;
	int8_t clean=0;
	int8_t restart=0;
	
	while(1)
	{
		if(shm->update.state != (inform_e=NONE))
		{
			//升级开始时把进度条全部置为0
			if(clean==0)
			{
				clean=1;
			}
			if(clean == 1)
			{
				for(i=0;i<UPDATE_OBJECT_NUM;i++)
				{
					shm->update.module_percent[0]=0;
				}
				clean=2;
			}

			//升级开始时蜂鸣器和指示灯对应响应
			if(shm->interactive_hint != (hint_level_e=FAULT))
				shm->interactive_hint=(hint_level_e=HINT);

			//判断是否为触发U盘升级
			if(shm->update.state == (inform_e=UDISK))
			{
				//从U盘拷贝升级包到/opt/update目录
				get_firmware_from_U_disk();
				shm->update.state = (inform_e=UPDATE);
			}
			
			if(shm->update.state == (inform_e=DOWNLOAD))
			{
				sleep(1);
			}
			else if(shm->update.state == (inform_e=UPDATE))
			{
				if(firmware_package_parse(shm->update.package_name) !=0 )
				{
					ERROR("Failed to check the firmware package. Procedure\n");
					shm->update.state = (inform_e=FAIL);
					continue;
				}
				if(module_firmware_extraction(shm->update.package_name) != 0)
				{
					ERROR("Firmware extraction failure\n");
					shm->update.state = (inform_e=FAIL);
					continue;
				}
				
				shm->update.state = (inform_e=UPDATING);
			}
			else if(shm->update.state == (inform_e=UPDATING))
			{
				//检查各个固件升级状态，如果如果有ARM升级任务则要等其他升级任务完成后重启
				for(n=0,i=0; i<UPDATE_OBJECT_NUM; i++)
				{
					//检查是否有ARM升级
					if((strncmp((char *)shm->update.module_object[i], "ARM", 3)==0) && (shm->update.module_percent[i]==0))
					{
						DEBUG("update_server:ARM update\n");
						sdk_system_cmd((int8_t *)"mount -o remount,rw /");
						sdk_system_cmd((int8_t *)"tar -zxvf /opt/update/opt.tar.gz -C /opt");
						memset(shm->update.module_object[i], 0, sizeof(shm->update.module_object[i]));	//升级完成后shm->update.module_object[0]置空
						shm->update.module_percent[i]=100;
						restart=1;
					}

					//检查各个模块是否升级完成
					if(strlen((char *)shm->update.module_object[i])==0)
					{
						if((n++) == (UPDATE_OBJECT_NUM-1))
						{
							shm->update.state = (inform_e=NONE);

							//升级完成后删除/opt/update/中所有文件
							sdk_system_cmd((int8_t *)"rm -rf /opt/update/*");

							sdk_system_cmd((int8_t *)"mount -o remount,ro /");

							if(restart==1)
							{
								sleep(10);//防止ARM没解压完而重启
								system("sync");
								system("reboot -f"); //以后WEB加个手动重启按钮
							}
						}
					}
				}
			}
			else
			{

			}
		}
		else
		{
			clean=0;
			
			if(shm->interactive_hint != (hint_level_e=FAULT))
				shm->interactive_hint=0;
		}

		sleep(1);
	}
}


