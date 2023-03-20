/**
 * @file     sdk_modbus.h
 * @brief    modbus协议SDK接口
 * @author   duyumeng
 * @note     无
 * @version  V1.0
 * @date     2023/1/17
 */
#ifndef __SDK_MODBUS_H__
#define __SDK_MODBUS_H__

#include "data_types.h"

/* Modbus 功能码 */
#define MODBUS_READ_HOLDING_REGISTERS    0x03        //读寄存器
#define MODBUS_WRITE_SINGLE_REGISTER     0x06        //写单个寄存器
#define MODBUS_WRITE_MULTIPLE_REGISTERS  0x10        //写多个寄存器
#define MODBUS_READ_DEVICE_IDENTIFIER    0x2B        //读设备信息
#define MODBUS_WRITE_FILE                0x41        //写片段文件
#define MODBUS_READ_FILE                 0x42        //读片段文件
#define MODBUS_READ_ENERGY               0x43        //读发电量
#define MODBUS_HEART_BEAT                0x49        //心跳
#define MODBUS_DOWNLOAD_UPDATE_FILE      0x50        //下载升级文件
#define MODBUS_START_UPDATE              0x51        //启动升级
#define MODBUS_SLAVE_ADDR_SET            0x52        //设置从机地址

/* Modbus function codes 自定义子功能码 */
#define MODBUS_FILE_TYPE                 0x01        //设置文件类型
#define MODBUS_OPERATE                   0x02        //执行操作，执行读或写
#define MODBUS_END                       0x03        //升级文件传输结束；读升级文件传输状态；
#define MODBUS_START_TIME_SET            0x04        //设置升级启动时间
#define MODBUS_START_TIME_GET            0x05        //读取升级启动时间

/* 设备信息的 Device ID 码 */
#define MODBUS_BASIC_DEVICE_ID           0x01        //basic device identification
#define MODBUS_REGULAR_DEVICE_ID         0x02        //regular device identification
#define MODBUS_EXTENDED_DEVICE_ID        0x03        //extended device identification
#define MODBUS_SPECIFIC_DEVICE_ID        0x04        //specific identification objec

/* Modbus function codes 设备类型功能码 对象ID */
#define MODBUS_COMPANY_IDENTIFICATION    0x00        //供应商名称
#define MODBUS_PRODUCT_CODE              0x01        //产品代码
#define MODBUS_REVISED_VERSION           0x02        //主要修订版本
#define MODBUS_COMPANY_WEB_SITE          0x03        //供应商网址
#define MODBUS_PRODUCT_NAME              0x04        //产品名称
#define MODBUS_TYPE_NAME                 0x05        //型号名称
#define MODBUS_APP_NAME                  0x06        //应用程序名称

#define MODBUS_MAX_MESSAGE_LENGTH        260         //最大数据

#define MODBUS_BROADCAST_ADDR            0          //广播地址

/* 波特率 */
typedef enum{
	MODBUS_BAUD_2400 	    = 2400,	    // 2400波特率
	MODBUS_BAUD_4800 	    = 4800,	    // 4800波特率
    MODBUS_BAUD_9600 	    = 9600,	    // 9600波特率
    MODBUS_BAUD_19200 	    = 19200,	// 19200波特率
    MODBUS_BAUD_38400 	    = 38400,	// 38400波特率
    MODBUS_BAUD_57600 	    = 57600,	// 57600波特率
    MODBUS_BAUD_115200 	    = 115200,	// 115200波特率
}modbus_baud_e;

