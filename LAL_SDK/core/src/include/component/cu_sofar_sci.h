#ifndef __CU_SOFAR_SCI_H__
#define __CU_SOFAR_SCI_H__

#include "stdint.h"

/**
 * @enum  内部SCI通讯错误类型
 * @brief 内部SCI通讯错误类型
 */
typedef enum {
    SCI_ACK_OK                =  0,   /** No error, everything OK.  */
    SCI_ACK_PARA_ERROR        = -1,   /** Formal parameter error.   */
    SCI_ACK_HEAD_ERROR        = -2,   /** Head error.               */
    SCI_ACK_ADDR_ERROR        = -3,   /** Addres error.             */
    SCI_ACK_FUNCID_ERROR      = -4,   /** FunctionID error.         */
    SCI_ACK_CHKSUM_ERROR      = -5,   /** Checksum error.           */
    SCI_ACK_RESULT_ERROR      = -6,   /** Result.                   */
    SCI_ACK_TIMEOUT_ERROR     = -7,   /** Timeout.                  */
    SCI_ACK_OTHER_ERROR       = -8,   /** Other.                    */
    SCI_SEND_ERROR            = -9,   /** Send err.                 */
    SCI_RECEIVE_ERROR         = -10,  /** Receive err.              */
} cu_sofar_sci_err_e;

#pragma pack(1)
typedef struct{
    uint8_t version;           // 协议版本
    uint8_t dev_addr;          // 设备地址
    uint16_t function_id;      // 功能码
    uint16_t data_len;         // 数据段的长度（字节数）
    uint8_t *p_data;           // 数据段缓存指针
} cu_sofar_sci_t;
#pragma pack()

/**
 * @brief  内部SCI通讯打包报文函数
 * @param  [in]  p_sofar_sci sci结构体缓存，具体见cu_sofar_sci_t
 * @param  [out] p_buf      打包好的报文buf
 * @param  [in]  buf_size   buf的大小
 * @return 打包后的报文字节数，-1为异常
 */
int16_t cu_sofar_sci_pack(cu_sofar_sci_t *p_sofar_sci, uint8_t *p_buf, uint16_t buf_size);

/**
 * @brief  内部SCI通讯报文解包函数
 * @param  [in] dev_addr     期望解包后的设备地址
 * @param  [in] function_id  期望解包后的功能码
 * @param  [out] p_sofar_sci sci结构体缓存，具体见cu_sofar_sci_t
 * @param  [in] p_buf        报文帧buf
 * @param  [in] frame_len    报文帧的长度
 * @return 0(SCI_ACK_OK)为正常，负数异常：具体见cu_sofar_sci_err_e
 */
int16_t cu_sofar_sci_unpack(uint8_t dev_addr, uint16_t function_id, cu_sofar_sci_t *p_sofar_sci, uint8_t *p_buf, uint16_t frame_len);

#endif /* __CU_SOFAR_SCI_H__  */
