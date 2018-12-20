/*
 * main.c
 *
 *  Created on: 2018. 12. 5.
 *      Author: Wed team 10
 */

// flash load "C:\Users\Team5\Desktop\term\launchpad\Debug\flashclear.axf"
// flash load "C:\Users\Team5\Desktop\term\launchpad\Debug\launchpad.axf"

#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x.h"
#include "misc.h"

const uint16_t ROW_PIN_NUMBER[4] = {GPIO_IDR_IDR3, GPIO_IDR_IDR5, GPIO_IDR_IDR14, GPIO_IDR_IDR0};
const uint16_t COLUMN_PIN_NUMBER[4] = {GPIO_IDR_IDR2, GPIO_IDR_IDR4, GPIO_IDR_IDR6, GPIO_IDR_IDR13};
int offset = 0;

const uint16_t RED_ROW_PIN_NUMBER[4] = {GPIO_Pin_0,GPIO_Pin_9,GPIO_Pin_4,GPIO_Pin_3};
const uint16_t BLUE_ROW_PIN_NUMBER[4] = {GPIO_Pin_1,GPIO_Pin_6,GPIO_Pin_5,GPIO_Pin_5};
const uint16_t GREEN_ROW_PIN_NUMBER[4] = {GPIO_Pin_8, GPIO_Pin_7, GPIO_Pin_7, GPIO_Pin_6};

uint32_t INPUT_ROW_PORT[4] = {GPIOE_BASE, GPIOE_BASE, GPIOC_BASE, GPIOC_BASE};
uint32_t INPUT_COLUMN_PORT[4] = {GPIOE_BASE, GPIOE_BASE, GPIOE_BASE, GPIOC_BASE};
uint32_t RED_PORT[4] = {GPIOE_BASE, GPIOB_BASE, GPIOB_BASE, GPIOB_BASE};
uint32_t BLUE_PORT[4] = {GPIOE_BASE, GPIOB_BASE, GPIOB_BASE, GPIOD_BASE};
uint32_t GREEN_PORT[4] = {GPIOB_BASE, GPIOB_BASE, GPIOD_BASE, GPIOD_BASE};

char DFPlayer_Cmd[10] = {0x7E, 0xFF, 0x06, 0, 0, 0, 0, 0, 0, 0xEF};

char red[4][4] = {0,};
char blue[4][4] = {0,};
char green[4][4] = {0,};


void RCC_configure(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO | RCC_APB2Periph_USART1, ENABLE);
}


void UART_configure(void) {
	USART_InitTypeDef USART3_Init;
	NVIC_InitTypeDef NVIC1_InitStruct;

	// Initialize USART1
	USART3_Init.USART_BaudRate = 115200;
	USART3_Init.USART_WordLength = USART_WordLength_8b;
	USART3_Init.USART_StopBits = USART_StopBits_1;
	USART3_Init.USART_Parity = USART_Parity_No;
	USART3_Init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART3_Init.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART3_Init);

	// Initialize USART1 IRQ
	  NVIC1_InitStruct.NVIC_IRQChannel = USART3_IRQn;
	  NVIC1_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	  NVIC1_InitStruct.NVIC_IRQChannelSubPriority = 0;
	  NVIC1_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	  NVIC_Init(&NVIC1_InitStruct);
}

