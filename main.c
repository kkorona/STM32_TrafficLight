#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_tim.h"
#include "stm32f10x.h"
#include "misc.h"

int offset = 0;
int time = 0;

uint16_t BTN_PIN_NUMBER = GPIO_Pin_11;
uint16_t LED_PIN_NUMBER[3] = {GPIO_Pin_3, GPIO_Pin_4, GPIO_Pin_6};

uint32_t BTN_PORT = GPIOD_BASE;
uint32_t LED_PORT[3] = {GPIOE_BASE, GPIOE_BASE, GPIOE_BASE};

char DFPlayer_Cmd[10] = {0x7E, 0xFF, 0x06, 0, 0, 0, 0, 0, 0, 0xEF};

int led_state = 0;

const int TIME_PERIOD = 9;
const int TIME_GO = 4;
const int TIME_WAIT = 5;

int warningFlag = 0;
int timeFlag = 0;
void delay(int x) {
	int i=0;
	for(;i<x;i++);
}

void RCC_configure(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO | RCC_APB2Periph_USART1, ENABLE);
}


void USART_configure(void) {
	USART_InitTypeDef USART3_Init;
	NVIC_InitTypeDef NVIC1_InitStruct;

	// Initialize USART1
	USART3_Init.USART_BaudRate = 9600;
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
	USART_Cmd(USART3, ENABLE);
}


void TIM2_configure() {
  NVIC_InitTypeDef NVIC_InitStructure; // for interreupt
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; // timerbase...

  /* TIM2 Clock Enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  /* Enable TIM2 Global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* TIM2 Initialize */
  TIM_TimeBaseStructure.TIM_Period = 1200-1;
  TIM_TimeBaseStructure.TIM_Prescaler = 60000-1;
  //°è»ê¹æ¹ý : 1/72mhz * 1200 * 60000
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  /* TIM2 Enale */
  TIM_ARRPreloadConfig(TIM2, ENABLE);
  TIM_Cmd(TIM2, ENABLE);
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); // interrupt enable
}

void GPIO_configure(void) {
    GPIO_InitTypeDef GPIOx;

    /*
     * Pin IN
     * Button input : PE0
     * USART RX : PB11
     */

    GPIOx.GPIO_Mode = GPIO_Mode_IPU;
    GPIOx.GPIO_Speed = GPIO_Speed_50MHz;
    GPIOx.GPIO_Pin  = GPIO_Pin_11;
    GPIO_Init(GPIOD, &GPIOx);
    GPIOx.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIOx.GPIO_Pin = GPIO_Pin_11;
    GPIO_Init(GPIOB, &GPIOx);


    /*
     *  Pin Out
     *  LED : PE3 PE4 PE8
     *  USART TX : PB10
     *
     */
    GPIOx.GPIO_Mode = GPIO_Mode_Out_PP;
   	GPIOx.GPIO_Pin = (GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_6);
	GPIO_Init(GPIOE, &GPIOx);
	GPIOx.GPIO_Pin = (GPIO_Pin_2 | GPIO_Pin_3);
	GPIO_Init(GPIOD, &GPIOx);
	GPIOx.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIOx.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOB, &GPIOx);

}

void EXTI11_configure(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource11);


  EXTI_InitStructure.EXTI_Line = EXTI_Line11;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  EXTI_Init(&EXTI_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
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

void send_alert(int _file_number) {
	
	/*
	DFPlayer_Cmd[3] = (char)0x03;
	DFPlayer_Cmd[4] = (char)0x00;
	DFPlayer_Cmd[5] = (char)( 0x00 + file_number / 256 );
	DFPlayer_Cmd[6] = (char)( 0x00 + file_number % 256 );
	DFPlayer_Cmd[7] = (char)0xFE;
	DFPlayer_Cmd[8] = (char)0xF4;
	SendStr(USART3,DFPlayer_Cmd,10);
	*/
}

void led_on(int time) {
	int prev_state = 0;

	prev_state = (led_state + 2)%3;

	//handling previous state
	((GPIO_TypeDef *)LED_PORT[prev_state])->BSRR = 0;
	((GPIO_TypeDef *)LED_PORT[prev_state])->BRR |= LED_PIN_NUMBER[prev_state];
	delay(10);

	//handling current state
	((GPIO_TypeDef *)LED_PORT[led_state])->BSRR |= LED_PIN_NUMBER[led_state];
}

void TIM2_IRQHandler(void) {
  time++;
  time%=TIME_PERIOD;
  if(led_state == 2 && time <= TIME_GO ) {
		led_state = 0; timeFlag = 1;
	}
	if(led_state == 0 && time > TIME_GO && time <= TIME_WAIT) {
		led_state = 1; timeFlag = 1;
	}
	if(led_state == 1 && time > TIME_WAIT && time <= TIME_PERIOD){
		led_state = 2; timeFlag = 1;
	}
  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
  //Clears the TIMx's interrupt pending bits.
}

void EXTI15_10_IRQHandler(void) {
	//play_music(1);
	if (EXTI_GetITStatus(EXTI_Line11) != RESET) {
		if((~((GPIO_TypeDef *)BTN_PORT)->IDR) & (BTN_PIN_NUMBER)) {
			GPIO_SetBits(GPIOD,GPIO_Pin_3);
			delay(10000);
			GPIO_ResetBits(GPIOD,GPIO_Pin_3);
		}
		else {
			//if(led_state == 2) {
				GPIO_SetBits(GPIOD,GPIO_Pin_2);
				delay(10000);
				GPIO_ResetBits(GPIOD,GPIO_Pin_2);
			//}
		}
	    EXTI_ClearITPendingBit(EXTI_Line11);
	}
}

int main(void) {
	int i=0;
	SystemInit();
	RCC_configure();
	GPIO_configure();
	EXTI11_configure();
	TIM2_configure();
	USART_configure();
	for(i=0; i<3; i++) {
		if(LED_PORT[i] != 0)
			(((GPIO_TypeDef *)LED_PORT[i])->BRR) |= LED_PIN_NUMBER[i];
		delay(10);
		(((GPIO_TypeDef *)LED_PORT[i])->BRR) = 0;
	}

	while(1) {

		if(timeFlag) {
			led_on(time);
			timeFlag = 0;
		}
		
		if(warningFlag) {
			warningFlag = 0;
			//send_alert(1);
		}

	}

	return 0;
}
