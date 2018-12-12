/*
 * HNSO02M.c
 *
 *  Created on: 2017. 11. 26.
 *      Author: Lee Sangjin
 */
//Includes
#include "HNSO02M.h"
   
//Typedefs
//Defines

//Private Variables
USART_TypeDef*     HNSO02_USARTx;
USART_InitTypeDef* HNSO02_USARTx_Struct;
uint8_t DeviceType = 0x00;
uint8_t GID[4] = {0x00, };
uint8_t SID[3] = {0x00, };

uint8_t SendFrame[108] = {0x00, };
uint8_t RecvFrame[108] = {0x00, };
uint8_t RecvLen = 0;

/* CRC16 implementation acording to CCITT standards */
static const unsigned short crc16tab[256]= {
  0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
  0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
  0x1231,0x0210,0x3273,0x2252,0x52b5,0x4294,0x72f7,0x62d6,
  0x9339,0x8318,0xb37b,0xa35a,0xd3bd,0xc39c,0xf3ff,0xe3de,
  0x2462,0x3443,0x0420,0x1401,0x64e6,0x74c7,0x44a4,0x5485,
  0xa56a,0xb54b,0x8528,0x9509,0xe5ee,0xf5cf,0xc5ac,0xd58d,
  0x3653,0x2672,0x1611,0x0630,0x76d7,0x66f6,0x5695,0x46b4,
  0xb75b,0xa77a,0x9719,0x8738,0xf7df,0xe7fe,0xd79d,0xc7bc,
  0x48c4,0x58e5,0x6886,0x78a7,0x0840,0x1861,0x2802,0x3823,
  0xc9cc,0xd9ed,0xe98e,0xf9af,0x8948,0x9969,0xa90a,0xb92b,
  0x5af5,0x4ad4,0x7ab7,0x6a96,0x1a71,0x0a50,0x3a33,0x2a12,
  0xdbfd,0xcbdc,0xfbbf,0xeb9e,0x9b79,0x8b58,0xbb3b,0xab1a,
  0x6ca6,0x7c87,0x4ce4,0x5cc5,0x2c22,0x3c03,0x0c60,0x1c41,
  0xedae,0xfd8f,0xcdec,0xddcd,0xad2a,0xbd0b,0x8d68,0x9d49,
  0x7e97,0x6eb6,0x5ed5,0x4ef4,0x3e13,0x2e32,0x1e51,0x0e70,
  0xff9f,0xefbe,0xdfdd,0xcffc,0xbf1b,0xaf3a,0x9f59,0x8f78,
  0x9188,0x81a9,0xb1ca,0xa1eb,0xd10c,0xc12d,0xf14e,0xe16f,
  0x1080,0x00a1,0x30c2,0x20e3,0x5004,0x4025,0x7046,0x6067,
  0x83b9,0x9398,0xa3fb,0xb3da,0xc33d,0xd31c,0xe37f,0xf35e,
  0x02b1,0x1290,0x22f3,0x32d2,0x4235,0x5214,0x6277,0x7256,
  0xb5ea,0xa5cb,0x95a8,0x8589,0xf56e,0xe54f,0xd52c,0xc50d,
  0x34e2,0x24c3,0x14a0,0x0481,0x7466,0x6447,0x5424,0x4405,
  0xa7db,0xb7fa,0x8799,0x97b8,0xe75f,0xf77e,0xc71d,0xd73c,
  0x26d3,0x36f2,0x0691,0x16b0,0x6657,0x7676,0x4615,0x5634,
  0xd94c,0xc96d,0xf90e,0xe92f,0x99c8,0x89e9,0xb98a,0xa9ab,
  0x5844,0x4865,0x7806,0x6827,0x18c0,0x08e1,0x3882,0x28a3,
  0xcb7d,0xdb5c,0xeb3f,0xfb1e,0x8bf9,0x9bd8,0xabbb,0xbb9a,
  0x4a75,0x5a54,0x6a37,0x7a16,0x0af1,0x1ad0,0x2ab3,0x3a92,
  0xfd2e,0xed0f,0xdd6c,0xcd4d,0xbdaa,0xad8b,0x9de8,0x8dc9,
  0x7c26,0x6c07,0x5c64,0x4c45,0x3ca2,0x2c83,0x1ce0,0x0cc1,
  0xef1f,0xff3e,0xcf5d,0xdf7c,0xaf9b,0xbfba,0x8fd9,0x9ff8,
  0x6e17,0x7e36,0x4e55,0x5e74,0x2e93,0x3eb2,0x0ed1,0x1ef0
};

