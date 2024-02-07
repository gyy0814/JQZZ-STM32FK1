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

#define NWaitBit(pin,clear) (xEventGroupWaitBits(InputEventGroup[pin/24],TO_BIT(pin),clear,pdTRUE,0) & TO_BIT(pin))==0
#define WaitBit(pin,clear) (xEventGroupWaitBits(InputEventGroup[pin/24],TO_BIT(pin),clear,pdTRUE,0) & TO_BIT(pin))==TO_BIT(pin)
#define delay(ms) if(GameDelay(&RunTime,ms)){gameFlag++;}
#define PlayMusicA(Name,Mode) {char FileName[]=Name;PlayMusicName(&MUSIC_1, FileName, strlen(FileName),Mode);}
#define PlayMusicB(Name,Mode) {char FileName[]=Name;PlayMusicName(&MUSIC_2, FileName, strlen(FileName),Mode);}
#define StopMusicA StopMusic(&MUSIC_1)
#define StopMusicB StopMusic(&MUSIC_2)
#define SetPin(pin) SetOutput(pin,GPIO_PIN_SET)
#define ResetPin(pin) SetOutput(pin,GPIO_PIN_RESET)
#define OpenLock(pin) {SetOutput(pin,GPIO_PIN_SET);osDelay(500);SetOutput(pin,GPIO_PIN_RESET);}


#define 听诊器1 35
#define 听诊器2 33
#define 听诊器3 34
#define 听诊器4 32
#define 听诊器5 24
#define 听诊器6 25
#define 听诊器7 26
#define 听诊器8 27

#define 输入茶室1 30
#define 输入茶室2 31
#define 输入茶室3 28
#define 输入茶室4 29
#define 输入茶室5 20
#define 输入茶室6 21
#define 输入茶室7 22
#define 输入茶室8 23

#define 输入扫雷1 37
#define 输入扫雷2 41
#define 输入扫雷3 38
#define 输入扫雷4 45
#define 输入扫雷5 44
#define 输入扫雷6 42
#define 输入扫雷7 36
#define 输入扫雷8 43
#define 输入扫雷9 40
#define 输入扫雷10 39
#define 输入扫雷11 47
#define 输入扫雷12 46
#define 输入扫雷14 49
#define 输入扫雷15 50
#define 输入扫雷16 95
#define 输入扫雷17 51
#define 输入扫雷18 52
#define 输入扫雷19 54
#define 输入扫雷20 53
#define 输入扫雷21 56
#define 输入扫雷22 55
#define 输入扫雷23 57
#define 输入扫雷24 59
#define 输入扫雷25 58

#define 输入卧室1 78
#define 输入卧室2 81
#define 输入卧室3 80
#define 输入卧室4 79
#define 输入卧室5 82
#define 输入卧室6 83

#define 输入厕所1 87
#define 输入厕所2 88
#define 输入厕所3 84
#define 输入厕所4 100
#define 输入厕所5 86
#define 输入厕所6 85
#define 输入厕所7 77
#define 输入厕所8 76

#define 输入儿童房1 61
#define 输入儿童房2 60
#define 输入儿童房3 68
#define 输入儿童房4 67
#define 输入儿童房5 66
#define 输入儿童房6 65
#define 输入儿童房7 64
#define 输入儿童房8 63
#define 输入儿童房9 62

#define 输入走廊1 19
#define 输入走廊2 18
#define 输入走廊3 17
#define 输入走廊4 16

#define 输入楼梯1 8
#define 输入楼梯2 7
#define 输入楼梯3 10
#define 输入楼梯4 9

#define 输入大厅1 94
#define 输入大厅2 93

#define 输入电视1 5
#define 输入电视2 6

#define 输入开场 13


#define 输出茶室1 33
#define 输出茶室2 34
#define 输出茶室3 32
#define 输出茶室4 31
#define 输出茶室5 38
#define 输出茶室6 37
#define 输出茶室7 36
#define 输出茶室8 35

