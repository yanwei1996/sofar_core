/**
* @file		sdk_record.c
* @brief    记录存储模块
* @copyright Shouhang
* @author   yangwenjuan
* @note     2023.02.08 新拟制
* @version  V1.0   
* @date     2023/02/08
*/

#include "errors.h"
#include "sdk_record.h"
#include <stdio.h>
#include<unistd.h>
#include <string.h>

#define SDK_RECORD_DLOG(...) printf(__VA_ARGS__)//log_d(__VA_ARGS__)
#define log_i(...) printf(__VA_ARGS__)

// #define RECORD_FILE_PATH_LEN		    64//30	
#define RECORD_FILE_PATH_LEN_LIMIT		128	

typedef struct
{
    int32_t one_record_size; // 单条记录大小(字节)
    int32_t max_num;         // 最大记录深度(条)
    int32_t index;           // 记录索引(0~N递增)
} sdk_record_t;

// 记录属性列表
static sdk_record_t g_record_attr[RECORD_TYPE_NUM] = {0};

// 文件句柄列表
static FILE *gp_fs[RECORD_TYPE_NUM][2] = {0};	

// 记录文件名列表指针	———— 路径待评审，待调整
const static char *g_record_file[RECORD_TYPE_NUM][2] = {
    {"/user/record/record_001_para", "/user/record/record_001"},
	{"/user/record/record_002_para", "/user/record/record_002"},
    {"/user/record/record_003_para", "/user/record/record_003"},
	{"/user/record/record_004_para", "/user/record/record_004"},
};

// 记录文件名列表，在record_path_init函数里会进行默认路径名设置；
// 备注：每一类记录对应两个文件，比如，若"/user/record/record_001"用于存记录数据，那"/user/record/record_001_para"用于存对应的记录参数
static char g_record_path[RECORD_TYPE_NUM][RECORD_FILE_PATH_LEN_LIMIT+8] = {
    "/user/record/record_001",
    "/user/record/record_002",
    "/user/record/record_003",
    "/user/record/record_004",
    "/user/record/record_005",
};

/**
 * @brief  记录的默认路径初始化函数————非sdk接口函数，在core进程初始化时调用
 * @return void
 */
void record_path_init(void)
{
    int32_t i;

    for (i = 0; i < RECORD_TYPE_NUM; i++)
    {
        snprintf(g_record_path[i],  RECORD_FILE_PATH_LEN_LIMIT, "/user/record/record_%03d", i);   //fixme 后面要把/opt 删除
        //snprintf(g_record_path[i],  RECORD_FILE_PATH_LEN_LIMIT, "/opt/user/record_%03d", i);   //临时测试用，因开发时使用的文件系统版本，/user不支持读写
    }
}


/**
 * @brief  记录路径设置
 * @param  [in] type 记录类型（枚举中定义的值，<RECORD_TYPE_NUM）
 * @param  [in] p_path 要设置的记录存储的路径（长度必须小于123字符）
 * @retval 0  成功
 * @retval -1 传参错误
 * @retval 
 * @note   该接口仅在应用对存储路径名有特殊要求时需使用；若无要求，则不需调用，记录会存储在默认路径
 * @note   使用时，应用必须在调用sdk_record_init之前先调用sdk_record_path_set接口，路径设置才能生效。
 */
int32_t sdk_record_path_set(uint32_t type, const int8_t *p_path)
{
    int32_t len;
    int32_t ret = ERR_NONE;

    len = strlen(p_path);

    if ((type >= RECORD_TYPE_NUM) || (p_path == NULL) || (len > RECORD_FILE_PATH_LEN_LIMIT))
    {
        ret = ERR_PARA; // 传参错误
    }
    else
    {
        memset(g_record_path[type], 0, RECORD_FILE_PATH_LEN_LIMIT); // 先清空默认的路径名
        memcpy(g_record_path[type], p_path, len);   // 设置路径名       
    }

    return ret;
}

/**
 * @brief  按记录类型挂载记录区
 *         注意，每次上电都要挂载，挂载传的配置参数如果不一样，会清空之前的记录
 * @param  [in] type 记录类型（枚举中定义的值，<RECORD_TYPE_NUM）
 * @param  [in] one_record_size 单条记录的大小(1~512字节)
 * @param  [in] max_num 最大记录深度(1~10000条)
 * @retval 0  成功
 * @retval -1 传参错误
 * @retval 其他负数，为文件系统出错
 * @note   ①每个type的记录，只调用一次；②挂载成功后，才能进行记录读写等操作；
 */
