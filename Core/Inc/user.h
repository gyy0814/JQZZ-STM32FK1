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

#define WaitBit(pin,clear) (xEventGroupWaitBits(InputEventGroup[pin/24],TO_BIT(pin),clear,pdTRUE,0) & TO_BIT(pin))==TO_BIT(pin)
#define delay(ms) if(GameDelay(&RunTime,ms)){gameFlag++;}
#define PlayMusicA(Name,Mode) {char FileName[]=Name;PlayMusicName(&MUSIC_1, FileName, strlen(FileName),Mode);}
#define PlayMusicB(Name,Mode) {char FileName[]=Name;PlayMusicName(&MUSIC_2, FileName, strlen(FileName),Mode);}
#define StopMusicA StopMusic(&MUSIC_1)
#define StopMusicB StopMusic(&MUSIC_2)
#define SetPin(pin) SetOutput(pin,GPIO_PIN_SET)
#define ResetPin(pin) SetOutput(pin,GPIO_PIN_RESET)


#define 动物按钮_鸡 2
#define 动物按钮_猫 2
#define 动物按钮_狗 2
#define 动物按钮_蛙 2
#define 动物按钮_牛 2
#define 动物摆放 2
#define 游戏过关 3
#define 酒吧过关 3
#define 医院过关 3
#define 敲门 3
#define 医院过关 3
#define 储存库过关 3
#define 密码器 7
#define 记忆卡拿开 9


#define 入口平移门开 0
#define 入口平移门关 1
#define 蓝色聚光灯 4
#define 婴儿床灯 4
#define 游戏灯 4
#define 酒吧灯 4
#define 医院灯 4
#define 片子灯 4
#define 出口灯 4
#define 出口平移门开 0
#define 出口平移门关 1
#define 蓝色聚光灯2 4
#define 实验室灯 9
#define 准备升空视频 1
#define 实验室台灯 9
#define 进入飞船视频 3
#define 柜子白光 3
#define 贝塔锁 3


#endif //FKS1_USER_H