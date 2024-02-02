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
//输出
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
#define 卧室柜上灯 12
#define 书房门锁 13
#define 摄像头灯 14
#define 书房灯带 15
#define 线路轨门锁 16
#define 快递柜门锁 17
#define 扫描仪门锁 18
#define 信息块1 19

#define 信息块2 20
#define 信息块3 21
#define 信息块4 22
#define 信息块5 23
#define 信息块6 24
#define 信息块7 25
#define 信息块8 26
#define 信息块9 27
#define 信息块10 28

#define 灯带 31
#define 线路射灯 32
#define 扫描灯1 34
#define 扫描灯2 36
#define 扫描灯3 35
#define 扫描灯4 37
#define 扫描灯5 40
#define 扫描灯6 38
#define 扫描灯7 39
#define 扫描灯8 42

#define 扫描灯9 41
#define 扫描灯10 33
#define 扫描激光 43
#define 电脑显示器 44
#define 书房月球灯 45
#define 电视背景灯带 46
#define 书房台灯 47
#define 信息投影灯 48
#define 卧室月球灯 49
#define 客厅房顶灯带 50
#define 卧室灯带 51
#define 前场电脑复位 52
#define 前场电脑触发 53
#define 电视信号7 54
#define 开场语音 55


//输入
#define 游戏开始 0
#define 遥控器1 1
#define 遥控器2 2
#define 遥控器3 3
#define 遥控器4 4
#define 遥控器5 5
#define 记忆卡刷卡1 6
#define 记忆卡刷卡2 7
#define 扫描仪1  9
#define 扫描仪2 11

#define 打电话 10
#define 扫描仪3 8
#define 扫描仪4 24
#define 卧室密码器 13
#define 录取通知 14
#define 记忆卡拿走 15
#define 敲门信号 16
#define 扫描仪6 17
#define 书房数独 18
#define 卧室抽屉打开 19

#define 快递柜密码器 20
#define 线路过关 21
#define 艾利刷卡 22
#define 扫描仪5 25
#define 扫描仪6 17
#define 扫描仪7 31
#define 扫描仪8 28
#define 扫描仪9 27
#define 扫描仪10 12
#define 矮桌 29
#define 开关面板 30
#define 艾琳开始 31
#endif //FKS1_USER_H