/**
 * @file     cu_sofar_sci.c
 * @brief    内部SCI通讯通用接口处理（sofar自定义sci协议）
 * @copyright Shouhang
 * @author   hewenjun
 * @note     2023.02.20 新拟制
 * @version  V1.0   
 * @date     2023/02/20
 */
#include "cu_sofar_sci.h"
#include <stdio.h>
#include <string.h>

static int16_t sofar_sci_pack_ver1(cu_sofar_sci_t *p_sofar_sci, uint8_t *p_buf, uint16_t buf_size);
static int16_t sofar_sci_pack_ver2(cu_sofar_sci_t *p_sofar_sci, uint8_t *p_buf, uint16_t buf_size);
static int16_t sofar_sci_unpack_ver1(uint8_t dev_addr, uint16_t function_id, cu_sofar_sci_t *p_sofar_sci, uint8_t *p_buf, uint16_t frame_len);
static int16_t sofar_sci_unpack_ver2(uint8_t dev_addr, uint16_t function_id, cu_sofar_sci_t *p_sofar_sci, uint8_t *p_buf, uint16_t frame_len);
static uint16_t calculate_checksum(uint8_t*p_buf, uint16_t len);

/**
 * @brief   校验和
 * @param   [in]p_buf要参与计算的数据块起始地址
 * @param   [in]len要参与计算的数据长度（字节）
 * @return  void
 */
static uint16_t calculate_checksum(uint8_t*p_buf, uint16_t len)
{
    uint16_t i;
    uint16_t sum = 0;

    for (i=0; i<len; i++)
    {
        sum = sum + *(p_buf + i);
    }
    return sum;
}

/**
* @brief  (作为主机)内部SCI通讯打包函数，协议版本0x01
 * @param  [in] p_sofar_sci sci结构体缓存，具体见sdk_sofar_sci_t
 * @param  [out] p_buf      打包好的报文buf
 * @param  [in]  buf_size   buf的大小
 * @return 打包后的报文字节数，-1(SCI_ACK_PARA_ERROR)为异常
 * @note   上层函数已保证p_sofar_sci、p_buf的可靠性，这里不需要再验证
 */
static int16_t sofar_sci_pack_ver1(cu_sofar_sci_t *p_sofar_sci, uint8_t *p_buf, uint16_t buf_size)
{
    uint16_t checksum;
    uint16_t i;

    if (p_sofar_sci->data_len + 9 > buf_size)
    {
        return SCI_ACK_PARA_ERROR;
    }

    p_buf[0] = 0x55;
    p_buf[1] = 0xAA;
    p_buf[2] = p_sofar_sci->dev_addr;
    p_buf[3] = p_sofar_sci->data_len + 3;
    p_buf[4] = 0;
    p_buf[5] = (uint8_t)((p_sofar_sci->function_id >> 8) & 0xff);
    p_buf[6] = (uint8_t)(p_sofar_sci->function_id & 0xff);

    for(i = 0; i < p_sofar_sci->data_len; i++)
    {
        p_buf[7+i] = p_sofar_sci->p_data[i];
    }

    checksum = calculate_checksum(p_buf + 2, p_sofar_sci->data_len + 5);
    p_buf[7 + p_sofar_sci->data_len] = (uint8_t)(checksum & 0x00FF);
    p_buf[8 + p_sofar_sci->data_len] = (uint8_t)((checksum & 0xFF00)>>8);

    return (p_sofar_sci->data_len + 9);
}

/**
 * @brief  内部SCI通讯打包函数，协议版本0x02
 * @param  [in] p_sofar_sci sci结构体缓存，具体见sdk_sofar_sci_t
 * @param  [out] p_buf      打包好的报文buf
 * @param  [in]  buf_size   buf的大小
* @return 打包后的报文字节数，-1(SCI_ACK_PARA_ERROR)为异常
 * @note   上层函数已保证p_sofar_sci、p_buf的可靠性，这里不需要再验证
 */
