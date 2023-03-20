#include <stdio.h>
#include <string.h>
#include "modbus.h"
#include "modbus-private.h"
#include "sdk_modbus.h"
#include "string.h"

#define RS485_RE GET_PIN(D, 7) // 临时

#define RS485_MODBUS_MAX    8
#define ETH_MODBUS_MAX      2
#define MODBUS_MAX          ((RS485_MODBUS_MAX) + (ETH_MODBUS_MAX))

static modbus_t *gp_modbus_ctx[MODBUS_MAX] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
static uint32_t g_modbus_slave[MODBUS_MAX] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}; // 默认地址为1
static uint32_t g_modbus_response_timeout[MODBUS_MAX] = {1000000, 1000000, 1000000, 1000000, 1000000, 1000000, 1000000, 1000000, 1000000, 1000000}; // 默认超时时间，单位微秒
static sdk_modbus_mapping_t *g_modbus_mapping = NULL;

/**
 * @brief   给寄存器new内存和设置起始地址
 * @param   [in] start_registers   寄存器起始地址
 * @param   [in] number_registers  寄存器点表数量
 * @return  返回执行结果：NULL异常
 */
sdk_modbus_mapping_t* sdk_modbus_mapping_start_address_set(uint32_t start_registers, uint32_t number_registers)
{
    sdk_modbus_mapping_t *sdk_modbus_mapping = NULL;
    modbus_mapping_t *mb_mapping = NULL;

    sdk_modbus_mapping = g_modbus_mapping;

	mb_mapping = modbus_mapping_new_start_address(
					0, 0,
					0, 0,
					start_registers, number_registers,
					0, 0); 
    
    sdk_modbus_mapping->start_registers = mb_mapping->start_registers;
    sdk_modbus_mapping->number_registers = mb_mapping->nb_registers;

    return (sdk_modbus_mapping);
}


/**
 * @brief   初始化RTU
 * @param   [in] index modbus下标，0~7
 * @param   [in] baud 波特率
 * @return  返回执行结果：0正常；-1异常
 */
int32_t sdk_modbus_rtu_init(uint32_t index, uint32_t slave_address, uint32_t baud)
{
    modbus_t *ctx = NULL;
    // uint32_t slave = 0;
    uint32_t timeout = 0;
    uint8_t uart_name[32] = {0};

    if (index >= RS485_MODBUS_MAX)
    {
        return -1;
    }

    if ((baud != 115200) && (baud != 57600) && (baud != 38400) && (baud != 19200) &&
        (baud != 9600) && (baud != 4800) && (baud != 2400))
    {
        return -1;
    }
    
	if (index < 8)
	{
		sprintf((char *)uart_name, "/dev/ttymxc%d", index);
	}
	else
	{
		// printf("[%s:%s:%d] uart device number error \n", __FILE__,__func__, __LINE__);
        return -1;
	}    

    gp_modbus_ctx[index] = modbus_new_rtu(uart_name, baud, 'N', 8, 1); 
    ctx = gp_modbus_ctx[index];

    g_modbus_slave[index] = slave_address;
    timeout = g_modbus_response_timeout[index];

    modbus_rtu_set_serial_mode(ctx, MODBUS_RTU_RS485);
    modbus_set_slave(ctx, slave_address);
    modbus_connect(ctx);
    modbus_set_response_timeout(ctx, 0, timeout);

    return 0;
}

/**
 * @brief   初始化TCP
 * @param   [in] index modbus下标，8~9
 * @param   [in] ip IP地址
 * @param   [in] port 端口
 * @note    modbus作为主机和从机时均需使用
 * @return  返回执行结果
 */
int32_t sdk_modbus_tcp_init(uint32_t index, uint32_t slave_address, const char *ip, int32_t port)
{
    modbus_t *ctx = NULL;
    uint32_t timeout = 0;
    int32_t s = -1;

    if (ip == NULL)
    {
        return -1;
    }

    if ((index >= MODBUS_MAX) || (index < RS485_MODBUS_MAX))
    {
        return -1;
    }

    gp_modbus_ctx[index] = modbus_new_tcp(ip, port);
    ctx = gp_modbus_ctx[index];

    g_modbus_slave[index] = slave_address;
    timeout = g_modbus_response_timeout[index];

    modbus_set_slave(ctx, slave_address);
    s = modbus_tcp_listen(ctx, 1);
    modbus_tcp_accept(ctx, (int *)&s);  
    modbus_set_response_timeout(ctx, 0, timeout); 

    return 0; 
}

/**
 * @brief   设置从机地址
 * @param   [in] index modbus下标，0~6
 * @param   [in] slave_address 从机地址 1~255
 */
int32_t sdk_modbus_slave_set(uint32_t index, uint32_t slave_address)
{
    modbus_t *ctx = NULL;
    if (index >= MODBUS_MAX)
    {
        return -1;
    }

    if (slave_address > 0 && slave_address <= 255)
    {
        ctx = gp_modbus_ctx[index];
        g_modbus_slave[index] = slave_address;
        modbus_set_slave(ctx, g_modbus_slave[index]);

        return 0;
    }
    else
    {
        return -1;
    }
}

