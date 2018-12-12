/*
* DFR0299.c
*
*  Created on: 2017. 10. 23.
*      Author: Lee Sangjin
*/

/* Includes ------------------------------------------------------------------*/
#include "DFR0299.H"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern uint8_t send_buf[10];
extern uint8_t recv_buf[10];

extern USART_TypeDef* DFPlayer_USARTx;

extern unsigned long timeOutTimer;
extern unsigned long timeOutDuration;

extern uint8_t receivedIndex;
extern uint8_t device;
extern uint8_t handleType;
extern uint8_t handleCommand;
extern uint16_t handleParameter;
extern bool isAvailable;
extern bool isSending;

unsigned long _last_call = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void DFPlayer_Uint16ToArray(uint16_t value, uint8_t *array){
  *array = (uint8_t)(value>>8);
  *(array + 1) = (uint8_t)(value);
}

uint16_t DFPlayer_ArrayToUint16(uint8_t *array){
  uint16_t value = *array;
  value <<=8;
  value += *(array+1);
  return value;
}

uint16_t DFPlayer_GetChecksum (uint8_t *buf){
  uint16_t checksum = 0;
  int i = 0;
  for(i = Stack_Version; i < Stack_Checksum; i++){
    checksum += buf[i];
  }
  
  return -checksum;
}

void DFPlayer_ResetBuffer(){
  int i = 0;
  for(; i < 10; i++){
    recv_buf[i] = 0;
  }
}

void DFPlayer_SendCmd(uint8_t cmd, uint8_t high_arg, uint8_t low_arg){
  uint16_t buffer = high_arg;
  buffer = buffer << 8;
  DFPlayer_SendCmd(cmd, buffer | low_arg);
}

void DFPlayer_SendCmd(uint8_t cmd, uint16_t arg){
  send_buf[Stack_Command] = cmd;
  DFPlayer_Uint16ToArray(arg, send_buf + Stack_Parameter);
  DFPlayer_Uint16ToArray(GetChecksum(send_buf), send_buf + Stack_Checksum);
  DFPlayer_SendCmd();
}

void DFPlayer_SendCmd(uint8_t cmd){
  DFPlayer_SendCmd(cmd, 0);
}

void DFPlayer_SendCmd(){
  int i;
  if(send_buf[Stack_ACK]){
    while(isSending){
      DFPlayer_available();
    }
  }
  else{
    Delay_ms(10);
  }
  printf("\nsending: ");
  for(i = 0; i < DFPLAYER_SEND_LENGTH; i++){
    printf("%x ",send_buf[i]);
  }
  printf("\n");
  
  SendStr(DFPlayer_USARTx,send_buf,DFPLAYER_SEND_LENGTH);
  timeOutTimer = millis();
  isSending = send_buf[Stack_ACK];
}

void DFPlayer_enableACK(){
  send_buf[Stack_ACK] = 0x01;
}

void DFPlayer_disableACK(){
  send_buf[Stack_ACK] = 0x00;
}

void DFPlayer_init(USART_TypeDef* USARTx, bool isACK){
  if (isACK){
    DFPlayer_enableACK();
  }
  else{
    DFPlayer_disableACK();
  }
  DFPlayer_USARTx = USARTx;
  timeOutDuration += 3000;
  DFPlayer_reset();
  DFPlayer_waitAvailable();
  timeOutDuration -= 3000;
  Delay_ms(200);
  return (DFPlayer_readType() == DFPlayerCardOnline) || !isACK);
}

void DFPlayer_parseBuffer(){
  handleCommand = *(recv_buf + Stack_Command);
  handleParameter = DFPlayer_ArrayToUint16(recv_buf + Stack_Parameter);
  
  switch (handleCommand) {
  case 0x3D:
    DFPlayer_handleMessage(DFPlayerPlayFinished, handleParameter);
    break;
  case 0x3F:
    if (handleParameter & 0x02) {
      DFPlayer_handleMessage(DFPlayerCardOnline, handleParameter);
    }
    break;
  case 0x3A:
    if (handleParameter & 0x02) {
      DFPlayer_handleMessage(DFPlayerCardInserted, handleParameter);
    }
    break;
  case 0x3B:
    if (handleParameter & 0x02) {
      DFPlayer_handleMessage(DFPlayerCardRemoved, handleParameter);
    }
    break;
  case 0x40:
    DFPlayer_handleMessage(DFPlayerError, handleParameter);
    break;
  case 0x41:
    isSending = false;
    break;
  case 0x3C:
  case 0x3E:
  case 0x42:
  case 0x43:
  case 0x44:
  case 0x45:
  case 0x46:
  case 0x47:
  case 0x48:
  case 0x49:
  case 0x4B:
  case 0x4C:
  case 0x4D:
  case 0x4E:
  case 0x4F:
    isAvailable = true;
    break;
  default:
    DFPlayer_handleError(WrongStack);
    break;
  }
}

