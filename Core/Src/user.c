//
// Created by 26913 on 2023/12/14.
//
//

#include <string.h>
//#include <stdio.h>
#include "user.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "cmsis_os.h"
#include "gpio.h"
#include <stdbool.h>
SemaphoreHandle_t xGameSemaphore;

extern QueueHandle_t GameMessageQueueHandle;
extern EventGroupHandle_t InputEventGroup[(INPUT_NUM / 24) + 1];
extern EventGroupHandle_t MusicEventGroup;
extern QueueHandle_t OutputMessageQueueHandle;

int gameFlag=0;
void GameInit(void)
{
        xGameSemaphore = xSemaphoreCreateMutex();
}
bool HC1,HC2;
/**********************
        蜡烛(HP)
**********************/
int HP = 16;
void SetHP(int hp) {

    SetOutput(蜡烛1,(1<=hp)?GPIO_PIN_SET:GPIO_PIN_RESET);
    SetOutput(蜡烛2,(2<=hp)?GPIO_PIN_SET:GPIO_PIN_RESET);
    SetOutput(蜡烛3,(3<=hp)?GPIO_PIN_SET:GPIO_PIN_RESET);
    SetOutput(蜡烛4,(4<=hp)?GPIO_PIN_SET:GPIO_PIN_RESET);
    SetOutput(蜡烛5,(5<=hp)?GPIO_PIN_SET:GPIO_PIN_RESET);
    SetOutput(蜡烛6,(6<=hp)?GPIO_PIN_SET:GPIO_PIN_RESET);
    SetOutput(蜡烛7,(7<=hp)?GPIO_PIN_SET:GPIO_PIN_RESET);
    SetOutput(蜡烛8,(8<=hp)?GPIO_PIN_SET:GPIO_PIN_RESET);
    SetOutput(蜡烛9,(9<=hp)?GPIO_PIN_SET:GPIO_PIN_RESET);
    SetOutput(蜡烛10,(10<=hp)?GPIO_PIN_SET:GPIO_PIN_RESET);
    SetOutput(蜡烛11,(11<=hp)?GPIO_PIN_SET:GPIO_PIN_RESET);
    SetOutput(蜡烛12,(12<=hp)?GPIO_PIN_SET:GPIO_PIN_RESET);
    SetOutput(蜡烛13,(13<=hp)?GPIO_PIN_SET:GPIO_PIN_RESET);
    SetOutput(蜡烛14,(14<=hp)?GPIO_PIN_SET:GPIO_PIN_RESET);
    SetOutput(蜡烛15,(15<=hp)?GPIO_PIN_SET:GPIO_PIN_RESET);
    SetOutput(蜡烛16,(16<=hp)?GPIO_PIN_SET:GPIO_PIN_RESET);

}
void SubHP()
{
    PlayMusicA("/02.mp3",单曲停止)
    SetPin(楼梯主灯);
    SetPin(楼梯侧窗灯);
    SetPin(二楼走廊灯);
    osDelay(2000);
    ResetPin(楼梯主灯);
    ResetPin(楼梯侧窗灯);
    ResetPin(二楼走廊灯);
    if(HP==2)
    {
        char FileName[] = "/32.mp3";
        PlayMusicName(&MUSIC_1,FileName, strlen(FileName),单曲停止);
    }
    SetHP(--HP);
}
/**********************
        箱子
**********************/
//箱子数量
#define BOX_NUM 68
//箱子刷卡输入pin
uint8_t BoxInputPin[BOX_NUM]=
        {
                输入茶室1,
                输入茶室2,
                输入茶室3,
                输入茶室4,
                输入茶室5,
                输入茶室6,
                输入茶室7,
                输入茶室8,
                输入扫雷1,
                输入扫雷2,
                输入扫雷3,
                输入扫雷4,
                输入扫雷5,
                输入扫雷6,
                输入扫雷7,
                输入扫雷8,
                输入扫雷9,
                输入扫雷10,
                输入扫雷11,
                输入扫雷12,
                输入扫雷14,
                输入扫雷15,
                输入扫雷16,
                输入扫雷17,
                输入扫雷18,
                输入扫雷19,
                输入扫雷20,
                输入扫雷21,
                输入扫雷22,
                输入扫雷23,
                输入扫雷24,
                输入扫雷25,
                输入卧室1,
                输入卧室2,
                输入卧室3,
                输入卧室4,
                输入卧室5,
                输入卧室6,
                输入厕所1,
                输入厕所2,
                输入厕所3,
                输入厕所4,
                输入厕所5,
                输入厕所6,
                输入厕所7,
                输入厕所8,
                输入儿童房6,
                输入儿童房7,
                输入儿童房4,
                输入儿童房1,
                输入儿童房9,
                输入儿童房5,
                输入儿童房3,
                输入儿童房8,
                输入儿童房2,
                输入走廊1,
                输入走廊2,
                输入走廊3,
                输入走廊4,
                输入楼梯1,
                输入楼梯2,
                输入楼梯3,
                输入楼梯4,
                输入大厅1,
                输入大厅2,
                输入电视1,
                输入电视2,
                输入开场
        };