//Fuctions
uint16_t crc16_ccitt(const unsigned char* buf, int len){
  register int counter;
  register unsigned short crc = 0;
  for( counter = 0; counter < len; counter++)
    crc = (crc<<8) ^ crc16tab[((crc>>8) ^ buf[counter])&0x00FF];
  return crc;
}

void HNSO02_Init(USART_TypeDef* USARTx, USART_InitTypeDef* USART_InitStruct){
  HNSO02_USARTx = USARTx;
  HNSO02_USARTx_Struct = USART_InitStruct;
  USARTInit();
  HNSO02_SendCMD(HNSO02_CMD_SYSINFOREQ,(uint8_t*) 0x00, 0);
  //HNSO02_RespCMD();
  //HNSO02_DecodeFrame(RecvFrame);
}

void USARTInit(void){
  GPIO_InitTypeDef GPIO_InitStruct;
  NVIC_InitTypeDef NVIC_InitStruct;
  
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  //AFIO enable
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  
  USART_Init(HNSO02_USARTx, HNSO02_USARTx_Struct);

  if(HNSO02_USARTx == USART1){
    //USART1 enable
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    //GPIOA enable
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
      
    // GPIO of UART1_Init
    // Initialize GPIOA Pin9
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9; // UART1_TX
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    // Initialize GPIOA Pin10
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10; // UART1_RX
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    // Initialize USART1 IRQ
    NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
  }
  if(HNSO02_USARTx == USART2){
    //USART2 enable
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    //GPIOA enable
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    // GPIO of UART2_Init  
    // Initialize GPIOA Pin2
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2; // UART2_TX
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    // Initialize GPIOA Pin3
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3; // UART2_RX
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    // Initialize USART2 IRQ
    NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
  }
  if(HNSO02_USARTx == USART3){
    //USART3 enable
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    //GPIOB enable
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  
    // GPIO of UART3_Init
    // Initialize GPIOB Pin10
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10; // UART3_TX
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    // Initialize GPIOB Pin11
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11; // UART3_RX
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    // Initialize USART3 IRQ
    NVIC_InitStruct.NVIC_IRQChannel = USART3_IRQn;
  }
  NVIC_Init(&NVIC_InitStruct);
  USART_ITConfig(HNSO02_USARTx, USART_IT_RXNE, ENABLE);
  USART_Cmd(HNSO02_USARTx, ENABLE);
}

uint8_t HNSO02_SetLen(void){
  uint8_t FrameLen = 0;
  while(SendFrame[FrameLen] != Frame_ETX && FrameLen < MAX_FrameLen) FrameLen++;
  if(FrameLen == MAX_FrameLen && SendFrame[MAX_FrameLen - 1] != Frame_ETX) return 0xff;
  else return FrameLen + 1;
}

void HNSO02_SetAPP(const unsigned char* buf, int len){
  memcpy(&SendFrame[Frame_IDX_APP], buf, len);
  SendFrame[Frame_IDX_APP + len + 2] = Frame_ETX;
}

void HNSO02_SetMSG(const unsigned char* buf, int len){
  memcpy(&SendFrame[Frame_IDX_MSG], buf, len);
  SendFrame[Frame_IDX_MSG + len + 2] = Frame_ETX;
}

void HNSO02_SendAPP(const uint8_t* DNID, const uint8_t* buf, int len){
  uint16_t crc;
  HNSO02_SetAPP(buf, len);
  HNSO02_SetSNID();
  HNSO02_SetDNID(DNID);
  SendFrame[Frame_IDX_STX] = Frame_STX;
  SendFrame[Frame_IDX_LEN] = HNSO02_SetLen();
  SendFrame[Frame_IDX_ST] = ST_AppRF;
  crc = crc16_ccitt(SendFrame, Frame_IDX_APP + len);
  SendFrame[Frame_IDX_APP + len + 1] = (crc & 0xFF00) >> 8;
  SendFrame[Frame_IDX_APP + len] = (crc & 0xFF);
  HNSO02_Send();
}

void HNSO02_Send(void){
  int i = 0;
  while(SendFrame[i] != Frame_ETX){
    HNSO02_SendData(HNSO02_USARTx, SendFrame[i]);
    //printf("%d 0x%X\n", i, SendFrame[i]);
    //HNSO02_SendData(USART2, SendFrame[i]);
    i++;
  }
  HNSO02_SendData(HNSO02_USARTx, Frame_ETX);
  //printf("%d 0x%X\n", i, SendFrame[i]);
  //HNSO02_SendData(USART2, Frame_ETX);
}

