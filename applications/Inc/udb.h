#ifndef __UDB_H_
#define __UDB_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "startup_main.h"
#include <cstdint>

/*接收数据数量*/
#define UDB_RX_BOOL_NUM 	2
#define UDB_RX_INT8_NUM 	0
#define UDB_RX_INT16_NUM 	4
#define UDB_RX_INT32_NUM 	0
#define UDB_RX_FP32_NUM 	0
#define UDB_RX_TOTAL_SIZE  (((UDB_RX_BOOL_NUM + 7) / 8) + \
                        (UDB_RX_INT8_NUM * 1) + \
                        (UDB_RX_INT16_NUM * 2) + \
                        (UDB_RX_INT32_NUM * 4) + \
                        (UDB_RX_FP32_NUM * 4))

/*发送数据数量*/
#define UDB_TX_BOOL_NUM      0
#define UDB_TX_INT8_NUM      0
#define UDB_TX_INT16_NUM     0
#define UDB_TX_INT32_NUM     0
#define UDB_TX_FP32_NUM      0
#define UDB_TX_TOTAL_SIZE    (((UDB_TX_BOOL_NUM + 7) / 8) + \
                            (UDB_TX_INT8_NUM * 1) + \
                            (UDB_TX_INT16_NUM * 2) + \
                            (UDB_TX_INT32_NUM * 4) + \
                            (UDB_TX_FP32_NUM * 4))


typedef struct
{
	int16_t ch[4];
	bool s[2];
}Udp_rc_t;


class UDB
{
	public:
	class RX
	{
		public:
		class DATA
		{
			public:
			uint8_t buffer[UDB_RX_TOTAL_SIZE + 3];
/****************************接受到的数据类型*******************************/
			bool bool_buffer[UDB_RX_BOOL_NUM];
			int8_t int8_buffer[UDB_RX_INT8_NUM];
			int16_t int16_buffer[UDB_RX_INT16_NUM];
			int32_t int32_buffer[UDB_RX_INT32_NUM];
			fp32 fp32_buffer[UDB_RX_FP32_NUM];
		}data;

		class APPLY
		{
			public:
			Udp_rc_t rc;
		}apply;

		bool Data_Analysis(uint8_t *msg_data,int16_t bool_num = UDB_RX_BOOL_NUM,int16_t int8_num = UDB_RX_INT8_NUM,int16_t int16_num = UDB_RX_INT16_NUM,int16_t int32_num = UDB_RX_INT32_NUM,int16_t fp32_num = UDB_RX_FP32_NUM,int16_t total_size = UDB_RX_TOTAL_SIZE);
		bool Buffer_Sep(void);
		bool Data_Apply(void);
	}rx;

	class TX
	{
		public:
		class DATA
		{
			public:
			uint8_t buffer[UDB_TX_TOTAL_SIZE + 3];
/****************************准备发送的数据类型*******************************/
			bool bool_buffer[UDB_TX_BOOL_NUM];
			int8_t int8_buffer[UDB_TX_INT8_NUM];
			int16_t int16_buffer[UDB_TX_INT16_NUM];
			int32_t int32_buffer[UDB_TX_INT32_NUM];
			fp32 fp32_buffer[UDB_TX_FP32_NUM];
		}data;

	}tx;

	class CHECKSUM
	{
		public:
		uint8_t __SUMCRC(uint8_t *puchMsg, uint16_t usDataLen);
	}checksum;

	class CONVERT
	{
		public:
		int16_t Bytes2Short(uint8_t DH,uint8_t DL);
		int32_t Bytes2Int(uint8_t DH, uint8_t D2, uint8_t D3, uint8_t DL);
		fp32 Bytes2Fp32(uint8_t DH, uint8_t D2, uint8_t D3, uint8_t DL);
	}convert;

};

extern UDB udb;

#ifdef __cplusplus
}
#endif
	
#endif
