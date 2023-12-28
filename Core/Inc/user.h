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

#define TO_BIT(num) (1<<(num%32))

//灯光
#define 场灯(num) (num+25)
#define 大厅灯光 19


//输入
#define 钢琴输入 5
#define 插香输入 11
#define 抽屉打开输入 12
#define 敲门输入 14
#define 电闸输入 NULL

//输出
#define 煤气灯输出 10
#define 抽屉锁 11
#define 水幕开 12
#define 水幕关 13
#define 视频播放 14
#define 地窖门控制 15
#define 地窖门电源 16

#define 爆闪灯 NULL


#endif //FKS1_USER_H