static int16_t sofar_sci_pack_ver2(cu_sofar_sci_t *p_sofar_sci, uint8_t *p_buf, uint16_t buf_size)
{
    uint16_t checksum;
    uint16_t i;

    if (p_sofar_sci->data_len + 8 > buf_size)
    {
        return SCI_ACK_PARA_ERROR;
    }

    p_buf[0] = 0x55;
    p_buf[1] = 0xBB;
    p_buf[2] = p_sofar_sci->dev_addr;
    p_buf[3] = p_sofar_sci->data_len;
    p_buf[4] = (uint8_t)((p_sofar_sci->function_id >> 8) & 0xff);
    p_buf[5] = (uint8_t)(p_sofar_sci->function_id & 0xff);

    for (i = 0; i < p_sofar_sci->data_len; i++)
    {
        p_buf[6 + i] = p_sofar_sci->p_data[i];
    }

    checksum = calculate_checksum(p_buf + 2, p_sofar_sci->data_len + 4);
    // SCI协议0x02版本要考虑send_data_len为0的情况，以下已考虑
    p_buf[6 + p_sofar_sci->data_len] = (uint8_t)(checksum & 0x00FF);
    p_buf[7 + p_sofar_sci->data_len] = (uint8_t)((checksum & 0xFF00)>>8);

    return (p_sofar_sci->data_len + 8);
}

/**
 * @brief  内部SCI通讯打包报文函数
 * @param  [in]  p_sofar_sci sci结构体缓存，具体见cu_sofar_sci_t
 * @param  [out] p_buf      打包好的报文buf
 * @param  [in]  buf_size   buf的大小
 * @return 打包后的报文字节数，-1为异常
 */
int16_t cu_sofar_sci_pack(cu_sofar_sci_t *p_sofar_sci, uint8_t *p_buf, uint16_t buf_size)
{
    int16_t ret;

    if ((p_sofar_sci == NULL) || (p_sofar_sci->p_data == NULL) || (p_buf == NULL))
    {
        return SCI_ACK_PARA_ERROR;
    }

    switch (p_sofar_sci->version)
    {
        case 0x01:
            ret = sofar_sci_pack_ver1(p_sofar_sci, p_buf, buf_size);
            break;
        case 0x02:
            ret = sofar_sci_pack_ver2(p_sofar_sci, p_buf, buf_size);
            break;
        default:
            ret = SCI_ACK_PARA_ERROR;
            break;
    }

    return ret;
}

/**
 * @brief  (作为主机)内部SCI通讯解包函数，协议版本0x01
 * @param  [in] dev_addr     期望解包后的设备地址
 * @param  [in] function_id  期望解包后的功能码
 * @param  [out] p_sofar_sci sci结构体缓存，具体见sdk_sofar_sci_t
 * @param  [in] p_buf        报文帧buf
 * @param  [in] frame_len    报文帧的长度
 * @return 0(SCI_ACK_OK)为正常，负数异常：具体见cu_sofar_sci_err_e
 * @note   上层函数已保证p_sofar_sci、p_buf的可靠性，这里不需要再验证
 */
static int16_t sofar_sci_unpack_ver1(uint8_t dev_addr, uint16_t function_id, cu_sofar_sci_t *p_sofar_sci, uint8_t *p_buf, uint16_t frame_len)
{
    uint16_t checksum;

    if ((p_buf[3] - 2 + 8 > frame_len) || (p_buf[3] < 2))
    {
        return SCI_ACK_PARA_ERROR;
    }

    p_sofar_sci->dev_addr = p_buf[2];
    p_sofar_sci->data_len = p_buf[3] - 2; // >= 0
    p_sofar_sci->function_id = (uint16_t)(p_buf[4] << 8) + p_buf[5];

     if (p_sofar_sci->dev_addr != dev_addr)
    {
        return SCI_ACK_ADDR_ERROR; // 设备地址不匹配
    }

    if ((p_sofar_sci->function_id != function_id) && (p_sofar_sci->function_id != 0xFFFF)) // 0xFFFF为协议0x02版本的异常回复功能码
    {
        return SCI_ACK_FUNCID_ERROR; // 功能码不匹配
    }

    checksum = calculate_checksum(p_buf + 2, p_sofar_sci->data_len + 4);
    if (((checksum & 0x00FF) != p_buf[p_sofar_sci->data_len + 6]) ||
        (((checksum & 0xFF00) >> 8) != p_buf[p_sofar_sci->data_len + 7]))
    {
        return SCI_ACK_CHKSUM_ERROR; // 解包checksum校验失败
    }
    if (p_sofar_sci->data_len != 0)
    {
        memcpy(p_sofar_sci->p_data, p_buf + 6, p_sofar_sci->data_len);
    }

    return SCI_ACK_OK;
}

