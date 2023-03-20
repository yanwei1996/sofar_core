/** 
  * @author        qinmingsheng
  * @date          2022/12/20 08:37:20
  * @lastAuthor    qinmingsheng
  * @lastTime      2022/12/20 08:52:24
  * @filePath      /Linux_SDK/lib/drm.h
  * @brief         与DRM有关的状态获取
  * @Copyright (c) 2022 by SofarSolar, All Rights Reserved. 
 */
#ifndef __SDK_DRM_H__
#define __SDK_DRM_H__

#include "data_types.h"

/** 
  * @brief        DRM0状态获取
  * @return       [int32_t] 返回执行结果
  * @note         
  * @retval       0、1代表DRM0对应的状态，小于0对应的错误代码。
  * @date         2022/12/20 08:39:17
  * @lastEditors  APOI
 */
int32_t sdk_drms0_get(void);
/** 
  * @brief        获取drm1-drm8的状态值（目前最大值数量是8个）
  * @param        [uint32_t] *p_drmn:转出参数，存放DRMN的有效值，不同的bit值组合对应的是功率输出值 bit0-bit7,对应DRM1-DRM8。
  * @return       [int32_t]
  * @note         
  * @retval       bit0 --bit8,对应编号为DRM1-DRM8的AFCI值（p_drmn）是否有效
  * @date         2022/12/20 08:39:30
  * @lastEditors  APOI
 */
int32_t sdk_drmn_get(uint32_t *p_drmn);

#endif