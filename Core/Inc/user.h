//
// Created by 26913 on 2023/12/14.
//

#ifndef FKS1_USER_H
#define FKS1_USER_H


#include "usart.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "main.h"
#include "cmsis_os.h"
#include "music.h"


typedef struct {
    uint8_t CMD;
    uint8_t Data[64];
    size_t DataLength;
} GameMessage;

#define TO_BIT(num) (1<<(num%32))

//灯光
#define 场灯(num) (num+25)
#define 大厅灯光 19
#define 副厅灯光 5


//输入
#define 钢琴输入 5
#define 戒指输入 17
#define 花砖检测 10
#define 插香输入 11
#define 抽屉打开输入 12
#define 毒药检测 13
#define 敲门输入 14

#define 电闸输入 NULL

//输出
#define 煤气灯输出 10
#define 抽屉锁 11
#define 水幕开 12
#define 水幕关 13
#define 视频播放 14
#define 地窖门控制 61
#define 地窖门电源 62
#define 留声机音乐1 15
#define 留声机音乐2 16
#define 留声机音乐3 17

#define 白骨射灯 49
#define 爆闪灯 NULL


#endif //FKS1_USER_H