/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Defines -------------------------------------------------------------------*/
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "stm32f10x_conf.h"
#include "led.h"
#include "HNSO02M.h"
   
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void RccInit(void);
void GpioInit(void);
void UartInit(void);
void EXTIInit(void);
void OhLoRa_Cmd();
void SendData(USART_TypeDef* USARTx, char dat);
void SendStr(USART_TypeDef* USARTx, char* str, int len);
void Delay_ms(volatile unsigned int nTime);
void ClearScreen(void);
void CHKCMD(uint8_t chkch);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
