//
// Created by 26913 on 2023/12/13.
//

#ifndef FKS1_MUSIC_H
#define FKS1_MUSIC_H


#include "stm32f1xx_hal.h"
#include "usart.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "main.h"
#include "cmsis_os.h"

#define MUSIC_1 huart2
#define MUSIC_2 huart3

#define MUSIC1_BUSY_EVENT (1<<0)
#define MUSIC2_BUSY_EVENT (1<<8)

typedef struct {
    UART_HandleTypeDef *huart;
    uint8_t CMD;
    uint8_t Data[64];
    size_t DataLength;
} MusicMessage;

void PlayMusicName(UART_HandleTypeDef *huart,const char* FileName,size_t FileNameSize, uint8_t PlayMode);

#define 单曲循环 0x01

#define 单曲停止 0x04


#endif //FKS1_MUSIC_H