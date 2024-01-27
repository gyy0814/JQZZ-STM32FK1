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

#define 关门检测 0
#define 语音识别1 1
#define 语音识别2 2

#define 门锁 2
#define 语音识别电源 3
#define 文房四宝柜 4
#define 钥匙掉下 7

#endif //FKS1_USER_H