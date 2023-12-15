//
// Created by 26913 on 2023/12/13.
//

#ifndef FKS1_MUSIC_H
#define FKS1_MUSIC_H

#endif //FKS1_MUSIC_H

#include "usart.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "main.h"
#include "cmsis_os.h"

#define MUSIC_1 huart2
#define MUSIC_2 huart3


typedef struct {
    UART_HandleTypeDef *huart;
    uint8_t CMD;
    uint8_t Data[32];
    size_t DataLength;
} MusicMessage;

void MUSIC_PLAY(UART_HandleTypeDef *huart, uint8_t *FileName, size_t FileNameLength);

#define 查询播放状态 0x00
#define U盘 0x00
#define SD卡 0x01
#define Flash 0x02

#define 播放 0x01

#define 暂停 0x02

#define 暂停 0x02


// 播放时间控制
#define PLAY_TIME_CONTROL_CMD 0x05

// 音量控制
#define VOLUME_CONTROL_CMD 0x06

// 复读控制
#define REPEAT_CONTROL_CMD 0x08

// 插播控制
#define INTERJECT_CONTROL_CMD 0x09

// 循环模式控制
#define LOOP_MODE_CONTROL_CMD 0x0B

// 组合播放指令
#define COMBINED_PLAY_CMD 0x0C

// 循环次数指令
#define LOOP_COUNT_CMD 0x0B

// EQ选择指令
#define EQ_SELECT_CMD 0x07
