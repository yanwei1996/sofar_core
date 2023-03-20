
//arm-linux-gnueabihf-gcc test_para.c ../sdk_para.c -o test_para -L../../../ -l_lal -I../../../../include/sdk -I../../../../include/lal -I../../../../include
//gcc
//测试程序使用注意事项：单板上运行时，注意para文件夹的路径、执行的程序路径 要和sdk_para.c里头sdk_para_init()定义的file_name内容匹配；

#include "core.h"
#include "sdk_para.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TEST_PARA_SIZE      48

int main(int argc, char* argv[])
{
    int32_t cmd = 1, type_num, i, j, ret;
    uint8_t write_buf[TEST_PARA_SIZE];
    uint8_t read_buf[TEST_PARA_SIZE];

    // cmd = atoi(argv[1]);       
    type_num = atoi(argv[1]); 

    while (cmd <= 3)// while (1)
    {
        switch (cmd)
        {
            case 1: // 测试挂载
            {
                // 挂载溢出空间
                sdk_para_init(0, PARA_SIZE_LIMIT + 1); 

                // 参数区不存在
                sdk_para_init(PARA_TYPE_NUM + 1, PARA_SIZE_LIMIT);                    

                // 按最大数量挂载
                for (i=0; i<PARA_TYPE_NUM; i++)
                {
                    sdk_para_init(i, PARA_SIZE_LIMIT);     
                }                
                break;
            }
            case 2: // 测试参数读写
            {
                //参数写
                for (i = 0; i < PARA_TYPE_NUM; i++)
                {
                    for (j = 0; j <TEST_PARA_SIZE; j++)
                    {
                        write_buf[j] = i+j;
                    }
                    ret = sdk_para_write(i, 0, write_buf, TEST_PARA_SIZE);
                }

                //参数读
                for (i = 0; i < PARA_TYPE_NUM; i++)
                {
                    ret = sdk_para_read(i, 0, read_buf, TEST_PARA_SIZE);
                    if (ret == TEST_PARA_SIZE)
                    {
                        printf("sdk_para_read sucess---- type = %03d\r\n", i);
                        for (j = 0; j < TEST_PARA_SIZE; j++)
                        {
                            printf("%03d ", read_buf[j]);
                        }
                        printf("\n");
                    }
                    else
                    {
                        printf("sdk_para_read error, i=%d, ret=%d\r\n", i, ret);
                    }
                }            
                break;
            }

            case 3: // 参数同步测试
            {
                sdk_para_sync(1);
                break;
            }

            default:
                break;
        }
        cmd++;
    }
}
