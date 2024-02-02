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
    uint8_t num;
    uint8_t Data;
} GameMessage;
void GameInit(void);

#define TO_BIT(num) (1<<(num%24))

#define 门把手 15
#define 快递检测 0
#define 手机按钮 1
#define 行程管理_3 2
#define 行程管理_唤醒 3
#define 行程管理_1 4
#define 行程管理_2 5
#define 电箱密码 6
#define 电箱开 7
#define 电箱关 8
#define 对讲_播放 9
#define 对接_重播 10
#define 行程管理_4 11
#define 温度控制_1 12
#define 温度控制_2 13
#define 温度控制_3 14
#define 季节切换 14

#define 入口门 0
#define 墙壁视频 1
#define 电箱锁 2
#define 出口门 3
#define 电箱射灯 4
#define 快递门 6
#define 外卖柜门 5
#define 红灯 7

#define 蓝灯 9

#define 电视视频 11
#define 视频1 12
#define 视频2 13
#define 温度控制电源 14
#define 行程管理电源 15
#define 门铃音效 16
#define 快递音效 17
#define 外卖音效 18

#define 冷风 21
#define 暖风 20
//#define 窗户推杆 22
#define 电箱电源 23
#define 墙壁视频2 24


#define 手机视频1 32
#define 手机视频2 33

#define 窗户开 47
#define 窗户关 46


#define WaitBit(pin,clear) (xEventGroupWaitBits(InputEventGroup[pin/24],TO_BIT(pin),clear,pdTRUE,0) & TO_BIT(pin))==TO_BIT(pin)
#define delay(ms) if(GameDelay(&RunTime,ms)){gameFlag++;}
#define PlayMusicA(Name,Mode) {char FileName[]=Name;PlayMusicName(&MUSIC_1, FileName, strlen(FileName),Mode);}
#define PlayMusicB(Name,Mode) {char FileName[]=Name;PlayMusicName(&MUSIC_2, FileName, strlen(FileName),Mode);}
#define StopMusicA StopMusic(&MUSIC_1)
#define StopMusicB StopMusic(&MUSIC_2)

#endif //FKS1_USER_H