int32_t sdk_record_init(uint32_t type, uint32_t one_record_size, uint32_t max_num)
{
    int32_t ret = 0, i;
    sdk_record_t *record_attr = &g_record_attr[type];
    uint8_t path_name[2][RECORD_FILE_PATH_LEN_LIMIT];

    if ((type >= RECORD_TYPE_NUM)\
        || ((one_record_size == 0) || (one_record_size > RECORD_ONE_SIZE))\
        || ((max_num == 0) || (max_num > RECORD_DEEP)))
    {
        ret = -1; // 传参错误
        goto __exit;
    }
    if (gp_fs[type][0])
    { 
        SDK_RECORD_DLOG("sdk_record_init() is multiple\r\n");
        return 0;   // 已经初始化，直接返回
    }

    /** 打开当前记录区的参数文件、并操作 */  
#if 1 
    strcpy(path_name[0], g_record_path[type]);
    strcat(path_name[0], "_para");
    strcpy(path_name[1], g_record_path[type]);

    //snprintf(path_name[0],  RECORD_FILE_PATH_LEN_LIMIT, "/opt/data/record/record_%03d_para", type); // 记录的属性文件路径
    //snprintf(path_name[1],  RECORD_FILE_PATH_LEN_LIMIT, "/opt/data/record/record_%03d_data", type); // 记录的内容文件路径 
#else   //测试用
    snprintf(path_name[0],  RECORD_FILE_PATH_LEN_LIMIT, "./data/record_%03d_para", type); // 记录的属性文件路径
    snprintf(path_name[1],  RECORD_FILE_PATH_LEN_LIMIT, "./data/record_%03d_data", type); // 记录的内容文件路径 
    printf("type = %d, path_name[0] = %s, path_name[1] = %s\r\n", type, path_name[0], path_name[1]);  
#endif  
    g_record_file[type][0] = path_name[0];
    g_record_file[type][1] = path_name[1];
    log_i("g_record_file[%d][0] = %s, g_record_file[%d][1] = %s\r\n", type, g_record_file[type][0], type, g_record_file[type][1]);
    if(access(g_record_file[type][0], F_OK) != 0) // 文件不存在
    {
        //fixme  后续需补充自动递归创建
        gp_fs[type][0] = fopen(g_record_file[type][0], "wt+"); // 文件不存在, 创建文件、读写方式打开
    }
    else
    {
        gp_fs[type][0] = fopen(g_record_file[type][0], "rt+"); // 文件存在, 读写方式打开
    }
    if (gp_fs[type][0] == NULL)
    {
        ret = -2;
        goto __exit;
    }
	ret = fseek(gp_fs[type][0], 0, SEEK_SET); 
    if (ret != 0) 
    {
        ret = -3;
        goto __exit;
    }
	ret = fread(record_attr, 1, sizeof(sdk_record_t), gp_fs[type][0]);            
    if ((record_attr->one_record_size != one_record_size) || (record_attr->max_num != max_num)) // 属性不匹配，重新更新参数
    { 
        //SDK_RECORD_DLOG("[warn] sdk_record_init() size: %d deep: %d!! \r\n", one_record_size, max_num);
        record_attr->index = 0;
        record_attr->one_record_size = one_record_size;
        record_attr->max_num = max_num;
		ret = fseek(gp_fs[type][0], 0, SEEK_SET); 
        if (ret != 0) //(ret < 0)
        {
            ret = -5;
            goto __exit;
        }
		ret = fwrite(record_attr, 1, sizeof(sdk_record_t), gp_fs[type][0]); 
        if (ret != sizeof(sdk_record_t)) //(ret < 0) // 写入记录属性
        {
            ret = -6;
            goto __exit;
        }
        else
        {
            ret = 0;
        }

        sync();
        if(access(g_record_file[type][1], F_OK) == 0)   
        {
            remove(g_record_file[type][1]);
        }
    }
    SDK_RECORD_DLOG("sdk_record_init() size: %d deep: %d index: %d\r\n", 
                    record_attr->one_record_size,                        
                    record_attr->max_num,                                
                    record_attr->index);   

    if(access(g_record_file[type][1], F_OK) != 0)   
    {
        gp_fs[type][1] = fopen(g_record_file[type][1], "wb+");  //文件不存在，则创建文件
    }
    else 
    {
        gp_fs[type][1] = fopen(g_record_file[type][1], "rb+"); //sdk_fs_open(g_record_file[type][1], FS_OPEN_ALWAYS | FS_WRITE | FS_READ);
    }
    if (gp_fs[type][1] == NULL)
    {
        ret = -7;
        goto __exit;
    }

__exit:
    if (ret < 0)
    {
        for (i = 0; i < 2; i++)
        {
            if (gp_fs[type][i])
            {
                fclose(gp_fs[type][i]); 
                gp_fs[type][i] = 0;
            }
        }
        SDK_RECORD_DLOG("sdk_record_init fail %d !! \r\n", ret);
    }
    return ret;
}

