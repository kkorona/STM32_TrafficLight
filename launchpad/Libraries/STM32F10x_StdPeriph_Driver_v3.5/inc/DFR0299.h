/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DFR0299_H
#define __DFR0299_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Defines -------------------------------------------------------------------*/
#define DFPLAYER_EQ_NORMAL  0
#define DFPLAYER_EQ_POP     1
#define DFPLAYER_EQ_ROCK    2
#define DFPLAYER_EQ_JAZZ    3
#define DFPLAYER_EQ_CLASSIC 4
#define DFPLAYER_EQ_BASS    5

#define DFPLAYER_MODE_REPEAT 0
#define DFPLAYER_MODE_FOLDER 1
#define DFPLAYER_MODE_SINGLE 2
#define DFPLAYER_MODE_RANDOM 3
   
#define DFPLAYER_REPEAT_STOP  0
#define DFPLAYER_REPEAT_START 1
   
#define DFPLAYER_DEVICE_U_DISK 0
#define DFPLAYER_DEVICE_SD     1
#define DFPLAYER_DEVICE_AUX    2
#define DFPLAYER_DEVICE_SLEEP  3
#define DFPLAYER_DEVICE_FLASH  4

#define DFPLAYER_RECEIVED_LENGTH 10
#define DFPLAYER_SEND_LENGTH     10

#define TimeOut              0
#define WrongStack           1
#define DFPlayerCardInserted 2
#define DFPlayerCardRemoved  3
#define DFPlayerCardOnline   4
#define DFPlayerPlayFinished 5
#define DFPlayerError        6

#define Busy             1
#define Sleeping         2
#define SerialWrongStack 3
#define ChecksumNotMatch 4
#define FileIndexOut     5
#define FileMismatch     6
#define Advertise        7

#define Stack_Header     0
#define Stack_Version    1
#define Stack_Length     2
#define Stack_Command    3
#define Stack_ACK        4
#define Stack_Parameter  5
#define Stack_Checksum   7
#define Stack_End        9

#define DFPlayer_Next            0x01
#define DFPlayer_Previous        0x02
#define DFPlayer_Tracking        0x03
#define DFPlayer_IncreaseVolume  0x04
#define DFPlayer_DecreaseVolume  0x05
#define DFPlayer_SpecifyVolume   0x06
#define DFPlayer_EQ              0x07
#define DFPlayer_PlaybackMode    0x08
#define DFPlayer_PlaybackSource  0x09
#define DFPlayer_EnterStandby    0x0A
#define DFPlayer_NormalWorking   0x0B
#define DFPlayer_ResetModule     0x0C
#define DFPlayer_Playback        0x0D
#define DFPlayer_Pause           0x0E
#define DFPlayer_FolderPlayback  0x0F
#define DFPlayer_VolumeAdjustSet 0x10
#define DFPlayer_RepeatPlay      0x11



/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "stm32f10x_conf.h"
/* Variables -----------------------------------------------------------------*/
uint8_t send_buf[DFPLAYER_SEND_LENGTH]
                = {0x7E, 0xFF, 0x06, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0xEF};
uint8_t recv_buf[DFPLAYER_RECEIVED_LENGTH];

// 7E FF 06 0F 00 01 01 xx xx EF
// 0	->	7E is start code
// 1	->	FF is version
// 2	->	06 is length
// 3	->	0F is command
// 4	->	00 is no receive
// 5~6	->	01 01 is argument
// 7~8	->	checksum = 0 - ( FF+06+0F+00+01+01 )
// 9	->	EF is end code

USART_TypeDef* DFPlayer_USARTx;

unsigned long _timeOutTimer;
unsigned long _timeOutDuration = 500;

uint8_t recevedIndex = 0;
uint8_t device = DFPLAYER_DEVICE_SD;
uint8_t handleType;
uint8_t handleCommand;
uint16_t hadleParameter;
bool isAvailable = false;
bool isSending = false;

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void DFPlayer_Uint16ToArray(uint16_t value, uint8_t *array);
uint16_t DFPlayer_ArrayToUint16(uint8_t *array);
uint16_t DFPlayer_GetChecksum (uint8_t *buf);
void DFPlayer_ResetBuffer();
void DFPlayer_SendCmd(uint8_t cmd, uint8_t high_arg, uint8_t low_arg);
void DFPlayer_SendCmd(uint8_t cmd, uint16_t arg);
void DFPlayer_SendCmd(uint8_t cmd);
void DFPlayer_SendCmd();
void DFPlayer_enableACK();
void DFPlayer_disableACK();
void DFPlayer_init(USART_TypeDef* USARTx, bool isACK = true);
void DFPlayer_parseBuffer();
bool DFPlayer_validateBuffer();
bool DFPlayer_handleMessage(uint8_t type, uint16_t parameter = 0);
void DFPlayer_handleError(uint8_t type, uint16_t parameter = 0);
uint8_t DFPlayer_readCommand();
bool DFPlayer_waitAvailable();
bool DFPlayer_available();
uint8_t DFPlayer_readType();
uint16_t DFPlayer_read();

void DFPlayer_USART_Init();

#ifdef __cplusplus
}
#endif

#endif /* __DFR0299_H */