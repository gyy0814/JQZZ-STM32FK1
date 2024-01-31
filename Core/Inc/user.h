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
//#include "stdbool.h"

typedef struct {
    uint8_t num;
    uint8_t Data;
} GameMessage;
void GameInit(void);

#define TO_BIT(num) (1<<(num%24))


#define 入口门锁 0
#define 保险箱 1
#define 电视信号 2
#define 锁内箱 3
#define 锁电视左箱 4
#define 锁右箱 5
#define 锁楼梯右上 6
#define 锁一层梯下四箱左上 7
#define 锁一层梯下四箱右下 8
#define 左下 9
#define 关锁 10
#define 关锁 11
#define 关锁 12
#define 关锁 13
#define 茶室门锁 14
#define 扫雷门锁 15





#define xx屋灯 28
#define LIGHT_B 29
#define LIGHT_C 30
#define LIGHT_D 31
#define LIGHT_E 32

#define KEY_A 23
#define KEY_B 24
#define KEY_C 23
#define KEY_D 23
#define KEY_E 23
#endif //FKS1_USER_H