bool DFPlayer_validateBuffer(){
  return DFPlayer_GetChecksum(recv_buf) == DFPlayer_ArrayToUint16(recv_buf + Stack_Checksum);
}

bool DFPlayer_handleMessage(uint8_t type, uint16_t parameter){
  receivedIndex = 0;
  handleType = type;
  handleParameter = parameter;
  isAvailable = true;
  return isAvailable;
}

void DFPlayer_handleError(uint8_t type, uint16_t parameter){
  DFPlayer_handleMessage(type, parameter);
  isSending = false;
}

uint8_t DFPlayer_readCommand(){
  isAvailable = false;
  return handleCommand;
}

bool DFPlayer_waitAvailable(){
  isSending = true;
  while (!DFPlayer_available());
  return handleType != TimeOut;
}

bool DFPlayer_available();

uint8_t DFPlayer_readType(){
  isAvailable = false;
  return handleType;
}

uint16_t DFPlayer_read(){
  isAvailable = false;
  return handleParameter;
}


void DFPlayer_USART_Init(){
  GPIO_InitTypeDef GPIOx_Init;
  USART_InitTypeDef USARTx_Init;
  NVIC_InitTypeDef NVICx_Init;

  GPIOx_Init.GPIO_Speed = GPIO_Speed_50MHz;
  
  USARTx_Init.USART_BaudRate = 9600;
  USARTx_Init.USART_WordLength = USART_WordLength_8b;
  USARTx_Init.USART_StopBits = USART_StopBits_1;
  USARTx_Init.USART_Parity = USART_Parity_No;
  USARTx_Init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USARTx_Init.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  NVICx_Init.NVIC_IRQChannelPreemptionPriority = 0;
  NVICx_Init.NVIC_IRQChannelSubPriority = 0;
  NVICx_Init.NVIC_IRQChannelCmd = ENABLE;
  
  //AFIO enable
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  
  switch(DFPlayer_USARTx){
  case USART1:
    // USART1 enable
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    
    // GPIOA enable
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    // GPIO of UART1_Init
    // Initialize GPIOA Pin9
    GPIOx_Init.GPIO_Pin = GPIO_Pin_9; // UART1_TX
    GPIOx_Init.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIOx_Init);

    // Initialize GPIOA Pin10
    GPIOx_Init.GPIO_Pin = GPIO_Pin_10; // UART1_RX
    GPIOx_Init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIOx_Init);
    
    // Initialize USART1
    USART_Init(USART1, &USARTx_Init);
    
    // Initialize USART1 IRQ
    NVICx_Init.NVIC_IRQChannel = USART1_IRQn;
    NVIC_Init(&NVICx_Init);
    
    // Enable USARTx Interrupt
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART1, ENABLE);
    
    break;
    
  case USART2:
    // USART2 enable
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    
    // GPIOA enable
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    // GPIO of UART2_Init
    // Initialize GPIOA Pin2
    GPIOx_Init.GPIO_Pin = GPIO_Pin_2; // UART2_TX
    GPIOx_Init.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIOx_Init);
    
    // Initialize GPIOA Pin3
    GPIOx_Init.GPIO_Pin = GPIO_Pin_3; // UART2_RX
    GPIOx_Init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIOx_Init);
    
    // Initialize USART2
    USART_Init(USART2, &USARTx_Init);
    
    // Initialize USART2 IRQ
    NVICx_Init.NVIC_IRQChannel = USART2_IRQn;
    NVIC_Init(&NVICx_Init);
    
    // Enable USARTx Interrupt
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART2, ENABLE);
    
    break;  
    
  case USART3:
    // USART3 enable
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
  
    // GPIOB enable
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    // GPIO of UART3_Init
    // Initialize GPIOB Pin10
    GPIOx_Init.GPIO_Pin = GPIO_Pin_10; // UART3_TX
    GPIOx_Init.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIOx_Init);
    
    // Initialize GPIOB Pin11
    GPIOx_Init.GPIO_Pin = GPIO_Pin_11; // UART3_RX
    GPIOx_Init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIOx_Init);
    
    // Initialize USART2
    USART_Init(USART3, &USARTx_Init);
    
    // Initialize USART2 IRQ
    NVICx_Init.NVIC_IRQChannel = USART3_IRQn;
    NVIC_Init(&NVICx_Init);
    
    // Enable USARTx Interrupt
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART3, ENABLE);
    
    break;  
  }
}