/**
 * @brief  获取可记录最大数目
 * @param  [in] type 记录类型（枚举中定义的值，<RECORD_TYPE_NUM）
 * @retval >0 最大深度
 * @retval -1 传参错误
 * @retval -2 未挂载
 * @pre    调用sdk_record_init()之后才生效。
 */
int32_t sdk_record_get_max_num(uint32_t type)
{
    int32_t ret = 0;

    if (type >= RECORD_TYPE_NUM)
    {
        ret = -1;
    }
    else if (gp_fs[type][0] == 0)
    {
        ret = -2;
    }
    else
    {
        ret = g_record_attr[type].max_num;
    }

    return ret;
}

/**
 * @brief  获取当前存储了多少条记录
 * @param  [in] type 记录类型（枚举中定义的值，<RECORD_TYPE_NUM）
 * @retval 0~N 存储了多少条
 * @retval -1 传参错误
 * @retval -2 未挂载
 * @pre    调用sdk_record_init()之后才生效。
 */
int32_t sdk_record_get_index(uint32_t type)
{
    int32_t ret = 0;

    if (type >= RECORD_TYPE_NUM) // 记录类型不存在
    { 
        ret = -1;
    }
    else if (gp_fs[type][0] == 0) // 未初始化挂载
    { 
        ret = -2;
    }
    else
    {
        if (g_record_attr[type].index > g_record_attr[type].max_num)
        {
            ret = g_record_attr[type].max_num;
        }
        else
        {
            ret = g_record_attr[type].index;
        }
    }
    return ret;
}

/**
 * @brief  写记录内容
 * @param  [in] type  记录类型（枚举中定义的值，<RECORD_TYPE_NUM）
 * @param  [in] index 内容索引: 取值范围[0], 0为新增一条记录
 * @param  [in] buf   内容
 * @param  [in] len   内容长度
 * @retval -1 录类型不存在
 * @retval -2 未挂载
 * @retval 其他负数: 文件系统出错
 * @pre    调用sdk_record_init()之后才生效。
 */
int32_t sdk_record_write(uint32_t type, int32_t index, uint8_t *buf, uint32_t len)
{
    int32_t ret = 0;           //
    uint32_t __index;          //
    uint32_t lseek;            // 文件偏移位置
    sdk_record_t *record_attr; // 记录属性指针

    if (type >= RECORD_TYPE_NUM)
    {
        ret = -1; // 传参错误
        goto __exit;
    }

    record_attr = &g_record_attr[type];

    if ((index != 0)  //
        || (buf == NULL) //
        || (len != record_attr->one_record_size))
    {
        ret = -1; // 传参错误
        goto __exit;
    }

    if (gp_fs[type][0] == 0)
    {
        ret = -2; // 文件打开失败
        goto __exit;
    }
    __index = record_attr->index % record_attr->max_num;
    lseek = __index * record_attr->one_record_size;
    ret = fseek(gp_fs[type][1], lseek, SEEK_SET);
    if (ret != 0)
    {
        ret = -5;
        goto __exit;
    }
    ret = fwrite(buf, 1, len, gp_fs[type][1]);
    if (ret != len)
    {
        ret = -6;
        goto __exit;
    }
    record_attr->index++;
  
    /* 写入记录属性 */
	ret = fseek(gp_fs[type][0], 0, SEEK_SET);
    if (ret != 0)
    {
        ret = -9;
        goto __exit;
    }
	ret = fwrite(record_attr, 1, sizeof(sdk_record_t), gp_fs[type][0]);
    if (ret != sizeof(sdk_record_t))
    {
        ret = -10;
        goto __exit;
    }
    else
    {
        ret = 0;
    }
	// sync();

__exit:
    if (ret < 0)
    {
        SDK_RECORD_DLOG("sdk_record_write() fail: %d !! \r\n", ret);
    }
    return ret;
}

/**
 * @brief  读记录内容
 * @param  [in] type  记录类型（枚举中定义的值，<RECORD_TYPE_NUM）
 * @param  [in] index 内容索引: 取值范围[0~max_num], 0为最新的记录，1为次新的记录...直到最大记录数为止
 * @param  [in] buf   内容
 * @param  [in] len   内容长度
 * @retval >0 读取到的字节长度
 * @retval -1 录类型不存在
 * @retval -2 未挂载
 * @retval 其他负数: 文件系统出错
 * @pre    调用sdk_record_init()之后才生效。
 */