//箱子电锁输出pin
uint8_t BoxOutputPin[BOX_NUM]=
        {

                输出茶室1,
                输出茶室2,
                输出茶室3,
                输出茶室4,
                输出茶室5,
                输出茶室6,
                输出茶室7,
                输出茶室8,
                输出扫雷1,
                输出扫雷2,
                输出扫雷3,
                输出扫雷4,
                输出扫雷5,
                输出扫雷6,
                输出扫雷7,
                输出扫雷8,
                输出扫雷9,
                输出扫雷10,
                输出扫雷11,
                输出扫雷12,
                输出扫雷14,
                输出扫雷15,
                输出扫雷16,
                输出扫雷17,
                输出扫雷18,
                输出扫雷19,
                输出扫雷20,
                输出扫雷21,
                输出扫雷22,
                输出扫雷23,
                输出扫雷24,
                输出扫雷25,
                输出卧室1,
                输出卧室2,
                输出卧室3,
                输出卧室4,
                输出卧室5,
                输出卧室6,
                输出厕所1,
                输出厕所2,
                输出厕所3,
                输出厕所4,
                输出厕所5,
                输出厕所6,
                输出厕所7,
                输出厕所8,
                输出儿童房1,
                输出儿童房2,
                输出儿童房3,
                输出儿童房4,
                输出儿童房5,
                输出儿童房6,
                输出儿童房7,
                输出儿童房8,
                输出儿童房9,
                输出走廊1,
                输出走廊2,
                输出走廊3,
                输出走廊4,
                输出楼梯1,
                输出楼梯2,
                输出楼梯3,
                输出楼梯4,
                输出大厅1,
                输出大厅2,
                输出电视1,
                输出电视2,
                输出开场
        };
//箱子是否打开过
bool OpenState[BOX_NUM];
//箱子属性
bool BoxProperties[BOX_NUM]=
        {
                true,
                true,
                true,
                true,
                true,
                true,
                false,
                true,
                true,
                false,
                true,
                true,
                true,
                false,
                true,
                true,
                true,
                true,
                true,
                false,
                false,
                false,
                false,
                true,
                true,
                false,
                true,
                true,
                true,
                true,
                true,
                false,
                false,
                true,
                true,
                true,
                true,
                false,
                false,
                true,
                true,
                false,
                true,
                true,
                false,
                true,
                false,
                true,
                true,
                true,
                true,
                true,
                false,
                false,
                true,
                true,
                true,
                false,
                true,
                false,
                true,
                true,
                true,
                true,
                false,
                true,
                false,
                false
        };

int Lz=0;
long int GameTime = 100*60*1000;

