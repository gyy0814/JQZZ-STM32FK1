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

#define TO_BIT(num) (1<<(num%32))

#define 门把手 0
#define 手机按钮 1
#define 行程管理_唤醒 2
#define 行程管理_1 3
#define 行程管理_2 4
#define 行程管理_3 5
#define 行程管理_4 6
#define 对讲_播放 7
#define 对接_重播 8
#define 快递检测 9
#define 温度控制_1 3
#define 温度控制_2 4
#define 温度控制_3 5
#define 季节切换 4
#define 电箱密码 6
#define 电箱开 8
#define 电箱关 9

#define 入口门 2
#define 视频1 3
#define 行程管理电源 4
#define 门铃音效 7
#define 快递音效 9
#define 快递门 10
#define 外卖音效 11
#define 外卖柜门 12
#define 手机视频1 3
#define 温度控制电源 4
#define 窗户推杆 4
#define 冷风 4
#define 暖风 4
#define 蓝灯 4
#define 红灯 4
#define 视频2 4
#define 墙壁视频 4
#define 手机视频2 3
#define 电箱电源 4
#define 电箱射灯 4
#define 电箱锁 4
#define 出口门 4




#define WaitBit(pin,clear) if(xEventGroupWaitBits(InputEventGroup[pin/24],TO_BIT(pin),clear,pdTRUE,0) & TO_BIT(pin))
#define delay(ms) if(GameDelay(&RunTime,ms)){gameFlag++;}
#define PlayMusicA(Name,Mode) {char FileName[]=Name;PlayMusicName(&MUSIC_1, FileName, strlen(FileName),Mode);}
#define PlayMusicB(Name,Mode) {char FileName[]=Name;PlayMusicName(&MUSIC_2, FileName, strlen(FileName),Mode);}


#endif //FKS1_USER_H