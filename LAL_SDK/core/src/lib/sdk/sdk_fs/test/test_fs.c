//arm-linux-gnueabihf-gcc test_fs.c ../sdk_fs.c -o test_fs  -I../../../../include/sdk -I../../../../include/lal -I../../../../include
//gcc test_fs.c ../sdk_fs.c -o test_fs  -I../../../../include/sdk -I../../../../include/lal -I../../../../include
//测试程序使用注意事项：TEST_FILE_PATH 路径要和sdk_fs.c里头OPEN_PATH_APP_OPERABLE匹配

#include "core.h"
#include "sdk_fs.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/stat.h>
#include <sys/types.h>

#define log_i(...)		printf(__VA_ARGS__)	    //fixme 后面easylog可以用后，屏蔽掉  

#define TEST_FILE_PATH  "/opt/app/app_file_test.ini"
#define TEST_FILE_LEN   32

int main(int argc, char* argv[])
{
    int32_t cmd=0, mode;
    int32_t ret, i;
    int32_t file_size;
    int8_t write_buf[TEST_FILE_LEN] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    int8_t read_buf[1024*8];
    fs_t *fs_test;
    fs_t file_1;

    cmd = atoi(argv[1]);   
    mode = atoi(argv[3]);   

    switch (cmd)
    {
        case 1: // 测试打开、关闭文件
        {
            //模拟模式参数错误
            fs_test = sdk_fs_open(TEST_FILE_PATH, 0);
            if (fs_test == NULL)
            {
                log_i("sdk_fs_open----fail, mode err!\r\n");
            }

            //模拟路径错误
            fs_test = sdk_fs_open("./app/app.ini", 0);
            if (fs_test == NULL)
            {
                log_i("sdk_fs_open----fail, path err!\r\n");
            }

            //正常测试打开、关闭文件
            fs_test = sdk_fs_open(TEST_FILE_PATH, (FS_OPEN_APPEND|FS_READ));
            if (fs_test != NULL)
            {
                log_i("sdk_fs_open----sucess!\r\n");
                ret = sdk_fs_close(fs_test);
                if (!ret)
                {
                    log_i("sdk_fs_close----sucess!\r\n");
                }
            }
            break;
        }

        case 2: //测试文件写、读(正常模式)    ———— 虚拟机下模拟测试，即使没有同步，数据也是立即写入文件的
        case 3: //测试文件写、读(追加模式，只写)
        case 4: //测试文件写、读(追加模式，可读写)
        {
            for (i = 0; i < 3; i++)     //执行程序，输入4个入参，后3个分别赋给write_buf[0]、write_buf[1]、write_buf[2]
            {
                write_buf[i] = atoi(argv[2 + i]);
            }

            if (cmd == 2)
            {
                fs_test = sdk_fs_open(TEST_FILE_PATH, (FS_OPEN_ALWAYS|FS_READ|FS_WRITE));   //正常模式
            }
            else if (cmd == 3)
            {
                fs_test = sdk_fs_open(TEST_FILE_PATH, (FS_OPEN_APPEND));                    //追加模式，只写         
            }
            else if (cmd == 4)
            {
                fs_test = sdk_fs_open(TEST_FILE_PATH, (FS_OPEN_APPEND|FS_READ|FS_WRITE));   //追加模式，可读写   
            }

            if (fs_test)
            {
                // sdk_fs_lseek(fs_test, 0); ————追加模式，无论是否使用该语句，写数据依然是写到文件末尾
                ret = sdk_fs_write(fs_test, write_buf, TEST_FILE_LEN);
                file_size = sdk_fs_get_size(fs_test);
                if (ret == TEST_FILE_LEN)
                {
                    log_i("sdk_fs_write----sucess!, write num=%d, ok num=%d, file_size=%d\r\n", TEST_FILE_LEN, ret, file_size);
                    sdk_fs_lseek(fs_test, 0);
                    ret = sdk_fs_read(fs_test, read_buf, file_size);
                    if (ret == file_size)
                    {
                        log_i("sdk_fs_read----sucess!, read num=ok num=%d, file_size=%d\r\n",ret, file_size);
                        for (i=0; i<file_size; i++)
                        {
                            log_i("%d, ", read_buf[i]);
                        }
                        log_i("\r\n\n");
                    }
                    else
                    {
                        log_i("sdk_fs_read----fail!, ret=%d, file_size=%d\r\n",ret, file_size);
                    }
                }                
            }
            break;
        }

        case 5: //测试文件同步
        {
            fs_test = sdk_fs_open(TEST_FILE_PATH, (FS_OPEN_APPEND|FS_READ|FS_WRITE));   //追加模式，可读写
            if (fs_test)
            {
                ret = sdk_fs_write(fs_test, write_buf, TEST_FILE_LEN);
                ret = sdk_fs_file_sync(fs_test);
                log_i("sdk_fs_file_sync test, ret = %d\r\n", ret);
            }
            break;
        }

        case 6: //测试文件重命名
        {
            ret = sdk_fs_rename("/opt/app/rename_old.ini", "/opt/app/rename_new.ini");
            log_i("sdk_fs_rename test, ret = %d\r\n", ret);
            break;
        }

        case 7: //测试创建目录（
                //例1,测试目录不合法，修改FS_USR_PATH_NAME为“./”后，虚拟机下执行：./test_fs 7 ./abc/1/2 777
        {
            mode = S_IRUSR|S_IWUSR|S_IXGRP;
            ret = sdk_fs_mkdir(argv[2], mode);
            log_i("sdk_fs_mkdir test, dirname=%s, mode = %04o, ret = %d\r\n", argv[2], mode, ret);
            break;
        }

        case 8: //测试文件删除
        {
            ret = sdk_fs_remove(argv[2]);
            log_i("sdk_fs_remove test, filename=%s, ret = %d\r\n", argv[2], ret);        
            break;
        }

        case 9: //测试异常文件关闭
        {
            fs_test = &file_1;
            // fs_test = fopen("./123", "r");   文件正常关闭
            ret = sdk_fs_close(fs_test);
            log_i("sdk_fs_close test, err test, ret = %d\r\n", ret);        
            break;
        }

        case 10:    // 测试未open文件，直接操作文件读写操作，是否会异常
        {
            fs_test = &file_1;
            ret = sdk_fs_lseek(fs_test, 10);
            ret = sdk_fs_read(fs_test, write_buf, TEST_FILE_LEN);
            ret = sdk_fs_write(fs_test, write_buf, TEST_FILE_LEN);
            sdk_fs_get_size(fs_test);
            log_i("sdk_fs_write test, err test, ret = %d\r\n", ret);        
            break;          
        }

        default:
            break;
    }

}