int Ls=0;
void StartBoxTask(void const *argument)
{
    for(;;)
    {

//        xSemaphoreTake(xGameSemaphore, portMAX_DELAY);
//        int GF = gameFlag;
//        xSemaphoreGive(xGameSemaphore);
//
//        if((GF>0)&&(GF<30)) {
            for (int i = 0; i < BOX_NUM; i++) {
                if (WaitBit(BoxInputPin[i], pdTRUE)) {
                    OpenLock(BoxOutputPin[i])
                    if (!OpenState[i]) {
                        if (BoxProperties[i]) {
                            if (i == 10) {
                                PlayMusicA("/24.mp3", 单曲停止)
                            } else if (i == 12) {
                                PlayMusicA("/31.mp3", 单曲停止)
                            } else if (i == 14) {
                                PlayMusicA("/29.mp3", 单曲停止)
                            } else if (i == 15) {
                                PlayMusicA("/30.mp3", 单曲停止)
                            } else if (i == 18) {
                                PlayMusicA("/27.mp3", 单曲停止)
                            } else if (i == 24) {
                                PlayMusicA("/25.mp3", 单曲停止)
                            } else if (i == 26) {
                                PlayMusicA("/28.mp3", 单曲停止)
                            } else if (i == 28) {
                                PlayMusicA("/26.mp3", 单曲停止)
                            } else {
                                PlayMusicA("/07.mp3", 单曲停止)
                            }
                        } else {
                            SubHP();
                        }
                        OpenState[i] = true;
                    }
                }
            }

            if (WaitBit(听诊器1, pdTRUE)) {
                PlayMusicA("/41.mp3", 单曲停止)
            }
            if (WaitBit(听诊器2, pdTRUE)) {
                PlayMusicA("/42.mp3", 单曲停止)
            }
            if (WaitBit(听诊器3, pdTRUE)) {
                PlayMusicA("/43.mp3", 单曲停止)
            }
            if (WaitBit(听诊器4, pdTRUE)) {
                PlayMusicA("/44.mp3", 单曲停止)
            }
            if (WaitBit(听诊器5, pdTRUE)) {
                PlayMusicA("/45.mp3", 单曲停止)
            }
            if (WaitBit(听诊器6, pdTRUE)) {
                PlayMusicA("/46.mp3", 单曲停止)
            }
            if (WaitBit(听诊器7, pdTRUE)) {
                PlayMusicA("/47.mp3", 单曲停止)
            }
            if (WaitBit(听诊器8, pdTRUE)) {
                PlayMusicA("/48.mp3", 单曲停止)
            }


            if (WaitBit(猜拳失败, pdTRUE)) {
                Lz++;
                if (Lz == 5) {
                    SubHP();
                    Lz = 0;
                    osDelay(7000);
                }
                PlayMusicA("/20.mp3", 单曲停止)
            }
            if (WaitBit(猜拳胜利, pdTRUE)) {
                PlayMusicA("/19.mp3", 单曲停止)
                Ls++;
            }
            if (Ls == 10) {
                PlayMusicA("/21.mp3", 单曲停止)
                Ls = 11;
            }

            EventBits_t bits = xEventGroupWaitBits(InputEventGroup[3],
                                                   TO_BIT(娃娃1) | TO_BIT(娃娃2) | TO_BIT(娃娃3) | TO_BIT(娃娃4),
                                                   pdTRUE, pdTRUE, 0);
            if ((bits & (TO_BIT(娃娃1) | TO_BIT(娃娃2) | TO_BIT(娃娃3) | TO_BIT(娃娃4)))==(TO_BIT(娃娃1) | TO_BIT(娃娃2) | TO_BIT(娃娃3) | TO_BIT(娃娃4))) {
                ResetPin(伸缩楼梯);
                PlayMusicA("/14.mp3", 单曲停止)
            }
            if (WaitBit(火柴1, pdTRUE)) {
                if ((!HC1) && (HP < 16)) {
                    HC1 = true;
                    SetHP(++HP);
                    PlayMusicA("/15.mp3", 单曲停止)
                }
            }
            if (WaitBit(火柴2, pdTRUE)) {
                if ((!HC2) && (HP < 16)) {
                    HC2 = true;
                    SetHP(++HP);
                    PlayMusicA("/15.mp3", 单曲停止)
                }
            }
            if (WaitBit(数字摆放, pdTRUE)) {
                ResetPin(保险箱);
            }

            if (WaitBit(拉绳1, pdTRUE)) {
                SetPin(卧室灯);
                ResetPin(厕所灯);
            }
            if (WaitBit(拉绳2, pdTRUE)) {
                SetPin(厕所灯);
                ResetPin(卧室灯);
            }
        }
//    }
}
int playerNum=0;
#define GameTimeReset RunTime = 0
bool GameDelay(int *pvRunTime,int waitTime)
{
    if(waitTime<*pvRunTime)
    {
        return true;
    }
    return false;
}
void StartGameFlagTask(void const *argument)
{
    GameMessage newMessage;
    for(;;)
    {
        if(xQueueReceive(GameMessageQueueHandle, &newMessage, portMAX_DELAY) == pdTRUE)
        {
            xSemaphoreTake(xGameSemaphore, portMAX_DELAY);
            gameFlag= newMessage.Data;
            xSemaphoreGive(xGameSemaphore);
        }
    }
}
void StartGameTask(void const *argument)
{
    int RunTime = 0;
    for(;;)
    {
        xSemaphoreTake(xGameSemaphore, portMAX_DELAY);
        switch (gameFlag) {
            case 100:
            {
                if (WaitBit(开始游戏, pdTRUE)) {

                    playerNum=8;
                    gameFlag=2;
                }
                break;
            }
            case 1: {
                if (WaitBit(开始游戏, pdTRUE)) {

                    playerNum=7;
                    gameFlag++;
                }
                break;
            }
            case 2: {
                for (int i = 0; i < BOX_NUM; i++)//复位所有箱子状态
                {
                    OpenState[i] = false;
                }
                GameTime = 100 * 60 * 1000;//复位倒计时时间
                PlayMusicB("/01.mp3", 单曲停止)
                GameTimeReset;
                gameFlag=88;
                break;
            }
            case 88:
            {
                delay(72*1000)
                break;
            }
            case 89:
            {
                SetPin(电视信号);
                GameTimeReset;
                gameFlag++;
                break;
            }
            case 90:
            {
                delay(45*1000)
                break;
            }
            case 91:
            {
                ResetPin(草灯);
                gameFlag=3;
                break;
            }
            case 3: {

                if (WaitBit(醒来正确, pdTRUE)) {
                    OpenLock(开场正确锁)
                    PlayMusicA("/04.mp3", 单曲停止)
                    SetPin(草灯);
                    ResetPin(楼梯主灯);
                    ResetPin(楼梯侧窗灯);
                    ResetPin(书房灯);
                    gameFlag = 6;
                }
                if (WaitBit(醒来错误, pdTRUE)) {
                    OpenLock(开场错误锁)
                    OpenState[67]=true;
                    SetPin(草灯);
                    SetHP(--HP);
                    PlayMusicA("/03.mp3", 单曲停止)
                    osDelay(7000);
                    ResetPin(楼梯主灯);
                    ResetPin(楼梯侧窗灯);
                    ResetPin(书房灯);
                    GameTimeReset;
                    gameFlag++;
                }
                break;
            }
            case 4: {
                delay(30 * 1000)
                break;
            }
            case 5: {
                OpenLock(开场正确锁)
                gameFlag++;
                break;
            }
            case 6: {
                if (NWaitBit(黑卡移开, pdFALSE)) {
                    gameFlag++;
                }
                break;
            }
            case 7: {
                if (playerNum < 8) {
                    PlayMusicA("/05.mp3", 单曲停止)
                    GameTimeReset;
                    gameFlag++;
                } else {
                    PlayMusicA("/06.mp3", 单曲停止)
                    gameFlag = 10;
                }
                break;
            }
            case 8: {
                delay(41 * 1000)
                break;
            }
            case 9: {
                OpenLock(电视下箱锁)
                gameFlag++;
                break;
            }
            case 10: {
                if (WaitBit(茶室门牌, pdTRUE)) {
                    gameFlag++;
                }
                break;
            }
            case 11: {
                ResetPin(茶室门锁);
                ResetPin(茶室灯);
                PlayMusicA("/09.mp3", 单曲停止)
                PlayMusicB("/08.mp3", 单曲循环)
                gameFlag++;
                break;
            }
            case 12: {
                if (WaitBit(厕所门牌, pdTRUE)) {
                    gameFlag++;
                }
                break;
            }
            case 13:
            {
                ResetPin(厕所门锁);
                ResetPin(厕所灯);
                gameFlag++;
                break;
            }
            case 14:
            {
                if (WaitBit(卧室门牌, pdTRUE)) {
                    gameFlag++;
                }
                break;
            }
            case 15:
            {
                ResetPin(卧室门锁);
                PlayMusicB("/12.mp3",单曲循环)
                PlayMusicA("/13.mp3",单曲停止)
                gameFlag++;
                break;
            }
            case 16:
            {
                if(WaitBit(儿童房门牌,pdTRUE))
                {
                    gameFlag++;
                }
                break;
            }
            case 17:
            {
                ResetPin(儿童房门锁);
                ResetPin(儿童房灯);
                PlayMusicB("/16.mp3",单曲循环)
                PlayMusicA("/18.mp3",单曲停止)
                gameFlag++;
                break;
            }
            case 18:
            {
                if(WaitBit(书房门牌,pdTRUE))
                {
                    gameFlag++;
                }
                break;
            }
            case 19:
            {
                ResetPin(书房门锁);
                ResetPin(书房灯);
                PlayMusicB("/22.mp3",单曲循环)

                PlayMusicA("/23.mp3",单曲停止)
                gameFlag++;
                break;
            }
            case 20:
            {
                if(WaitBit(彩球过关,pdTRUE))
                {
                    ResetPin(八罪外门);
                    gameFlag++;
                }
                break;
            }
            case 21:
            {
                if(WaitBit(八罪过关,pdTRUE))
                {
                    gameFlag=50;
                }
                break;
            }

            case 30:
            {
                StopMusicB;

                SetPin(草灯);
                SetPin(茶室灯);
                SetPin(厕所灯);
                SetPin(卧室灯);
                SetPin(儿童房灯);
                SetPin(书房灯);
                SetPin(草灯);
                SetPin(楼梯主灯);
                SetPin(楼梯侧窗灯);
                SetPin(二楼走廊灯);

                SetPin(楼梯主灯红);
                SetPin(楼梯侧窗灯红);
                SetPin(红色射灯);

                PlayMusicA("/33.mp3",单曲停止)
                GameTimeReset;
                gameFlag++;
                break;
            }
            case 31:
            {
                delay(38*1000)
                break;
            }
            case 32:
            {
                ResetPin(楼梯主灯红);
                ResetPin(楼梯侧窗灯红);
                ResetPin(红色射灯);
                gameFlag++;
                break;
            }
            case 33:
            {
                delay(79*1000)
                break;
            }
            case 34:
            {
                ResetPin(草灯);
                ResetPin(茶室灯);
                ResetPin(厕所灯);
                ResetPin(卧室灯);
                ResetPin(儿童房灯);
                ResetPin(书房灯);
                ResetPin(草灯);
                ResetPin(楼梯主灯);
                ResetPin(楼梯侧窗灯);
                ResetPin(二楼走廊灯);

                ResetPin(出口门锁);
                gameFlag=0;
                break;
            }
            case 40:
            {
                StopMusicB;

                SetPin(草灯);
                SetPin(茶室灯);
                SetPin(厕所灯);
                SetPin(卧室灯);
                SetPin(儿童房灯);
                SetPin(书房灯);
                SetPin(草灯);
                SetPin(楼梯主灯);
                SetPin(楼梯侧窗灯);
                SetPin(二楼走廊灯);

                SetPin(楼梯主灯红);
                SetPin(楼梯侧窗灯红);
                SetPin(红色射灯);

                PlayMusicA("/34.mp3",单曲停止)
                GameTimeReset;
                gameFlag++;
                break;
            }
            case 41:
            {
                delay(33*1000)
                break;
            }
            case 42:
            {
                ResetPin(楼梯主灯红);
                ResetPin(楼梯侧窗灯红);
                ResetPin(红色射灯);
                gameFlag++;
                break;
            }
            case 43:
            {
                delay(79*1000)
                break;
            }
            case 44:
            {
                ResetPin(草灯);
                ResetPin(茶室灯);
                ResetPin(厕所灯);
                ResetPin(卧室灯);
                ResetPin(儿童房灯);
                ResetPin(书房灯);
                ResetPin(草灯);
                ResetPin(楼梯主灯);
                ResetPin(楼梯侧窗灯);
                ResetPin(二楼走廊灯);

                ResetPin(出口门锁);
                gameFlag=0;
                break;
            }

            case 50:
            {
                StopMusicB;

                SetPin(草灯);
                SetPin(茶室灯);
                SetPin(厕所灯);
                SetPin(卧室灯);
                SetPin(儿童房灯);
                SetPin(书房灯);
                SetPin(草灯);
                SetPin(楼梯主灯);
                SetPin(楼梯侧窗灯);
                SetPin(二楼走廊灯);

                ResetPin(楼梯主灯红);
                ResetPin(楼梯侧窗灯红);

                PlayMusicA("/35.mp3",单曲停止)
                GameTimeReset;
                gameFlag++;
                break;
            }
            case 51:
            {
                delay(56*1000)
                break;
            }
            case 52:
            {
                SetPin(红色射灯);
                gameFlag++;
                break;
            }

            case 53:
            {
                delay(79*1000)
                break;
            }
            case 54:
            {
                ResetPin(草灯);
                ResetPin(茶室灯);
                ResetPin(厕所灯);
                ResetPin(卧室灯);
                ResetPin(儿童房灯);
                ResetPin(书房灯);
                ResetPin(草灯);
                ResetPin(楼梯主灯);
                ResetPin(楼梯侧窗灯);
                ResetPin(二楼走廊灯);

                ResetPin(出口门锁);
                gameFlag=0;
                break;
            }
            case 60:
            {
                SetHP(HP = 16);//复位血量
                StopMusicA;
                StopMusicB;
                ResetPin(书房灯);
                ResetPin(卧室灯);
                ResetPin(厕所灯);
                ResetPin(茶室灯);
                ResetPin(草灯);
                ResetPin(楼梯主灯);
                ResetPin(楼梯侧窗灯);
                ResetPin(二楼走廊灯);
                ResetPin(楼梯侧窗灯);

                SetPin(出口门锁);
                SetPin(书房门锁);
                SetPin(儿童房门锁);
                SetPin(卧室门锁);
                SetPin(厕所门锁);
                SetPin(茶室门锁);
                ResetPin(电视信号);
                SetPin(保险箱);
                SetPin(八罪外门);
                gameFlag=0;
                break;
            }
            case 61:
            {
                SetPin(书房灯);
                SetPin(卧室灯);
                SetPin(厕所灯);
                SetPin(茶室灯);
                SetPin(草灯);
                SetPin(楼梯主灯);
                SetPin(楼梯侧窗灯);
                SetPin(儿童房灯);
                SetPin(二楼走廊灯);
                SetPin(楼梯侧窗灯);
                SetPin(伸缩楼梯);
                ResetPin(红色射灯);
                Ls=0;
                Lz=0;
                HC1=false;
                HC2=false;
                gameFlag=0;
                break;
            }
        }
        if((gameFlag>0)&&(gameFlag<30))
        {
            if(GameTime>0)
            {
                GameTime--;
            } else
            {
                gameFlag=30;
            }
        }
        if(WaitBit(出口门牌,pdTRUE))
        {
            gameFlag=40;
        }
        RunTime++;
        static int oldGameFlag = 0;
        if(gameFlag!=oldGameFlag)
        {
            uint8_t TxBuff[5] = {0xCC,0x05,0x00,gameFlag,0xFF};
            HAL_UART_Transmit(&huart1,TxBuff,5,HAL_MAX_DELAY);
            oldGameFlag = gameFlag;
        }
        xSemaphoreGive(xGameSemaphore);
        osDelay(1); //等待音频播放
    }
}