/**
 * @brief   modbus debug模式设置
 * @param   [in] index modbus下标，0~9
 * @param   [in] flag  1打开调试，0关闭调试
 * @return  返回执行结果：0正常；-1：异常
 */
int32_t sdk_modbus_debug_set(uint32_t index, int32_t flag)
{
    modbus_t *ctx = NULL;
    if (index >= MODBUS_MAX)
    {
        return -1;
    }

    ctx = gp_modbus_ctx[index];
    modbus_set_debug(ctx, flag);

    return 0; 
}

/**
 * @brief   监听
 * @param   [in] index modbus下标，8~9
 * @param   [in] connection
 * @note    modbus作为主机和从机时均需使用
 * @return  返回执行结果
 */
int32_t sdk_modbus_tcp_listen(uint32_t index, int32_t connection)
{
    modbus_t *ctx = NULL;
    int32_t s = -1;

    if ((index >= MODBUS_MAX) || (index < RS485_MODBUS_MAX))
    {
        return -1;
    }

    ctx = gp_modbus_ctx[index];
    s = modbus_tcp_listen(ctx, 1);
  
    return (s);
}

/**
 * @brief   监听
 * @param   [in] index modbus下标，8~9
 * @param   [in] socket
 * @note    modbus作为主机和从机时均需使用
 * @return  返回执行结果
 */
int32_t sdk_modbus_tcp_accept(uint32_t index, int32_t *socket)
{
    modbus_t *ctx = NULL;

    if ((index >= MODBUS_MAX) || (index < RS485_MODBUS_MAX))
    {
        return -1;
    }

    ctx = gp_modbus_ctx[index];
    modbus_tcp_accept(ctx, socket);

    return (0);
}

/**
 * @brief   设置响应超时时间
 * @param   [in] index modbus下标，0~6
 * @param   [in] timeout_ms 毫秒,范围值0~10000
 * @return  返回执行结果：0正常；-1异常
 */
int32_t sdk_modbus_response_timeout_set(uint32_t index, uint32_t timeout_ms)
{
    modbus_t *ctx = NULL;

    if (index >= MODBUS_MAX)
    {
        return -1;
    }
    if (timeout_ms > 10000)
    {
        return -1;
    }

    g_modbus_response_timeout[index] = timeout_ms * 1000; // 毫秒转成微秒
    ctx = gp_modbus_ctx[index];
    modbus_set_response_timeout(ctx, 0, g_modbus_response_timeout[index]);

    return 0;
}

/**
 * @brief   modbus连接
 * @param   [in] index modbus下标，0~6
 * @return  返回执行结果：0正常；-1异常
 */
int32_t sdk_modbus_connect(uint32_t index)
{
    modbus_t *ctx = NULL;
    if (index >= RS485_MODBUS_MAX)
    {
        return -1;
    }

    ctx = gp_modbus_ctx[index];
    return modbus_connect(ctx);
}

/**
 * @brief   modbus关闭
 * @param   [in] index modbus下标，0~6
 * @return  返回执行结果：0正常；-1：index异常
 */
int32_t sdk_modbus_close(uint32_t index)
{
    modbus_t *ctx = NULL;
    if (index >= MODBUS_MAX)
    {
        return -1;
    }

    ctx = gp_modbus_ctx[index];
    modbus_close(ctx);

    return 0;
}

/**
 * @brief   modbus释放
 * @param   [in] index modbus下标，0~6
 * @return  返回执行结果：0正常；-1：index异常
 */
int32_t sdk_modbus_free(uint32_t index)
{
    modbus_t *ctx = NULL;
    if (index >= MODBUS_MAX)
    {
        return -1;
    }

    ctx = gp_modbus_ctx[index];
    modbus_free(ctx);

    return 0;
}


/**
 * @brief   modbus 接收数据
 * @param   [in] index modbus下标，0~6
 * @param   [in] req   接收数据
 * @return  返回执行结果：>0 接收数据长度；-1：异常
 */
int32_t sdk_modbus_receive(uint32_t index, uint8_t *req)
{
    int32_t rc = 0;
    modbus_t *ctx = NULL;

    if (req == NULL)
    {
        return -1;
    }

    if (index >= RS485_MODBUS_MAX)
    {
        return -1;
    }

    ctx = gp_modbus_ctx[index];
    rc = modbus_receive(ctx, req);

    return(rc);
}

/**
 * @brief   用户自定义modbus处理函数
 * @param   [in] index modbus下标，0~6
 * @param   [in] req   接收数据
 * @param   [in] req_length   接收数据
 * @param   [in] pf_modbus_function  回调函数，应用层实现接收数据的解析
 * @note    modbus作为从机时使用，有回调函数注册及使用的功能
 * @return  返回执行结果：>0 接收数据长度；-1：异常
 */