int32_t sdk_record_read(uint32_t type, int32_t index, uint8_t *buf, uint32_t len)
{
    int32_t ret = 0;
    int32_t __index;
    uint32_t lseek; // 文件偏移位置
    uint32_t record_id;
    sdk_record_t *record_attr;

    if (type >= RECORD_TYPE_NUM)
    {
        ret = -1; // 传参错误
        goto __exit;
    }

    record_attr = &g_record_attr[type];
    if ((index >= record_attr->max_num) || (index >= record_attr->index) || (index < 0))
    {
        ret = -1; // 传参错误
        goto __exit;
    }

    if ((buf == 0) || (len != record_attr->one_record_size))
    {
        ret = -1; // 传参错误
        goto __exit;
    }

    if (gp_fs[type][0] == 0)
    { 
        ret = -2; // 文件打开失败
        goto __exit;
    }

    __index = record_attr->index - index - 1;
    record_id = (__index) % record_attr->max_num;
    lseek = record_id * record_attr->one_record_size;
    ret = fseek(gp_fs[type][1], lseek, SEEK_SET);;
    if (ret != 0)
    {
        ret = -3;
        goto __exit;
    }
    ret = fread(buf, 1, len, gp_fs[type][1]);
    if (ret != len)
    {
        ret = -4;
        goto __exit;
    }

__exit:
    if (ret < 0)
    {
        SDK_RECORD_DLOG("sdk_record_read() fail: %d !! \r\n", ret);
    }
    return ret;
}

/**
 * @brief     同步记录(保存数据，否则有可能丢失不确定的数据)
 * @param[in] type 记录类型（枚举中定义的值，<RECORD_TYPE_NUM）
 * @retval 0  成功
 * @retval -1 传参错误
 * @retval -2 未挂载
 * @retval 其他负数: 文件系统出错
 */
int32_t sdk_record_sync(uint32_t type)
{
    int32_t ret = 0;

    if (type >= RECORD_TYPE_NUM)
    {
        ret = -1; // 记录类型不存在，返回错误
        goto __exit;
    }

    if (gp_fs[type][0] == 0)
    { 
        ret = -2;   // 文件打开失败
        goto __exit;
    }

	sync();

__exit:
    if (ret < 0)
    {
        SDK_RECORD_DLOG("sdk_record_sync() fail: %d !! \r\n", ret);
    }
    return ret;
}

/**
 * @brief     删除所有记录
 * @param[in] type 记录类型（枚举中定义的值，<RECORD_TYPE_NUM）
 * @retval 0  成功
 * @retval -1 传参错误
 * @retval -2 未挂载
 * @retval 其他负数: 文件系统出错
 */
int32_t sdk_record_delete(uint32_t type)
{
    sdk_record_t *record_attr;
	int32_t ret = 0;

    if (type >= RECORD_TYPE_NUM)
    { 
        ret = -1;  // 记录类型不存在，返回错误
        goto __exit;
    }

    if (gp_fs[type][0] == 0)
    { 
        ret = -2;  // 文件打开失败
        goto __exit;
    }

    ftruncate(fileno(gp_fs[type][1]), 0);   // 记录的数据文件大小置零
 
    // 复位记录属性
    record_attr = &g_record_attr[type];
    record_attr->index = 0;
	ret = fseek(gp_fs[type][0], 0, SEEK_SET);
    if (ret != 0)//(ret < 0)
    {
        ret = -3;
        goto __exit;
    }

	ret = fwrite(record_attr, 1, sizeof(sdk_record_t), gp_fs[type][0]);
    if (ret != sizeof(sdk_record_t))
    {
        ret = -4;
        goto __exit;     
    }
    else
    {
        ret = 0;
    }

	sync();

__exit:
    if (ret < 0)
    {
        SDK_RECORD_DLOG("sdk_record_sync() fail: %d !! \r\n", ret);
    }
    return ret;
}


 /** 
  * @brief        读取文件的大小 —————————— 临时辅助测试记录删除功能用，后面删除
  * @param        [in] p_fs 已打开的文件指针
  * @return       [int32_t] 执行结果
  * @retval       返回文件大小，单位字节
  */  
 int32_t test_fs_get_size(uint32_t type)	//用stat()实现的话，需要传入文件路径，不采用
 {
	int32_t ret = 0;
    FILE *p_fs;
    
    if (type >= RECORD_TYPE_NUM)
    { 
        ret = -1;  // 记录类型不存在，返回错误
        goto __exit;
    }
	
	if (gp_fs[type][1] == NULL)
	{
		ret = -2;
		goto __exit;
	}  

	fseek((FILE *)gp_fs[type][1], 0, SEEK_END);
	ret = ftell((FILE *)gp_fs[type][1]);	//获取文件尾指针偏移量，即文件大小
	
__exit:
    if (ret < 0)
    {
	    SDK_RECORD_DLOG("Function[%s] Operation failed, ret = %d!\r\n", __func__, ret);
    }
	return ret;		
 }

