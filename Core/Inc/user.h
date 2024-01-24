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

#define 入口门锁 0
#define 电视信号1 1
#define 电视信号2 2
#define 电视信号3 3
#define 电视信号4 4
#define 电视信号5 5
#define 电视信号6 6
#define 接电话信号 7
#define 卧室门锁  8
#define 卧室柜锁 9
#define 敲门锁 10
#define 卧室柜下灯 11
#define 电视信号4 12
#define 电视信号5 13
#define 电视信号6 14
#define 接电话信号 15













#define 门锁 2
#define 语音识别电源 3
#define 文房四宝柜 4
#define 钥匙掉下 7

#endif //FKS1_USER_H