/* modbus 异常功能码 */
typedef enum {
    MODBUS_ILLEGAL_FUNCTION = 0x01,       //非法功能
    MODBUS_ILLEGAL_DATA_ADDRESS,          //非法数据地址
    MODBUS_ILLEGAL_DATA_VALUE,            //非法数值
    MODBUS_SLAVE_OR_SERVER_FAILURE,       //从站设备故障
    MODBUS_ACKNOWLEDGE,                   //确认
    MODBUS_SLAVE_OR_SERVER_BUSY,          //从设备忙
    MODBUS_NEGATIVE_ACKNOWLEDGE,          //
    MODBUS_MEMORY_PARITY,                 //存储奇偶差错
    MODBUS_NOT_DEFINED,                   //
    MODBUS_GATEWAY_PATH,                  //不可用网关路径
    MODBUS_GATEWAY_TARGET,                //网关目标设备响应失败
    MODBUS_MAX
}modbus_illegal_fun_e;

typedef struct 
{
    uint32_t start_registers;            //寄存器起始地址
    uint32_t number_registers;           //寄存器点表数量
}sdk_modbus_mapping_t;

/**
 * @brief   给寄存器new内存和设置起始地址
 * @param   [in] start_registers   寄存器起始地址
 * @param   [in] number_registers  寄存器点表数量
 * @return  返回执行结果：NULL异常
 */
sdk_modbus_mapping_t* sdk_modbus_mapping_start_address_set(uint32_t start_registers, uint32_t number_registers);

/**
 * @brief   初始化RTU
 * @param   [in] index modbus下标，0~7
 * @param   [in] slave_address 从机地址
 * @param   [in] baud  波特率
 * @note    modbus作为主机和从机时均需使用
 * @return  返回执行结果：0正常；-1异常
 */
int32_t sdk_modbus_rtu_init(uint32_t index, uint32_t slave_address, uint32_t baud);


/**
 * @brief   初始化TCP
 * @param   [in] index modbus下标，8~9
 * @param   [in] slave_address 从机地址
 * @param   [in] ip IP地址
 * @param   [in] port 端口号
 * @note    modbus作为主机和从机时均需使用
 * @return  返回执行结果
 */
int32_t sdk_modbus_tcp_init(uint32_t index, uint32_t slave_address, const char *ip, int32_t port);

/**
 * @brief   监听
 * @param   [in] index modbus下标，8~9
 * @param   [in] connection
 * @note    modbus作为主机和从机时均需使用
 * @return  返回执行结果
 */
int32_t sdk_modbus_tcp_listen(uint32_t index, int32_t connection);

/**
 * @brief   监听
 * @param   [in] index modbus下标，8~9
 * @param   [in] socket
 * @note    modbus作为主机和从机时均需使用
 * @return  返回执行结果
 */
int32_t sdk_modbus_tcp_accept(uint32_t index, int32_t *socket);

/**
 * @brief   设置响应超时时间
 * @param   [in] index modbus下标，0~9
 * @param   [in] timeout_ms 毫秒,范围值0~10000
 * @note    modbus作为主机时的从机响应超时时间
 * @return  返回执行结果：0正常；-1异常
 */
int32_t sdk_modbus_response_timeout_set(uint32_t index, uint32_t timeout_ms);

/**
 * @brief   modbus连接
 * @param   [in] index modbus下标，0~7
 * @note    modbus的rtu函数需要使用，在初始化之后需要调用此函数连接
 * @return  返回执行结果：0正常；-1异常
 */
int32_t sdk_modbus_connect(uint32_t index);

/**
 * @brief   modbus关闭
 * @param   [in] index modbus下标，0~9
 * @return  返回执行结果：0正常；-1：index异常
 */
int32_t sdk_modbus_close(uint32_t index);

/**
 * @brief   modbus释放
 * @param   [in] index modbus下标，0~9
 * @return  返回执行结果：0正常；-1：index异常
 */
int32_t sdk_modbus_free(uint32_t index);

/**
 * @brief   modbus debug模式设置
 * @param   [in] index modbus下标，0~9
 * @param   [in] flag  1打开调试，0关闭调试
 * @return  返回执行结果：0正常；-1：异常
 */
int32_t sdk_modbus_debug_set(uint32_t index, int32_t flag);

