//arm-linux-gnueabihf-gcc test_record.c ../sdk_record.c -o test_record  -I../ -I../../../../include
//gcc test_record.c ../sdk_record.c -o test_record  -I../ -I../../../../include
//测试程序使用注意事项：单板上运行时，注意data文件夹的路径、执行的程序路径 要和sdk_record_init()里头给path_name[]赋值的内容匹配；

#include "core.h"
#include "sdk_record.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define TEST_ONE_RECORD_SIZE        64
#define TEST_RECORD_TYPE            3
#define TEST_ONE_TYPE_RECORD_NUM    6


extern  int32_t test_fs_get_size(uint32_t type);    //临时测试用，后面删除
extern void record_path_init(void); //临时测试用，后面删除

int main(int argc, char* argv[])
{
    int32_t cmd=0, type_num, i=0, j, k, ret;
    int8_t write_buf[TEST_ONE_RECORD_SIZE];
    int8_t read_buf[TEST_ONE_RECORD_SIZE];

    cmd = atoi(argv[1]);      

    for (i = 0; i < 3; i++)     //执行程序，输入4个入参，第1个为命令，其它的分别赋给write_buf[2]、write_buf[3]、write_buf[4]
    {
        write_buf[i + 2] = atoi(argv[2 + i]);
    }

    record_path_init();
    
    switch (cmd)
    {
        case 7:
        {
            
            sdk_record_path_set(0, "/opt/record_test/record_test1");
            sdk_record_init(0, TEST_ONE_RECORD_SIZE, TEST_ONE_TYPE_RECORD_NUM);
            printf("\n\n\n");
            sdk_record_init(1, TEST_ONE_RECORD_SIZE, TEST_ONE_TYPE_RECORD_NUM);
            break;
        }
        case 1: // 测试挂载
        {
            // 挂载溢出空间
            sdk_record_init(0, 512, 10005); 
            sdk_record_init(0, 1024, 3000); 

            // 参数区不存在
            sdk_record_init(RECORD_TYPE_NUM + 1, TEST_ONE_RECORD_SIZE, 3000);    

            // 按2类记录正常挂载进行测试
            for (i=0; i<TEST_RECORD_TYPE; i++)
            {
                ret = sdk_record_init(i, TEST_ONE_RECORD_SIZE, TEST_ONE_TYPE_RECORD_NUM); 
                printf("sdk_record_init--- type = %d, ret = %d\r\n", i, ret);   
            }      

            //测试写入单条记录超限的情况
            sdk_record_write(0, 0, write_buf, TEST_ONE_RECORD_SIZE+5);

            break;
        }
        case 2: // 测试记录读写
        {
            // 记录写
            for (i = 0; i < TEST_RECORD_TYPE; i++)
            {
                // 记录挂载
                sdk_record_init(i, TEST_ONE_RECORD_SIZE, 3000); 

                for (j = 5; j <TEST_ONE_RECORD_SIZE; j++)
                {
                    write_buf[j] = i*10+j;
                }
                // 每类记录，连续写入6条记录
                for (k = 0; k < (TEST_ONE_TYPE_RECORD_NUM); k++)
                {
                    write_buf[0] = i; //write_buf[0]---记录类型
                    write_buf[1] = k; //write_buf[1]---索引类型
                    sdk_record_write(i, 0, write_buf, TEST_ONE_RECORD_SIZE); 
                }

                ret = test_fs_get_size(i);
                printf("record file, type = %d, filesize = %d\r\n", i, ret);
            }

            // 记录读
            for (i = 0; i < TEST_RECORD_TYPE; i++)
            {
                printf("sdk_record_read----------------type = %03d\r\n", i);
                for (j = 0; j <TEST_ONE_TYPE_RECORD_NUM; j++)
                {
                    ret = sdk_record_read(i, j, read_buf, TEST_ONE_RECORD_SIZE);
                    if (ret >= 0)
                    {
                        printf("sdk_record read sucess---------type = %03d,index = %03d\r\n", i, j);
                        for (k = 0; k < TEST_ONE_RECORD_SIZE; k++)
                        {
                            printf("%03d ", read_buf[k]);
                        }
                        }
                    printf("\n************\r\n");
                }
                printf("\n\n");               
            }            
            break;
        }

        case 3: // 参数同步测试
        {
            sdk_record_init(0, TEST_ONE_RECORD_SIZE, 3000); 
            sdk_record_write(0, 0, write_buf, TEST_ONE_RECORD_SIZE); 
            sdk_record_sync(0);
            break;
        }

        case 4: // 测试记录删除
        {
            // 记录初始挂载、写
            for (i = 0; i < TEST_RECORD_TYPE; i++)
            {
                // 记录挂载
                sdk_record_init(i, TEST_ONE_RECORD_SIZE, 3000); 

                for (j = 5; j <TEST_ONE_RECORD_SIZE; j++)
                {
                    write_buf[j] = i*10+j;
                }
                // 每类记录，连续写入6条记录
                for (k = 0; k < (TEST_ONE_TYPE_RECORD_NUM); k++)
                {
                    write_buf[0] = i; //write_buf[0]---记录类型
                    write_buf[1] = k; //write_buf[1]---索引类型
                    sdk_record_write(i, 0, write_buf, TEST_ONE_RECORD_SIZE); 
                }

                ret = test_fs_get_size(i);
                printf("before delete, type = %d, filesize = %d\r\n", i, ret);
            }

            for (i = 0; i < TEST_RECORD_TYPE; i++)
            {
                sdk_record_delete(i);
                ret = test_fs_get_size(i);
                printf("after delete, type = %d, filesize = %d\r\n", i, ret);
            }
        }
        default:
            break;
    }

}