int32_t sdk_modbus_reply(uint32_t index, const uint8_t *req, int req_length, modbus_function_cb pf_modbus_function)
{
    int offset = 0;
    int slave = 1;
    int function = 0;
    int ret = 0;
    // uint16_t address = 0;
    uint8_t rsp[MODBUS_MAX_MESSAGE_LENGTH] = {0};
    int rsp_length = 0;
    sft_t sft;

    modbus_t *ctx = NULL;

    if (req == NULL)
    {
        return -1;
    }

    if (index >= MODBUS_MAX)
    {
        return -1;
    }

    ctx = gp_modbus_ctx[index];    

    offset = ctx->backend->header_length;
    slave = req[offset - 1];
    function = req[offset];
    // address = (req[offset + 1] << 8) + req[offset + 2];

    sft.slave = slave;
    sft.function = function;
    sft.t_id = ctx->backend->prepare_response_tid(req, &req_length);   

    if (pf_modbus_function == NULL)
    {
        rsp_length = modbus_reply_exception( ctx, req, MODBUS_EXCEPTION_NOT_DEFINED);
    }
    else
    {
        rsp_length = ctx->backend->build_response_basis(&sft, rsp);
        ret = pf_modbus_function(function, &req[offset + 1], &rsp[offset + 1], MODBUS_MAX_MESSAGE_LENGTH);
        if (ret > 0)
        {
            rsp_length += ret;
        }
        else
        {
            rsp_length = modbus_reply_exception( ctx, req, MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE); 
        }
    }

    /* Suppress any responses when the request was a broadcast */
    return (ctx->backend->backend_type == _MODBUS_BACKEND_TYPE_RTU &&
            slave == MODBUS_BROADCAST_ADDRESS) ? 0 : modbus_send_raw_request(ctx, rsp, rsp_length);
}

/**
 * @brief   读取保持寄存器值
 * @param   [in] index modbus下标，0~6
 * @param   [in] addr 起始寄存器地址
 * @param   [in] nb 寄存器数量
 * @param   [out] dest 读取的数据的指针
 * @return  返回执行结果：-1异常
 */
int32_t sdk_modbus_registers_read(uint32_t index, int32_t addr, int32_t nb, uint16_t *dest)
{
    modbus_t *ctx = NULL;

    if (dest == NULL)
    {
        return -1;
    }

    if (index >= MODBUS_MAX)
    {
        return -1;
    }

    ctx = gp_modbus_ctx[index];

    return modbus_read_registers(ctx, addr, nb, dest);
}

/**
 * @brief   写单个保持寄存器
 * @param   [in] index modbus下标，0~6
 * @param   [in] addr 寄存器地址
 * @param   [in] value 写入值
 * @return  返回执行结果：-1异常
 */
int32_t sdk_modbus_register_write(uint32_t index, int32_t addr, const uint16_t value)
{
    modbus_t *ctx = NULL;
    if (index >= MODBUS_MAX)
    {
        return -1;
    }

    ctx = gp_modbus_ctx[index];

    return modbus_write_register(ctx, addr, value);
}

/**
 * @brief   写多个保持寄存器
 * @param   [in] index modbus下标，0~6
 * @param   [in] addr 寄存器起始地址
 * @param   [in] nb 寄存器数量
 * @param   [in] data 写入值的指针
 * @return  返回执行结果：-1异常
 */
int32_t sdk_modbus_registers_write(uint32_t index, int32_t addr, int32_t nb, const uint16_t *data)
{
    modbus_t *ctx = NULL;
    if (index >= MODBUS_MAX)
    {
        return -1;
    }

    ctx = gp_modbus_ctx[index];

    return modbus_write_registers(ctx, addr, nb, data);
}

/**
 * @brief   modbus 接收数据
 * @param   [in] index modbus下标，0~9
 * @param   [in] req   接收数据
 * @note    modbus作为主机的接收时使用
 * @return  返回执行结果：>0 接收数据长度；-1：异常
 */
int32_t sdk_modbus_receive_confirmation(uint32_t index, uint8_t *req)
{
    int32_t rc = 0;
    modbus_t *ctx = NULL;

    if (req == NULL)
    {
        return -1;
    }

    if (index >= MODBUS_MAX)
    {
        return -1;
    }

    ctx = gp_modbus_ctx[index];
    rc = modbus_receive_confirmation(ctx, req);

    return (rc);    
}

/**
 * @brief   用户将将要写的数据全部打包
 * @param   [in] index modbus下标，0~6
 * @param   [in] data 需要发送的ADSU
 * @param   [in] data_length 需要发送的ADSU长度
 * @note    user需要将一整包数据包含 从机地址、功能码等数据全部打包，底层只是将数据按rtu和tcp组包后发送
 * @return  返回执行结果：-1异常
 */
int32_t sdk_modbus_write(uint32_t index, const uint8_t *data, uint8_t data_length)
{
    modbus_t *ctx = NULL;
    if (index >= MODBUS_MAX)
    {
        return -1;
    }

    ctx = gp_modbus_ctx[index];

    return modbus_send_raw_request(ctx, data, data_length);

}