/**
 * @brief   modbus 接收数据
 * @param   [in] index modbus下标，0~9
 * @param   [in] req   接收数据
 * @note    modbus作为从机的接收时使用
 * @return  返回执行结果：>0 接收数据长度；-1：异常
 */
int32_t sdk_modbus_receive(uint32_t index, uint8_t *req);

/**
 * @brief   modbus响应格式
 * @param   [in] function 功能码
 * @param   [in] p_req 接收报文的指针,功能码之后的数据
 * @param   [out] p_rsq 响应报文的指针，功能码之后的数据
 * @param   [in] rsq_size 响应报文buff的最大数据值
 * @return  返回执行结果，>0 实际返回的数据长度，< 0 返回异常
 */
typedef int32_t (*modbus_function_cb)(uint32_t function, const uint8_t *p_req, uint8_t *p_rsq, int32_t rsq_size);

/**
 * @brief   用户自定义modbus处理函数
 * @param   [in] index modbus下标，0~9
 * @param   [in] req   接收数据
 * @param   [in] req_length   接收数据
 * @param   [in] pf_modbus_function  回调函数，应用层实现接收数据的解析
 * @note    modbus作为从机时使用，有回调函数注册及使用的功能
 * @return  返回执行结果：>0 发送数据长度；-1：异常
 */
int32_t sdk_modbus_reply(uint32_t index, const uint8_t *req, int req_length, modbus_function_cb pf_modbus_function);


/**
 * @brief   设置从机地址 
 * @param   [in] index modbus下标，0~9
 * @param   [in] slave_address 从机地址 0~255
 * @note    modbus作为主机和从机时均可使用
 * @return  返回执行结果：0正常；-1异常
 */
int32_t sdk_modbus_slave_set(uint32_t index, uint32_t slave_address);


/**
 * @brief   读取保持寄存器值
 * @param   [in] index modbus下标，0~9
 * @param   [in] addr 起始寄存器地址
 * @param   [in] nb 寄存器数量
 * @param   [out] dest 读取的数据的指针
 * @note    modbus作为主机时使用，会发送数据并处理从机返回的数据
 * @return  返回执行结果：-1异常
 */
int32_t sdk_modbus_registers_read(uint32_t index, int32_t addr, int32_t nb, uint16_t *dest);

/**
 * @brief   写单个保持寄存器
 * @param   [in] index modbus下标，0~9
 * @param   [in] addr 寄存器地址
 * @param   [in] value 写入值
 * @note    modbus作为主机时使用，会发送数据并处理从机返回的数据
 * @return  返回执行结果：-1异常
 */
int32_t sdk_modbus_register_write(uint32_t index, int32_t addr, const uint16_t value);


/**
 * @brief   写多个保持寄存器
 * @param   [in] index modbus下标，0~9
 * @param   [in] addr 寄存器起始地址
 * @param   [in] nb 寄存器数量
 * @param   [in] data 写入值的指针
 * @note    modbus作为主机时使用，会发送数据并处理从机返回的数据
 * @return  返回执行结果：-1异常
 */
int32_t sdk_modbus_registers_write(uint32_t index, int32_t addr, int32_t nb, const uint16_t *data);

/**
 * @brief   modbus 接收数据
 * @param   [in] index modbus下标，0~9
 * @param   [in] req   接收数据
 * @note    modbus作为主机的接收时使用
 * @return  返回执行结果：>0 接收数据长度；-1：异常
 */
int32_t sdk_modbus_receive_confirmation(uint32_t index, uint8_t *req);

/**
 * @brief   用户将将要写的数据全部打包
 * @param   [in] index modbus下标，0~9
 * @param   [in] data 需要发送的ADSU
 * @param   [in] data_length 需要发送的ADSU长度
 * @note    user需要将一整包数据包含 从机地址、功能码等数据全部打包，底层只是将数据按rtu和tcp组包后发送，在做主机时使用
 * @return  返回执行结果：-1异常
 */
int32_t sdk_modbus_write(uint32_t index, const uint8_t *data, uint8_t data_length);

#endif