/**
 * @brief  内部SCI通讯解包函数，协议版本0x02
 * @param  [in] dev_addr     期望解包后的设备地址
 * @param  [in] function_id  期望解包后的功能码
 * @param  [out] p_sofar_sci sci结构体缓存，具体见sdk_sofar_sci_t
 * @param  [in] p_buf        报文帧buf
 * @param  [in] frame_len    报文帧的长度
 * @return 0(SCI_ACK_OK)为正常，负数异常：具体见cu_sofar_sci_err_e
 * @note   上层函数已保证p_sofar_sci、p_buf的可靠性，这里不需要再验证
 */
static int16_t sofar_sci_unpack_ver2(uint8_t dev_addr, uint16_t function_id, cu_sofar_sci_t *p_sofar_sci, uint8_t *p_buf, uint16_t frame_len)
{
    uint16_t checksum;

    if (p_buf[3] + 8 > frame_len)
    {
        return SCI_ACK_PARA_ERROR;
    }

    p_sofar_sci->dev_addr = p_buf[2];
    p_sofar_sci->data_len = p_buf[3];
    p_sofar_sci->function_id = (uint16_t)(p_buf[4] << 8) + p_buf[5];
 
    if (p_sofar_sci->dev_addr != dev_addr)
    {
        return SCI_ACK_ADDR_ERROR;   // 设备地址不匹配
    }
    if ((p_sofar_sci->function_id != function_id) && (p_sofar_sci->function_id != 0xFFFF)) // 0xFFFF为协议0x02版本的异常回复功能码
    {
        return SCI_ACK_FUNCID_ERROR; // 功能码不匹配
    }
 
    checksum = calculate_checksum(p_buf + 2, p_sofar_sci->data_len + 4);
    if (((checksum & 0x00FF) != p_buf[p_sofar_sci->data_len + 6]) ||
        (((checksum & 0xFF00) >> 8) != p_buf[p_sofar_sci->data_len + 7]))
    {
        return SCI_ACK_CHKSUM_ERROR; // 解包checksum校验失败
    }
    if (p_sofar_sci->data_len != 0)
    {
        memcpy(p_sofar_sci->p_data, p_buf + 6, p_sofar_sci->data_len);
    }

    return SCI_ACK_OK;
}

/**
 * @brief  内部SCI通讯报文解包函数
 * @param  [in] dev_addr     期望解包后的设备地址
 * @param  [in] function_id  期望解包后的功能码
 * @param  [out] p_sofar_sci sci结构体缓存，具体见cu_sofar_sci_t
 * @param  [in] p_buf        报文帧buf
 * @param  [in] frame_len    报文帧的长度
 * @return 0(SCI_ACK_OK)为正常，负数异常：具体见cu_sofar_sci_err_e
 */
int16_t cu_sofar_sci_unpack(uint8_t dev_addr, uint16_t function_id, cu_sofar_sci_t *p_sofar_sci, uint8_t *p_buf, uint16_t frame_len)
{
    int16_t ret;
    uint16_t start_word; // 起始字

    if ((p_sofar_sci == NULL) || (p_sofar_sci->p_data == NULL) || (p_buf == NULL))
    {
        return SCI_ACK_PARA_ERROR;
    }
    if (frame_len < 2)
    {
        return SCI_ACK_PARA_ERROR;
    }

    start_word = p_buf[1] | (uint16_t)(p_buf[0] << 8);
    switch (start_word)
    {
        case 0x55AA:
            p_sofar_sci->version = 0x01;
            ret = sofar_sci_unpack_ver1(dev_addr, function_id, p_sofar_sci, p_buf, frame_len);
            break;
        case 0x55BB:
            p_sofar_sci->version = 0x02;
            ret = sofar_sci_unpack_ver2(dev_addr, function_id, p_sofar_sci, p_buf, frame_len);
            break;
        default:
            ret = SCI_ACK_HEAD_ERROR;
            break;
    }

    return ret;
}

 /*****************************************************************************
 **                                End Of File
 ******************************************************************************/
