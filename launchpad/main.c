/*
 * main.c
 *
 *  Created on: 2018. 12. 5.
 *      Author: Wed team 10
 */

#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"
// #include "stm32f10x_exti.h"
#include "stm32f10x.h"
#include "misc.h"

const uint16_t ROW_PIN_NUMBER[4] = {GPIO_IDR_IDR3, GPIO_IDR_IDR5, GPIO_IDR_IDR14, GPIO_IDR_IDR0};
const uint16_t COLUMN_PIN_NUMBER[4] = {GPIO_IDR_IDR2, GPIO_IDR_IDR4, GPIO_IDR_IDR6, GPIO_IDR_IDR13};
int offset = 0;

void RCC_configure(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
}

/*
void EXTI11_Config(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource2);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource3);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource4);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource5);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource6);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource0);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource1);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource2);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource3);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource13);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource15);

  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  EXTI_Init(&EXTI_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);


}
*/

void GPIO_configure(void) {
    GPIO_InitTypeDef GPIOx;

    GPIOx.GPIO_Mode = GPIO_Mode_IPD;
    GPIOx.GPIO_Speed = GPIO_Speed_50MHz;
    GPIOx.GPIO_Pin  = (GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6);
    GPIO_Init(GPIOA, &GPIOx);
    GPIOx.GPIO_Pin  = (GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
    GPIO_Init(GPIOC, &GPIOx);
    GPIOx.GPIO_Pin = (GPIO_Pin_2);
    GPIO_Init(GPIOD, &GPIOx);
    GPIOx.GPIO_Pin  = (GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6);
    GPIO_Init(GPIOE, &GPIOx);


    GPIOx.GPIO_Mode = GPIO_Mode_Out_PP;
    // LED Out
    GPIOx.GPIO_Pin = (GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
    GPIO_Init(GPIOA, &GPIOx);
    GPIOx.GPIO_Pin = (GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
    GPIO_Init(GPIOB, &GPIOx);
    GPIOx.GPIO_Pin = (GPIO_Pin_2 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9);
	GPIO_Init(GPIOC, &GPIOx);
	GPIOx.GPIO_Pin = (GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
	GPIO_Init(GPIOD, &GPIOx);
	GPIOx.GPIO_Pin = (GPIO_Pin_0 | GPIO_Pin_1);
	GPIO_Init(GPIOE, &GPIOx);
}

void play_music(int cur_row,int cur_col) {
	int file_number = offset * 16 + cur_row * 4 + cur_col;
	char data[8];
}

int main(void) {
	int i=0, j=0;
	uint32_t row_port[4];
	uint32_t column_port[4];
	SystemInit();
	RCC_configure();
	GPIO_configure();
	while(1) {
		row_port[0] = ~GPIOE->IDR;
		row_port[1] = ~GPIOE->IDR;
		row_port[2] = ~GPIOC->IDR;
		row_port[3] = ~GPIOC->IDR;
		column_port[0] = ~GPIOE->IDR;
		column_port[1] = ~GPIOE->IDR;
		column_port[2] = ~GPIOE->IDR;
		column_port[3] = ~GPIOC->IDR;

		for(i=0; i<4; i++) {
			if(row_port[i] & ROW_PIN_NUMBER[i]) {
				for(j=0; j<4; j++) {
					if(column_port[i] & COLUMN_PIN_NUMBER[i]) {
						play_music(i,j);
					}
				}
			}
		}

	}

	return 0;
}
