/** 
 * @file          sdk_dido.h
 * @brief         干接点的输入输出管理
 * @author        qinmingsheng
 * @version       V0.0.2     初始版本
 * @date          2022/12/20 11:12:30
 * @lastAuthor    Please set LastEditors
 * @copyright     Copyright (c) 2022 by SofarSolar, All Rights Reserved. 
 **********************************************************************************
 * @par 修改日志:
 * <table>
 * <tr><th>Date        <th>Version  <th>Author    <th>Description
 * <tr><td>2023/02/23  <td>0.0.1    <td>apoi     <td>创建初始版本
 * <tr><td>2023/03/07  <td>0.0.2    <td>apoi     <td>取消枚举，改为MAX_ID ,参数中的_index统一改为_id
 * </table>
 *
 **********************************************************************************
 * @par 示例:
 * @code
 * 暂无
 * @endcode
 */

#ifndef __SDK_DIDO_H__
#define __SDK_DIDO_H__

#include "data_types.h"
#include <stdbool.h>

typedef enum
{
    DI_IRQ_LOW     = 0U,          //低电平中断
    DI_IRQ_HIGH    = 1U,          //高电平中断
    DI_IRQ_FALLING = 2U,          //下降沿中断
    DI_IRQ_RISING  = 3U,          //上升沿中断
    DI_IRQ_RISING_FALLING  = 4U,  //上升下降都中断
}dido_irq_e;

#define DO_INDEX_MAX    12   //所有硬件平台中支持的最大数，实际值见不同硬件平台的说明
#define DI_INDEX_MAX    12   //所有硬件平台中支持的最大数，实际值见不同硬件平台的说明

typedef void(*f_dido_irq_callback)(void *args);   //定义的回调函数类型

/** 
 * @brief        写数据到指定的DO口
 * @param        [in] do_id  DO的编号（索引）值
 * @param        [in] value 传入参数，所要写入的值：false或true。
 * @return       [int32_t] 执行结果
 * @retval       =0 成功
 * @retval       <0 失败 -1：索引不存在或超过最大值
 * @note         
 * @date         2022/12/20 13:30:47
 * @lastAuthor   APOI
 */
int32_t  sdk_dido_write (uint8_t do_id, bool value);

/** 
  * @brief        读取指定索引值的 DI的数值
  * @param        [in] di_id 传入参数，索引值
  * @return       [int32_t] 执行结果
  * @retval       0或1 成功，表示对应的数值
  * @retval       <0 失败，如：-1 索引不存在或超过最大值
  * @note         
  * @date         2022/12/20 13:30:53
  * @lastAuthor   APOI
 */
int32_t  sdk_dido_read (uint8_t di_id);

/** 
 * @brief        设置接口的中断方式和中断后的回调函数
 * @param        [in] di_id 传入参数，输入接口的索引
 * @param        [in] mode 传入参数，中断模式，
 * @param        [in] p_fcallback  传入参数，回调函数入口地址，当有中断产生时调用此函数。
 * @return       [int32_t] 执行结果
 * @retval       =0 成功
 * @retval       <0 失败，如：-1 索引不存在或超过最大值
 * @note         回调函数中禁止使用延时
 * @date         2022/12/20 13:31:01
 * @lastAuthor   APOI
 */
int32_t  sdk_dido_set_irq (uint8_t di_id, dido_irq_e mode, f_dido_irq_callback p_fcallback);

/** 
 * @brief        释放输入口的中断
 * @param        [in] di_id 传入参数，输入接口的索引
 * @return       [int32_t] 执行结果
 * @retval       =0 成功
 * @retval       <0 失败，如：-1 索引不存在或超过最大值
 * @note         释放中断后，此接口恢复为普通的输入端口。
 * @date         2022/12/20 13:31:06
 * @lastAuthor   APOI
 */
int32_t  sdk_dido_free_irq (uint8_t di_id);


#endif /* __SDK_DIDO_H__  */
