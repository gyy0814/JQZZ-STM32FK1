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

#define NWaitBit(pin,clear) (xEventGroupWaitBits(InputEventGroup[pin/24],TO_BIT(pin),clear,pdTRUE,0) & TO_BIT(pin))==0
#define WaitBit(pin,clear) (xEventGroupWaitBits(InputEventGroup[pin/24],TO_BIT(pin),clear,pdTRUE,0) & TO_BIT(pin))==TO_BIT(pin)
#define delay(ms) if(GameDelay(&RunTime,ms)){gameFlag++;}
#define PlayMusicA(Name,Mode) {char FileName[]=Name;PlayMusicName(&MUSIC_1, FileName, strlen(FileName),Mode);}
#define PlayMusicB(Name,Mode) {char FileName[]=Name;PlayMusicName(&MUSIC_2, FileName, strlen(FileName),Mode);}
#define StopMusicA StopMusic(&MUSIC_1)
#define StopMusicB StopMusic(&MUSIC_2)
#define SetPin(pin) SetOutput(pin,GPIO_PIN_SET)
#define ResetPin(pin) SetOutput(pin,GPIO_PIN_RESET)


#define 动物摆放 0
#define 游戏过关 1
#define 酒吧过关 2
#define 医院过关 3
#define 记忆卡刷卡 4
#define 记忆卡2刷卡 5
#define 储存库过关 6
#define 密码器 7
#define 记忆卡拿开 8
#define 拉杆上 9
#define 紧急停止 10
#define 信拿走 11

//答案选择按钮
#define 答案1_A 13
#define 答案1_B 12
#define 答案1_C 15

#define 答案2_A 14
#define 答案2_B 17
#define 答案2_C 16

#define 答案3_A 19
#define 答案3_B 18
#define 答案3_C 21

#define 答案确认 20
////////////////

#define 敲门 22
#define 拉杆下 23
//？//？//？//？//



///////////////////
#define 蓝色聚光灯 0
#define 婴儿床灯 1
#define 游戏灯 2
#define 酒吧灯 3
#define 医院灯 4
#define 片子灯 5
// 消防门
#define 出口门 7
#define 出口灯 8

#define 实验室台灯 10
#define 实验室灯 11
#define 三柜锁 12
#define 柜子白光 13

#define 贝塔锁 15

#define 答案按钮灯 17
#define 急停射灯 18
#define 炉子灯带 19
#define 答案选择灯 20
#define 取消发射视频 21
#define 火箭升空视频 22
#define 进入飞船视频 23
#define 准备升空视频 24
#define 记忆卡2视频 25
#define 记忆卡视频 26
#define 电脑复位 27
#define 电脑开机 28
#define 成功发射视频 29

#define 炉子平移门关 48
#define 炉子平移门开 49
#define 出口平移门关 50
#define 出口平移门开 51
#define 入口平移门关 52
#define 入口平移门开 53

#define 窗户火箭视频 54
#define 窗户警察视频 55
#endif //FKS1_USER_H