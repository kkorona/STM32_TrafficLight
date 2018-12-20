/*
 * HNSO02M.h
 *
 *  Created on: 2017. 11. 26.
 *      Author: Lee Sangjin
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HNSO02M_H
#define __HNSO02M_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Defines -------------------------------------------------------------------*/
#define MIN_APPLEN   0
#define MAX_APPLEN   90
#define MAX_FrameLen 108
   
#define Frame_STX 0x02
#define Frame_ETX 0x03
   
#define Frame_IDX_STX  0
#define Frame_IDX_LEN  1
#define Frame_IDX_ST   2
#define Frame_IDX_RSVD 3
#define Frame_IDX_MSG  9
#define Frame_IDX_DNID 9
#define Frame_IDX_SNID 12
#define Frame_IDX_APP  15
   
#define ST_LocalControl  0x0E
#define ST_AppRF         0xA0
#define ST_DimmerApp     0xC0
#define ST_RFControl     0xD0
   
#define HNSO02_CMD_SYSINFOREQ  0x42
#define HNSO02_CMD_SIDListINIT 0x43
#define HNSO02_CMD_SIDListADD  0x44
#define HNSO02_CMD_SIDListDEL  0x45
#define HNSO02_CMD_SIDListREQ  0x46
#define HNSO02_CMD_SYSINFOSET  0x47
#define HNSO02_CMD_SYSPARASET  0x48
   
#define HNSO02_CMD_SETCMDACK   0x61
#define HNSO02_CMD_SYSINFORESP 0x62
#define HNSO02_CMD_SIDListRESP 0x66
   
#define HNSO02_INFO_SID      8
#define HNSO02_INFO_DEV      11
#define HNSO02_INFO_GID      12
#define HNSO02_INFO_RFCH     16
#define HNSO02_INFO_LISTNUM  17
#define HNSO02_INFO_NETSTATE 35

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
   
/* Exported Variables --------------------------------------------------------*/
extern uint8_t SendFrame[108];
extern uint8_t RecvFrame[108];

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
uint16_t crc16_ccitt(const unsigned char* buf, int len);
void HNSO02_Init(USART_TypeDef* USARTx, USART_InitTypeDef* USART_InitStruct);
void USARTInit(void);
uint8_t HNSO02_SetLen(void);
void HNSO02_SetAPP(const unsigned char* buf, int len);
void HNSO02_SetMSG(const unsigned char* buf, int len);
void HNSO02_SendAPP(const uint8_t* DNID, const uint8_t* buf, int len);
void HNSO02_Send(void);
void HNSO02_SendCMD(uint8_t CMD, uint8_t* DATA, int len);
void HNSO02_SendData(USART_TypeDef* USARTx, char dat);
uint8_t RecvData(USART_TypeDef* USARTx);
void HNSO02_RespCMD(void);
void HNSO02_SetDNID(const uint8_t* DNID);
void HNSO02_SetSNID(void);
void HNSO02_DecodeFrame(uint8_t* Frame);
void HNSO02_DecodeCMD(uint8_t* Frame);
void HNSO02_DecodeAPPRF(uint8_t* Frame);
void CHKFrame(uint8_t chkch);

#ifdef __cplusplus
}
#endif

#endif /* __HNSO02M_H */