/**
 * @file     	sdk_record.h
 * @brief    	记录存储sdk接口
 * @author   	yuanzhihua
 * @note     	无
 * @version  	V1.0.1
 * @date     	2022/12/30
 * @copyright   Copyright(c) 2022 by SofarSolar, All Rights Reserved. 
 */

#ifndef __SDK_RECORD_H__
#define __SDK_RECORD_H__

#include "data_types.h"

enum {

    RECORD_TYPE_NUM = 500,	// 记录的类型总数，定义最大值
};



#define RECORD_ONE_SIZE     512
#define RECORD_DEEP         10000


/**
 * @brief  设置记录的存储路径
 * @param  [in] type 记录类型（枚举中定义的值，<RECORD_TYPE_NUM）
 * @param  [in] p_path 要设置的记录存储的路径（长度必须小于123字符）
 * @retval 0  成功
 * @retval -1 传参错误
 * @retval 
 * @note   该接口仅在应用对存储路径名有特殊要求时需使用；若无要求，则不需调用，记录会存储在默认路径
 * @note   使用时，应用必须在调用sdk_record_init之前先调用sdk_record_path_set接口，路径设置才能生效。
 */
int32_t sdk_record_path_set(uint32_t type, const int8_t *p_path);



/**
 * @brief  按记录类型挂载记录区
 *         注意，每次上电都要挂载，挂载传的配置参数如果不一样，会清空之前的记录
 * @param  [in] type 记录类型（枚举中定义的值，<RECORD_TYPE_NUM）
 * @param  [in] one_record_size 单条记录的大小(1~512字节) 
 * @param  [in] max_num 最大记录深度(1~10000条) 
 * @retval 0  成功
 * @retval -1 传参错误
 * @retval 其他负数，为文件系统出错
 */
int32_t sdk_record_init(uint32_t type, uint32_t one_record_size, uint32_t max_num);

/**
 * @brief  获取可记录最大数目
 * @param  [in] type 记录类型（枚举中定义的值，<RECORD_TYPE_NUM）
 * @retval >0 最大深度(返回初始化时的max_num参数值)
 * @retval -1 传参错误
 * @retval -2 未挂载
 * @pre    调用sdk_record_init()之后才生效。
 */
int32_t sdk_record_get_max_num(uint32_t type);

/**
 * @brief  获取当前存储了多少条记录
 * @param  [in] type 记录类型（枚举中定义的值，<RECORD_TYPE_NUM）
 * @retval 0~N 存储了多少条
 * @retval -1 传参错误
 * @retval -2 未挂载
 * @pre    调用sdk_record_init()之后才生效。
 */
int32_t sdk_record_get_index(uint32_t type);

/**
 * @brief  写记录内容
 * @param  [in] type  记录类型（枚举中定义的值，<RECORD_TYPE_NUM）
 * @param  [in] index 内容索引: 取值范围[0], 0为新增一条记录
 * @param  [in] buf   内容
 * @param  [in] len   内容长度
 * @retval -1 传参错误
 * @retval -2 未挂载
 * @retval 其他负数: 文件系统出错
 * @pre    调用sdk_record_init()之后才生效。
 */
int32_t sdk_record_write(uint32_t type, int32_t index, uint8_t *p_buf, uint32_t len);

/**
 * @brief  读记录内容
 * @param  [in] type  记录类型（枚举中定义的值，<RECORD_TYPE_NUM）
 * @param  [in] index 内容索引: 取值范围[0~max_num], 0为最新的记录，1为次新的记录...直到最大记录数为止
 * @param  [in] buf   内容
 * @param  [in] len   内容长度
 * @retval >=0 成功读取到的字节长度
 * @retval -1 传参错误
 * @retval -2 未挂载
 * @retval 其他负数: 文件系统出错
 * @pre    调用sdk_record_init()之后才生效。
 */
int32_t sdk_record_read(uint32_t type, int32_t index, uint8_t *p_buf, uint32_t len);

/**
 * @brief     同步记录(保存数据，否则新写的数据不会保存到flash)
 * @param[in] type 记录类型（枚举中定义的值，<RECORD_TYPE_NUM）
 * @retval 0  成功
 * @retval -1 传参错误
 * @retval -2 未挂载
 * @retval 其他负数: 文件系统出错
 */
int32_t sdk_record_sync(uint32_t type);

/**
 * @brief     删除所有记录
 * @param[in] type 记录类型（枚举中定义的值，<RECORD_TYPE_NUM）
 * @retval 0  成功
 * @retval -1 传参错误
 * @retval -2 未挂载
 * @retval 其他负数: 文件系统出错
 */
int32_t sdk_record_delete(uint32_t type);


#endif