void GPIO_configure(void) {
    GPIO_InitTypeDef GPIOx;

    /*
     * Pin IN
     * BreakPad input row / col : PE3 ~ PC0, PE2 ~ PC13
     * USART RX : PB11
     * Offset setting Button : PD11, PD12
     */
    GPIOx.GPIO_Mode = GPIO_Mode_IPU;
    GPIOx.GPIO_Speed = GPIO_Speed_50MHz;
    GPIOx.GPIO_Pin  = (GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6);
    GPIO_Init(GPIOA, &GPIOx);
    GPIOx.GPIO_Pin  = (GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
    GPIO_Init(GPIOC, &GPIOx);
    GPIOx.GPIO_Pin = (GPIO_Pin_11 | GPIO_Pin_12 |GPIO_Pin_2);
    GPIO_Init(GPIOD, &GPIOx);
    GPIOx.GPIO_Pin  = (GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6);
    GPIO_Init(GPIOE, &GPIOx);
    GPIOx.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIOx.GPIO_Pin = GPIO_Pin_11;
    GPIO_Init(GPIOB, &GPIOx);


    /*
     *  Pin Out
     *  LED RED :
     *  LED BLUE :
     *  LED GREEN :
     *  USART TX : PB10
     *
     */
    GPIOx.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIOx.GPIO_Pin = (GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
    GPIO_Init(GPIOA, &GPIOx);
    GPIOx.GPIO_Pin = (GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_14 | GPIO_Pin_15);
    GPIO_Init(GPIOB, &GPIOx);
    GPIOx.GPIO_Pin = (GPIO_Pin_2 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9);
	GPIO_Init(GPIOC, &GPIOx);
	GPIOx.GPIO_Pin = (GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
	GPIO_Init(GPIOD, &GPIOx);
	GPIOx.GPIO_Pin = (GPIO_Pin_0 | GPIO_Pin_1);
	GPIO_Init(GPIOE, &GPIOx);
	GPIOx.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIOx.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOB, &GPIOx);

}

void SendData(USART_TypeDef* USARTx, char dat){
  USART_SendData(USARTx, dat);
  while (!USART_GetFlagStatus(USARTx, USART_FLAG_TXE));
}

void SendStr(USART_TypeDef* USARTx, char* str, int len){
  int i = 0;
  for (i = 0; i < len; i++) {
    SendData(USARTx, str[i]);
  }
}

void play_music(int cur_row,int cur_col) {
	int file_number = offset * 16 + cur_row * 4 + cur_col;

	DFPlayer_Cmd[3] = (char)0x03;
	DFPlayer_Cmd[4] = (char)0x00;
	DFPlayer_Cmd[5] = (char)( 0x00 + file_number / 256 );
	DFPlayer_Cmd[6] = (char)( 0x00 + file_number % 256 );
	DFPlayer_Cmd[7] = (char)0xFE;
	DFPlayer_Cmd[8] = (char)0xF4;
	SendStr(USART1,DFPlayer_Cmd,10);

}

void set_port(uint32_t* port,const uint32_t *target) {
	int i=0;
	for(i=0; i<4; i++) port[i] = target[i];
}

void led_on(int cur_row, int cur_col, int color) {
	uint32_t cur_port;
	if(color == 0) {
		cur_port = RED_PORT[cur_row];
		(((GPIO_TypeDef *) cur_port)->BSRR) |= RED_ROW_PIN_NUMBER[cur_row];
	}
	else if(color == 1) {
		(((GPIO_TypeDef *) cur_port)->BSRR) |= BLUE_ROW_PIN_NUMBER[cur_row];
	}
	else {
		(((GPIO_TypeDef *) cur_port)->BSRR) |= GREEN_ROW_PIN_NUMBER[cur_row];
	}
}

int main(void) {
	int i=0, j=0;
	uint32_t row_port[4];
	uint32_t column_port[4];
	SystemInit();
	RCC_configure();
	GPIO_configure();
	while(1) {
		set_port(row_port,INPUT_ROW_PORT);
		set_port(column_port,INPUT_COLUMN_PORT);

		for(i=0; i<4; i++) {
			if(~(((GPIO_TypeDef *)row_port[i])->IDR) & ROW_PIN_NUMBER[i]) {
				for(j=0; j<4; j++) {
					if(~(((GPIO_TypeDef *)column_port[i])->IDR) & COLUMN_PIN_NUMBER[i]) {
						printf("led on");
						play_music(i,j);
						led_on(i,j,0);
					}
				}
			}
		}
		for(i=0;i<100000;i++);

	}

	return 0;
}