#define 输出扫雷1 29
#define 输出扫雷2 25
#define 输出扫雷3 28
#define 输出扫雷4 20
#define 输出扫雷5 21
#define 输出扫雷6 24
#define 输出扫雷7 30
#define 输出扫雷8 23
#define 输出扫雷9 26
#define 输出扫雷10 27
#define 输出扫雷11 18
#define 输出扫雷12 19
#define 输出扫雷14 111
#define 输出扫雷15 110
#define 输出扫雷16 22
#define 输出扫雷17 109
#define 输出扫雷18 108
#define 输出扫雷19 107
#define 输出扫雷20 105
#define 输出扫雷21 104
#define 输出扫雷22 106
#define 输出扫雷23 103
#define 输出扫雷24 102
#define 输出扫雷25 101

#define 输出卧室1 85
#define 输出卧室2 86
#define 输出卧室3 87
#define 输出卧室4 84
#define 输出卧室5 83
#define 输出卧室6 82

#define 输出厕所1 77
#define 输出厕所2 76
#define 输出厕所3 81
#define 输出厕所4 80
#define 输出厕所5 79
#define 输出厕所6 78
#define 输出厕所7 88
#define 输出厕所8 16

#define 输出儿童房1 100
#define 输出儿童房2 99
#define 输出儿童房3 98
#define 输出儿童房4 97
#define 输出儿童房5 96
#define 输出儿童房6 95
#define 输出儿童房7 92
#define 输出儿童房8 93
#define 输出儿童房9 94

#define 输出走廊1 42
#define 输出走廊2 41
#define 输出走廊3 39
#define 输出走廊4 40

#define 输出楼梯1 7
#define 输出楼梯2 6
#define 输出楼梯3 9
#define 输出楼梯4 8

#define 输出大厅1 61
#define 输出大厅2 60

#define 输出电视1 4
#define 输出电视2 5

#define 输出开场 62

#define 开始游戏 0
#define 醒来正确 97
#define 醒来错误 96
#define 黑卡移开 3
#define 茶室门牌 12
#define 卧室门牌 73
#define 厕所门牌 71
#define 儿童房门牌 72
#define 书房门牌 11

#define 出口门锁 0
#define 草灯 75
#define 楼梯主灯 66
#define 楼梯侧窗灯 68
#define 楼梯主灯红 67
#define 楼梯侧窗灯红 69
#define 二楼走廊灯 65
#define 开场正确锁 63
#define 开场错误锁 62
#define 电视下箱锁 3
#define 茶室门锁 14
#define 茶室灯 72
#define 厕所灯 73
#define 厕所门锁 91
#define 卧室灯 74
#define 卧室门锁 89
#define 儿童房灯 71
#define 儿童房门锁 90
#define 书房灯 70
#define 书房门锁 15

#define 出口门牌 1

#define 猜拳胜利 70
#define 猜拳失败 69

#define 彩球过关 14
#define 八罪外门 43
#define 八罪过关 15

#define 蜡烛1 44
#define 蜡烛2 45
#define 蜡烛3 48
#define 蜡烛4 49
#define 蜡烛5 50
#define 蜡烛6 51
#define 蜡烛7 46
#define 蜡烛8 47
#define 蜡烛9 52
#define 蜡烛10 53
#define 蜡烛11 54
#define 蜡烛12 55
#define 蜡烛13 56
#define 蜡烛14 57
#define 蜡烛15 58
#define 蜡烛16 59

#define 娃娃1 89
#define 娃娃2 90
#define 娃娃3 91
#define 娃娃4 92

#define 伸缩楼梯 10
#define 火柴1 98
#define 火柴2 99

#define 数字摆放 2
#define 保险箱 1

#define 电视信号 2
#define 红色射灯 12

#define 拉绳1 74
#define 拉绳2 75
#endif //FKS1_USER_H