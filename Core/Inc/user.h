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
//输入
#define 游戏开始 0
#define 路由器重启按钮 1
#define 密码器 2
#define 手办哈里 3
#define 手办路飞 4
#define 手办斯内普 5
#define 手办乔巴 6
#define 手办赫敏 7
#define 手办纽特 8
#define 手办初音 9
#define 手办索隆 10
#define 手办娜美 11
#define 杂志按钮 12
#define 开门杂志 13
#define 小说密码器 14
#define cd输入1 15
#define cd输入2 16
#define cd输入3 17
#define cd输入4 18
#define cd输入5 19
#define cd输入6 20
#define 门把手 21
#define cd输入 22
#define 三摆件过关 23
#define 小霸王 24
#define 时钟1 25
#define 时钟2 26
#define 时钟3 27
#define 时钟4 28
#define 电脑视频过关 29
#define 同学语音 30
#define 电脑微信 31

#define 小爱1 40
#define 小爱2 41
#define 小爱3 42
#define 小爱4 43
#define 小爱5 44
#define 小爱6 45
#define 小爱7 46
#define 小爱8 47


//输出
#define 入口门 0
#define 路由器柜门 1
#define 杂志柜门 2
#define 主光源 3
#define 哈利波特门 4
#define 手办刷卡灯 5
#define 三摆件门 6
#define 初音cd柜门 7
#define 漫画柜门 8
#define 日记柜门 9
#define 小说柜门 10
#define 海贼柜门 11
#define 游戏卡带柜 12
#define 电脑过关 13
#define 电脑复位 14
#define 卧室门锁 15
#define 电视电源 16
#endif //FKS1_USER_H