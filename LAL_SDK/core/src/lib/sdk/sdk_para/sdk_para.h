#ifndef __SDK_PARA_H__
#define __SDK_PARA_H__

/**
 * @file             sdk_para.h
 * @brief            参数存储sdk接口
 * @author           yuanzhihua
 * @note             无
 * @version          V1.0.1
 * @date             2022/12/30
 * @copyright   Copyright(c) 2022 by SofarSolar, All Rights Reserved. 
 */

#include "data_types.h"

#define PARA_TYPE_NUM   	100		//应用在使用时，定义的参数类型type不能超过PARA_TYPE_NUM。PARA_TYPE_NUM的值，待定义

#define PARA_SIZE_LIMIT		1024*2	//单个参数文件限制大小，按2K. 若需求变化，可调整


/**
 * @brief  按参数区类型挂载参数
 *         多次挂载没有副作用
 * @param  [in] type 参数区类型（取值范围：<PARA_TYPE_NUM）
 * @param  [in] para_size 参数区大小（取值范围：<PARA_SIZE_LIMIT）
 * @retval 0  成功
 * @retval -1 参数区不存在
 * @retval -2 参数大小非法
 * @retval -3 文件系统错误
 */
int32_t sdk_para_init(uint32_t type, uint32_t para_size);

/**
 * @brief  按参数区类型写内容
 * @param  [in] type 参数区类型（（取值范围：<PARA_TYPE_NUM）
 * @param  [in] offset 参数中的位置 (offset+len < para_size)
 * @param  [in] buf 内容
 * @param  [in] len 内容长度 (offset+len < para_size)
 * @retval 0  成功
 * @retval -1 参数区不存在
 * @retval -2 参数区未挂载
 * @retval -3 写入超出长度
 * @retval -4 -5 -6 文件系统错误
 */
int32_t sdk_para_write(uint32_t type, uint32_t offset, uint8_t *p_buf, uint32_t len);

/**
 * @brief  按参数区类型读参数
 * @param  [in]  type 参数区类型（取值范围：<PARA_TYPE_NUM）
 * @param  [in]  offset 参数中的位置 (offset+len < para_size)
 * @param  [out] buf 内容
 * @param  [in]  len 内容长度 (offset+len < para_size)
 * @retval 0  成功
 * @retval -1 参数区不存在
 * @retval -2 参数区未挂载
 * @retval -3 写入超出长度
 * @retval -4 -5 -6 文件系统错误
 */
int32_t sdk_para_read(uint32_t type, uint32_t offset, uint8_t *p_buf, uint32_t len);

/**
 * @brief  同步参数的数据到flash
 * @param  [in] type 参数区类型（取值范围：<PARA_TYPE_NUM）
 * @retval 0  成功
 * @retval -1 失败
 */
int32_t sdk_para_sync(uint32_t type);

#endif
