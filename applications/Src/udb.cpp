#include "udb.h"
#include <cstdint>
#include <string.h> // For memcpy

//USART_Debuger
uint8_t rx_buffer[1];

//应用数据
UDB udb;

extern "C"
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART2)
	{
		udb.rx.Data_Analysis(rx_buffer);
	}
}


bool UDB::RX::Data_Apply(void)
{
/*  可使用的数据
	this->data.bool_buffer[i];
	this->data.int8_buffer[i];
	this->data.int16_buffer[i];
	this->data.int32_buffer[i];
	this->data.fp32_buffer[i];
*/

  this->apply.rc.s[0] = this->data.bool_buffer[0];
  this->apply.rc.s[1] = this->data.bool_buffer[1];

	this->apply.rc.ch[0] = this->data.int16_buffer[0];
	this->apply.rc.ch[1] = this->data.int16_buffer[1];
	this->apply.rc.ch[2] = this->data.int16_buffer[2];
	this->apply.rc.ch[3] = this->data.int16_buffer[3];

	return true;
}

uint8_t SUMCRC_FLAG;
uint8_t buffer[7];
int16_t rx_cnt;
uint8_t finded_flag;

bool UDB::RX::Data_Analysis(uint8_t *msg_data,int16_t bool_num,int16_t int8_num,int16_t int16_num,int16_t int32_num,int16_t fp32_num,int16_t total_size)
{
//	static uint8_t SUMCRC_FLAG;
//	static uint8_t buffer[7];
//	static int16_t rx_cnt;
//	static uint8_t finded_flag;
	if(*msg_data == 0xA5)
	{
		finded_flag = 1;
	}
	if(finded_flag == 1)
	{
		buffer[rx_cnt++] = *msg_data;
		if((rx_cnt == (total_size+3)))
		{
			finded_flag = 0;
			rx_cnt = 0;
			SUMCRC_FLAG = udb.checksum.__SUMCRC(buffer+1,total_size);
			if((buffer[total_size+2] == 0x5A)&&(SUMCRC_FLAG == buffer[total_size+1]))
			{
				this->Buffer_Sep();   //将数据分离
				this->Data_Apply();   //给于数据实际意义
				return true;
			}
		}
	}
	return false;
}

bool UDB::RX::Buffer_Sep(void)
{
	// 假设 msg_data 指向接收到的数据缓冲区的起始位置。
    uint8_t *ptr = this->data.buffer;

	// 解析 bool
    for (int i = 0; i < UDB_RX_BOOL_NUM; ++i) 
	{
        this->data.bool_buffer[i] = (*ptr & (1 << (i % 8))) != 0;
        if (i % 8 == 7) 
		{
            ++ptr;  // 每8个布尔值后移动到下一个字节
        }
    }

	// 解析 int8_t
    for (int i = 0; i < UDB_RX_INT8_NUM; ++i) 
	{
        this->data.int8_buffer[i] = *reinterpret_cast<int8_t*>(ptr);
        ptr += sizeof(int8_t);
    }
    
    // 解析 int16_t
    for (int i = 0; i < UDB_RX_INT16_NUM; ++i) 
	{
        uint8_t DH = *ptr++;
        uint8_t DL = *ptr++;
        this->data.int16_buffer[i] = udb.convert.Bytes2Short(DH, DL);
    }
    
    // 解析 int32_t
    for (int i = 0; i < UDB_RX_INT32_NUM; ++i) 
	{
        uint8_t DH = *ptr++;
        uint8_t D2 = *ptr++;
        uint8_t D3 = *ptr++;
        uint8_t DL = *ptr++;
        this->data.int32_buffer[i] = udb.convert.Bytes2Int(DH, D2, D3, DL);
    }
    
    // 解析 fp32
    for (int i = 0; i < UDB_RX_FP32_NUM; ++i) 
	{
        uint8_t DH = *ptr++;
        uint8_t D2 = *ptr++;
        uint8_t D3 = *ptr++;
        uint8_t DL = *ptr++;
        this->data.fp32_buffer[i] = udb.convert.Bytes2Fp32(DH, D2, D3, DL);
    }

	return true;
}


uint8_t UDB::CHECKSUM::__SUMCRC(uint8_t *puchMsg, uint16_t usDataLen)
{
    int16_t i = 0;
	uint8_t uchSUMCRC = 0x00;
    for (; i < usDataLen; i++)
    {
			uchSUMCRC += puchMsg[i];
    }
    return uchSUMCRC;
}

int16_t UDB::CONVERT::Bytes2Short(uint8_t DH,uint8_t DL)
{
	int16_t result = (int16_t)((int16_t)DH << 8 | DL);
	return result;
}

int32_t UDB::CONVERT::Bytes2Int(uint8_t DH, uint8_t D2, uint8_t D3, uint8_t DL)
{
    int32_t result = (int32_t)((int32_t)DH << 24 | (int32_t)D2 << 16 | (int32_t)D3 << 8 | DL);
    return result;
}

fp32 UDB::CONVERT::Bytes2Fp32(uint8_t DH, uint8_t D2, uint8_t D3, uint8_t DL)
{
    uint8_t bytes[4] = {DL, D3, D2, DH};
    fp32 result;
    memcpy(&result, bytes, sizeof(result));
    return result;
}



#ifndef __cplusplus   //如果在C++使用，则不启用
#include <stdio.h>
/******************************************************************************************/
/* 加入以下代码, 支持printf函数, 而不需要选择use MicroLIB */

#if 1
#if (__ARMCC_VERSION >= 6010050)                    /* 使用AC6编译器时 */
__asm(".global __use_no_semihosting\n\t");          /* 声明不使用半主机模式 */
__asm(".global __ARM_use_no_argv \n\t");            /* AC6下需要声明main函数为无参数格式，否则部分例程可能出现半主机模式 */

#else
/* 使用AC5编译器时, 要在这里定义__FILE 和 不使用半主机模式 */
#pragma import(__use_no_semihosting)

struct __FILE
{
    int handle;
    /* Whatever you require here. If the only file you are using is */
    /* standard output using printf() for debugging, no file handling */
    /* is required. */
};

#endif

/* 不使用半主机模式，至少需要重定义_ttywrch\_sys_exit\_sys_command_string函数,以同时兼容AC6和AC5模式 */
int _ttywrch(int ch)
{
    ch = ch;
    return ch;
}

/* 定义_sys_exit()以避免使用半主机模式 */
void _sys_exit(int x)
{
    x = x;
}

char *_sys_command_string(char *cmd, int len)
{
    return NULL;
}

/* FILE 在 stdio.h里面定义. */
FILE __stdout;

/* 重定义fputc函数, printf函数最终会通过调用fputc输出字符串到串口 */
int fputc(int ch, FILE *f)
{
    while ((USART1->SR & 0X40) == 0);               /* 等待上一个字符发送完成 */

    USART1->DR = (uint8_t)ch;                       /* 将要发送的字符 ch 写入到DR寄存器 */
    return ch;
}
#endif
/***********************************************END*******************************************/
#endif