void HNSO02_SendCMD(uint8_t CMD, uint8_t* DATA, int len){
  uint16_t crc;
  uint8_t msg[90] = {0x00, };
  msg[0] = CMD;
  msg[1] = 0x01;
  memcpy(&msg[8], DATA, len);
  HNSO02_SetMSG(msg, 8 + len);
  SendFrame[Frame_IDX_STX] = Frame_STX;
  SendFrame[Frame_IDX_LEN] = HNSO02_SetLen();
  SendFrame[Frame_IDX_ST] = ST_LocalControl;
  crc = crc16_ccitt(SendFrame, Frame_IDX_MSG + 8 + len);
  SendFrame[Frame_IDX_MSG + 8 + len + 1] = (crc & 0xFF00) >> 8;
  SendFrame[Frame_IDX_MSG + 8 + len] = (crc & 0xFF);
  //SendFrame[Frame_IDX_MSG + 8 + len + 1] = 0x34;
  //SendFrame[Frame_IDX_MSG + 8 + len] = 0x12;
  
  HNSO02_Send();
}

void HNSO02_SendData(USART_TypeDef* USARTx, char dat){
  USART_SendData(USARTx, dat);
  while (!USART_GetFlagStatus(USARTx, USART_FLAG_TXE));
}

uint8_t RecvData(USART_TypeDef* USARTx){
  while (!USART_GetFlagStatus(USARTx, USART_FLAG_RXNE));
  return USART_ReceiveData(USARTx);
}

void HNSO02_RespCMD(void){
  int i = 0;
  uint8_t buf;
  do{
    buf = RecvData(HNSO02_USARTx);
    RecvFrame[i] = buf;
  } while( buf != Frame_ETX);
}

void HNSO02_SetDNID(const uint8_t* DNID){
  SendFrame[Frame_IDX_DNID] = DNID[0];
  SendFrame[Frame_IDX_DNID + 1] = DNID[1];
  SendFrame[Frame_IDX_DNID + 2] = DNID[2];
}

void HNSO02_SetSNID(void){
  SendFrame[Frame_IDX_SNID] = SID[0];
  SendFrame[Frame_IDX_SNID + 1] = SID[1];
  SendFrame[Frame_IDX_SNID + 2] = SID[2];
}

void HNSO02_DecodeFrame(uint8_t* Frame){
  switch(Frame[Frame_IDX_ST]){
  case ST_LocalControl:
    HNSO02_DecodeCMD(Frame);
    break;
  case ST_AppRF:
    HNSO02_DecodeAPPRF(Frame);
    break;
  case ST_DimmerApp:
    break;
  case ST_RFControl:
    break;
  }
}

void HNSO02_DecodeCMD(uint8_t* Frame){
  uint8_t Len = Frame[Frame_IDX_LEN] - Frame_IDX_MSG - 3;
  uint8_t msg[90] = {0x00, };
  memcpy(msg, &Frame[Frame_IDX_MSG], Len);
  switch(msg[Frame_IDX_MSG]){
  case HNSO02_CMD_SETCMDACK:
    break;
  case HNSO02_CMD_SYSINFORESP:
    memcpy(SID, &msg[HNSO02_INFO_SID], 3);
    DeviceType = msg[HNSO02_INFO_DEV];
    memcpy(GID, &msg[HNSO02_INFO_GID], 4);
    break;
  case HNSO02_CMD_SIDListRESP: 
    break;
  }
}

void HNSO02_DecodeAPPRF(uint8_t* Frame){
  uint8_t Len = Frame[Frame_IDX_LEN] - Frame_IDX_APP - 3;
  uint8_t i = 0;
  printf("\n%02X%02X%02X>\n",Frame[Frame_IDX_SNID],Frame[Frame_IDX_SNID + 1],Frame[Frame_IDX_SNID + 2]);
  while(i < Len){
    printf("%c",Frame[Frame_IDX_APP + i]);
    i++;
  }
  printf("\n");
}

void CHKFrame(uint8_t chkch){
  switch (chkch){
  case 0x03:
    RecvFrame[RecvLen] = chkch;
    RecvLen++;
    HNSO02_DecodeFrame(RecvFrame);
    OhLoRa_Cmd();
    RecvLen = 0;
    memset(RecvFrame, 0x00, MAX_FrameLen);
    break;
  default:
    RecvFrame[RecvLen] = chkch;
    RecvLen++;
    if(RecvFrame[0] != 0x02) RecvLen = 0;
    break;
  }
}