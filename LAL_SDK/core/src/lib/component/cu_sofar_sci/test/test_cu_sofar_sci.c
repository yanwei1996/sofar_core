
//arm-linux-gnueabihf-gcc test_sofar_sci.c ../cu_sofar_sci.c -o test_sofar_sci -L../../../ -l_lal -I../../../../include/sdk -I../../../../include/lal -I../../../../include



#include "core.h"
#include "cu_sofar_sci.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define log_i(...)		printf(__VA_ARGS__)	    //fixme 后面easylog可以用后，屏蔽掉  



// //DSP communication addr
// #define DSP_ADDR_MASTER						0x02

// //DSP communication function id
// #define	DSP_FUNCID_HANDSHAKED					0x1000		/*上电握手*/	

// //各命令帧数据段字节长度
// #define DSP_CMD_DATA_LEN_HANDSHAKED				8		//握手命令，数据段字节数





// int32_t dsp485comm_subtask_mdsp_handshake_test(uint32_t port)
// {
// 	cu_sofar_sci_t	sofar_sci;
// 	uint8_t send_data_buf[256];		//256
// 	uint8_t recv_data_buf[256];
// 	int32_t ret;
	
// 	//1、应用层给para_buf赋值，填充主机发送的数据段参数
	
// 	//2、填充sofar_sci
// 	sofar_sci.dev_addr = DSP_ADDR_MASTER;
// 	sofar_sci.function_id = DSP_FUNCID_HANDSHAKED;
// 	sofar_sci.p_send_data = send_data_buf;	
// 	sofar_sci.send_data_len = DSP_CMD_DATA_LEN_HANDSHAKED;
// 	sofar_sci.p_recv_data = recv_data_buf;	
// 	sofar_sci.recv_data_len = DSP_CMD_DATA_LEN_HANDSHAKED;	
// 	// sofar_sci.recv_data_deal = onparse_handledata_masterdsp；	//注册回调函数———— 不采用回调的方法 各有利、弊

	
// 	//3、调用通讯交互接口函数
// 	ret = cu_sofar_sci_interactive(port, &sofar_sci, 500);
	
// 	//4、rec=ok, 则应用层处理数据段数据recv_data_buf

//     log_i("dsp485comm_subtask_mdsp_handshake_test -----ret = %d, \r\n", ret);
// }

int main(int argc, char* argv[])
{
    // sdk_uart_conf_t uart_conf;
    // uint32_t port;

    // port = atoi(argv[1]);

    // uart_conf.baud_rate = 115200;
    // uart_conf.data_bits = 8;
    // uart_conf.stop_bit = 1;   
    // uart_conf.parity = 0;   
    // uart_conf.flow_ctrl = 0;  

    // cu_sofar_sci_init(port, &uart_conf);
    
    // dsp485comm_subtask_mdsp_handshake_test(port);
}
