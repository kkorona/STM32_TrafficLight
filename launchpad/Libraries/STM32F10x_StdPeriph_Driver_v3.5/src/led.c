/*
 * led.c
 *
 *  Created on: 2016. 12. 01.
 *      Author: Lee Sangjin
 */
#include "led.h"

void LED_Init(int n){
  //GPIOC enable
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  
  GPIO_InitTypeDef GPIOC_Init;

  GPIOC_Init.GPIO_Speed = GPIO_Speed_10MHz;
  GPIOC_Init.GPIO_Mode = GPIO_Mode_Out_PP;

  if(n==3){
    //GPIO of LD3_Init
    GPIOC_Init.GPIO_Pin = GPIO_Pin_9; //LD3 Green LED
    GPIO_Init(GPIOC, &GPIOC_Init);
  } else if(n==4){
    //GPIO of LD4_Init
    GPIOC_Init.GPIO_Pin = GPIO_Pin_8; //LD4 Blue LED
    GPIO_Init(GPIOC, &GPIOC_Init);
  } else;
}

void LED_On(int n){
  if(n==3) GPIO_SetBits(GPIOC, GPIO_Pin_9);
  else if(n==4) GPIO_SetBits(GPIOC, GPIO_Pin_8);
  else;
}

void LED_Off(int n){
  if(n==3) GPIO_ResetBits(GPIOC, GPIO_Pin_9);
  else if(n==4) GPIO_ResetBits(GPIOC, GPIO_Pin_8);
  else;
}