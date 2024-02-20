//
// Created by 26913 on 2023/12/14.
//
//

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "user.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "cmsis_os.h"
#include "gpio.h"

#define GameTaskNum 1
int gameFlags[GameTaskNum] = {51};
SemaphoreHandle_t xGameSemaphore[GameTaskNum];

extern QueueHandle_t GameMessageQueueHandle;
extern EventGroupHandle_t InputEventGroup[(INPUT_NUM / 24) + 1];
extern EventGroupHandle_t MusicEventGroup;
extern QueueHandle_t OutputMessageQueueHandle;
bool ASCTaskHandle[9];
TaskHandle_t BlinkTaskHandle = NULL;

void GameInit(void) {
    for (int i = 0; i < GameTaskNum; i++)
        xGameSemaphore[i] = xSemaphoreCreateMutex();
}

void StartGameFlagTask(void const *argument) {
    GameMessage newMessage;
    for (;;) {
        if (xQueueReceive(GameMessageQueueHandle, &newMessage, portMAX_DELAY) == pdTRUE) {
            xSemaphoreTake(xGameSemaphore[newMessage.num], portMAX_DELAY);
            gameFlags[newMessage.num] = newMessage.Data;
            xSemaphoreGive(xGameSemaphore[newMessage.num]);
        }
    }
}


#define GameTimeReset RunTime = 0

bool GameDelay(int *pvRunTime, int waitTime) {
    if (waitTime < *pvRunTime) {
        *pvRunTime = 0;
        return true;
    }
    return false;
}

void StartGameTask(void const *argument) {
    int RunTime = 0;
    for (;;) {
        xSemaphoreTake(xGameSemaphore[0], portMAX_DELAY);
        switch (gameFlags[0]) {
            case 0://等待游戏开始
            {
                EventBits_t bits = xEventGroupWaitBits(InputEventGroup[游戏开始 / 24], TO_BIT(游戏开始), pdTRUE, pdTRUE,
                                                       0);
                if (bits & TO_BIT(游戏开始)) {
                    gameFlags[0]++;
                }
                break;
            }
            case 1: //游戏开始
            {
                char *fileName1 = "/68.mp3";
                PlayMusicName(&MUSIC_2, fileName1, strlen(fileName1), 单曲循环);
                char *fileName = "/01.mp3";
                PlayMusicName(&MUSIC_1, fileName, strlen(fileName), 单曲停止);

                SetOutput(主光源, GPIO_PIN_SET);
                osDelay(7000);
                gameFlags[0]++;
                break;
            }
            case 2: // 等待第一次门把手
            {
                EventBits_t bits = xEventGroupWaitBits(InputEventGroup[门把手 / 24], TO_BIT(门把手), pdTRUE, pdTRUE, 0);
                if (bits & TO_BIT(门把手)) {
                    gameFlags[0]++;
                }
                break;
            }
            case 3:// 第一次门把手
            {

                char *fileName = "/02.mp3";
                PlayMusicName(&MUSIC_1, fileName, strlen(fileName), 单曲停止);
                gameFlags[0]++;
                break;
            }
            case 4:// 等待小爱同学1
            {
                EventBits_t bits = xEventGroupWaitBits(InputEventGroup[小爱1 / 24], TO_BIT(小爱1), pdTRUE, pdTRUE, 0);
                if (bits & TO_BIT(小爱1)) {
                    gameFlags[0]++;
                }
                break;
            }
            case 5:// 小爱同学1
            {
                SetOutput(路由器柜门, GPIO_PIN_RESET);
                gameFlags[0]++;
                break;
            }
            case 6:// 等待路由器重启按钮
            {
                EventBits_t bits = xEventGroupWaitBits(InputEventGroup[路由器重启按钮 / 24], TO_BIT(路由器重启按钮),
                                                       pdTRUE, pdTRUE, 0);
                if (bits & TO_BIT(路由器重启按钮)) {
                    gameFlags[0]++;
                }
                break;
            }
            case 7://路由器重启按钮
            {
                char *fileName = "/03.mp3";
                PlayMusicName(&MUSIC_1, fileName, strlen(fileName), 单曲停止);
                gameFlags[0]++;
                break;
            }
            case 8:// 等待密码器
            {
                EventBits_t bits = xEventGroupWaitBits(InputEventGroup[密码器 / 24], TO_BIT(密码器),
                                                       pdTRUE, pdTRUE, 0);
                if (bits & TO_BIT(密码器)) {
                    gameFlags[0]++;
                }
                break;
            }
            case 9:// 密码器
            {
                char *fileName = "/04.mp3";
                PlayMusicName(&MUSIC_1, fileName, strlen(fileName), 单曲停止);
                gameFlags[0]++;
                break;
            }
            case 10: // 等待路由器重启按钮
            {
                EventBits_t bits = xEventGroupWaitBits(InputEventGroup[路由器重启按钮 / 24], TO_BIT(路由器重启按钮),
                                                       pdTRUE, pdTRUE, 0);
                if (bits & TO_BIT(路由器重启按钮)) {
                    gameFlags[0]++;
                }
                break;
            }
            case 11://路由器重启按钮
            {
                char *fileName = "/05.mp3";
                PlayMusicName(&MUSIC_1, fileName, strlen(fileName), 单曲停止);
                gameFlags[0]++;
                break;
            }
            case 12://等待第二次摸门把手
            {
                EventBits_t bits = xEventGroupWaitBits(InputEventGroup[门把手 / 24], TO_BIT(门把手), pdTRUE, pdTRUE, 0);
                if (bits & TO_BIT(门把手)) {
                    gameFlags[0]++;
                }
                break;
            }
            case 13: //第二次摸门把手
            {
                char fileName[] = "/06.mp3";
                PlayMusicName(&MUSIC_1, fileName, strlen(fileName), 单曲停止);
                gameFlags[0]++;
                GameTimeReset;
                break;
            }
            case 14: //
            {
                if (GameDelay(&RunTime, 11000)) {
                    gameFlags[0]++;
                }
                break;
            }
            case 15: //
            {
                SetOutput(日记柜门, GPIO_PIN_RESET);
                gameFlags[0]++;
                break;
            }
            case 16: //等待小爱2
            {
                EventBits_t bits = xEventGroupWaitBits(InputEventGroup[小爱2 / 24], TO_BIT(小爱2), pdTRUE, pdTRUE, 0);
                if (bits & TO_BIT(小爱2)) {
                    gameFlags[0]++;
                }
                break;
            }
            case 17: //小爱2
            {
                SetOutput(游戏卡带柜, GPIO_PIN_RESET);
                gameFlags[0]++;
                break;
            }
            case 18: //等待小霸王干簧管
            {
                EventBits_t bits = xEventGroupWaitBits(InputEventGroup[小霸王 / 24], TO_BIT(小霸王), pdTRUE, pdTRUE, 0);
                if (bits & TO_BIT(小霸王)) {
                    gameFlags[0]++;
                }
                break;
            }
            case 19: //小霸王干簧管
            {
                SetOutput(电视电源, GPIO_PIN_SET);
                gameFlags[0]++;
                break;
            }
            case 20: //等待小说密码器
            {
                EventBits_t bits = xEventGroupWaitBits(InputEventGroup[小说密码器 / 24], TO_BIT(小说密码器), pdTRUE,
                                                       pdTRUE, 0);
                if (bits & TO_BIT(小说密码器)) {
                    gameFlags[0]++;
                }
                break;
            }
            case 21: //小说密码器
            {
                SetOutput(小说柜门, GPIO_PIN_RESET);
                gameFlags[0]++;
                GameTimeReset;
                break;
            }
            case 22: //
            {
                if (GameDelay(&RunTime, 5000)) { gameFlags[0]++; }
                break;
            }
            case 23: //
            {
                char *fileName = "/07.mp3";
                PlayMusicName(&MUSIC_1, fileName, strlen(fileName), 单曲停止);
                gameFlags[0]++;
                GameTimeReset;
                break;
            }
            case 24: //
            {
                if (GameDelay(&RunTime, 20000)) { gameFlags[0]++; }
                break;
            }
            case 25: //哈利波特门
            {
                SetOutput(哈利波特门, GPIO_PIN_RESET);
                gameFlags[0]++;
                break;
            }
            case 26: //等待电脑微信
            {
                EventBits_t bits = xEventGroupWaitBits(InputEventGroup[电脑微信 / 24], TO_BIT(电脑微信), pdTRUE,
                                                       pdTRUE, 0);
                if (bits & TO_BIT(电脑微信)) {
                    gameFlags[0]++;
                }
                break;
            }
            case 27: //电脑微信
            {
                char *fileName = "/12.mp3";
                PlayMusicName(&MUSIC_1, fileName, strlen(fileName), 单曲停止);
                SetOutput(三摆件门, GPIO_PIN_RESET);
                SetOutput(初音cd柜门, GPIO_PIN_RESET);
                gameFlags[0]++;
                break;
            }
            case 28: //等待三摆件过关
            {
                EventBits_t bits = xEventGroupWaitBits(InputEventGroup[三摆件过关 / 24], TO_BIT(三摆件过关), pdTRUE,
                                                       pdTRUE, 0);
                if (bits & TO_BIT(三摆件过关)) {
                    gameFlags[0]++;
                }
                break;
            }
            case 29: //三摆件过关
            {
                char *fileName = "/14.mp3";
                PlayMusicName(&MUSIC_1, fileName, strlen(fileName), 单曲停止);
                SetOutput(漫画柜门, GPIO_PIN_RESET);
                gameFlags[0]++;
                GameTimeReset;
                break;
            }
            case 30: //等待
            {
                if (GameDelay(&RunTime, 7000)) { gameFlags[0]++; }
                break;
            }
            case 31: //等待
            {
                char *fileName = "/15.mp3";
                PlayMusicName(&MUSIC_1, fileName, strlen(fileName), 单曲停止);
                SetOutput(电脑过关, GPIO_PIN_SET);
                osDelay(500);
                SetOutput(电脑过关, GPIO_PIN_RESET);
                gameFlags[0]++;
                break;
            }
            case 32: //等待同学语音
            {
                EventBits_t bits = xEventGroupWaitBits(InputEventGroup[同学语音 / 24], TO_BIT(同学语音), pdTRUE,
                                                       pdTRUE, 0);
                if (bits & TO_BIT(同学语音)) {
                    gameFlags[0]++;
                    GameTimeReset;
                }
                break;
            }
            case 33: //等待
            {
                if (GameDelay(&RunTime, 6000)) { gameFlags[0]++; }
                break;
            }
            case 34: //等待
            {
                char *fileName = "/18.mp3";
                PlayMusicName(&MUSIC_1, fileName, strlen(fileName), 单曲停止);
                gameFlags[0]++;
                break;
            }
            case 35: //等待杂志按钮
            {
                EventBits_t bits = xEventGroupWaitBits(InputEventGroup[杂志按钮 / 24], TO_BIT(杂志按钮), pdTRUE,
                                                       pdTRUE, 0);
                if (bits & TO_BIT(杂志按钮)) {
                    gameFlags[0]++;
                }
                break;
            }
            case 36: //杂志按钮
            {
                char *fileName = "/19.mp3";
                PlayMusicName(&MUSIC_1, fileName, strlen(fileName), 单曲停止);
                gameFlags[0]++;
                break;
            }
            case 37: //等待时钟1
            {
                EventBits_t bits1 = xEventGroupWaitBits(InputEventGroup[杂志按钮 / 24], TO_BIT(杂志按钮), pdTRUE,
                                                       pdTRUE, 0);
                if (bits1 & TO_BIT(杂志按钮)) {
                    char *fileName = "/19.mp3";
                    PlayMusicName(&MUSIC_1, fileName, strlen(fileName), 单曲停止);
                }

                EventBits_t bits = xEventGroupWaitBits(InputEventGroup[时钟1 / 24], TO_BIT(时钟1), pdTRUE,
                                                       pdTRUE, 0);
                if (bits & TO_BIT(时钟1)) {
                    gameFlags[0]++;
                }
                break;
            }
            case 38: //时钟1
            {
                char *fileName = "/20.mp3";
                PlayMusicName(&MUSIC_1, fileName, strlen(fileName), 单曲停止);
                gameFlags[0]++;
                break;
            }
            case 39: //等待杂志按钮
            {
                EventBits_t bits = xEventGroupWaitBits(InputEventGroup[杂志按钮 / 24], TO_BIT(杂志按钮), pdTRUE,
                                                       pdTRUE, 0);
                if (bits & TO_BIT(杂志按钮)) {
                    gameFlags[0]++;
                }
                break;
            }
            case 40: //杂志按钮
            {
                SetOutput(杂志柜门, GPIO_PIN_RESET);
                char *fileName = "/21.mp3";
                PlayMusicName(&MUSIC_1, fileName, strlen(fileName), 单曲停止);
                gameFlags[0]++;
                break;
            }
            case 41: //等待电脑视频过关
            {
                EventBits_t bits = xEventGroupWaitBits(InputEventGroup[电脑视频过关 / 24], TO_BIT(电脑视频过关), pdTRUE,
                                                       pdTRUE, 0);
                if (bits & TO_BIT(电脑视频过关)) {
                    gameFlags[0]++;

                }
                break;
            }
            case 42: //电脑视频过关
            {
                char *fileName = "/22.mp3";
                PlayMusicName(&MUSIC_1, fileName, strlen(fileName), 单曲停止);
                gameFlags[0]++;
                GameTimeReset;
                break;
            }
            case 43: //等待
            {
                if (GameDelay(&RunTime, 15000)) { gameFlags[0]++; }
                break;
            }
            case 44: //等待
            {
                char *fileName = "/23.mp3";
                PlayMusicName(&MUSIC_1, fileName, strlen(fileName), 单曲停止);
                gameFlags[0]++;
                break;
            }
            case 45: //等待时钟2
            {
                EventBits_t bits = xEventGroupWaitBits(InputEventGroup[时钟2 / 24], TO_BIT(时钟2), pdTRUE,
                                                       pdTRUE, 0);
                if (bits & TO_BIT(时钟2)) {
                    gameFlags[0]++;
                }
                break;
            }
            case 46: //时钟2
            {
                char *fileName = "/24.mp3";
                PlayMusicName(&MUSIC_1, fileName, strlen(fileName), 单曲停止);
                gameFlags[0]++;
                break;
            }
            case 47: //等待时钟3
            {
                EventBits_t bits = xEventGroupWaitBits(InputEventGroup[时钟3 / 24], TO_BIT(时钟3), pdTRUE,
                                                       pdTRUE, 0);
                if (bits & TO_BIT(时钟3)) {
                    gameFlags[0]++;
                }
                break;
            }
            case 48: //时钟3
            {
                SetOutput(海贼柜门, GPIO_PIN_RESET);
                char *fileName = "/25.mp3";
                PlayMusicName(&MUSIC_1, fileName, strlen(fileName), 单曲停止);
                gameFlags[0]++;
                break;
            }
            case 49: //等待时钟4
            {
                EventBits_t bits = xEventGroupWaitBits(InputEventGroup[时钟4 / 24], TO_BIT(时钟4), pdTRUE,
                                                       pdTRUE, 0);
                if (bits & TO_BIT(时钟4)) {
                    gameFlags[0]++;
                }
                break;
            }
            case 50: //时钟4
            {
                SetOutput(卧室门锁, GPIO_PIN_RESET);
                char *fileName = "/30.mp3";
                PlayMusicName(&MUSIC_1, fileName, strlen(fileName), 单曲停止);
                gameFlags[0]++;
                break;
            }
            case 51: //
            {
                break;
            }
            case 52: //复位
            {
                PauseMusic(&MUSIC_1);
                PauseMusic(&MUSIC_2);

                SetUsbMusic(&MUSIC_1);
                SetUsbMusic(&MUSIC_2);

                SetPlayMode(&MUSIC_1,单曲停止);
                SetPlayMode(&MUSIC_2,单曲循环);

                SetOutput(入口门, GPIO_PIN_SET);
                SetOutput(路由器柜门, GPIO_PIN_SET);
                SetOutput(杂志柜门, GPIO_PIN_SET);
                SetOutput(主光源, GPIO_PIN_SET);
                SetOutput(哈利波特门, GPIO_PIN_SET);
                SetOutput(手办刷卡灯, GPIO_PIN_SET);
                SetOutput(三摆件门, GPIO_PIN_SET);
                SetOutput(初音cd柜门, GPIO_PIN_SET);
                SetOutput(漫画柜门, GPIO_PIN_SET);
                SetOutput(日记柜门, GPIO_PIN_SET);
                SetOutput(小说柜门, GPIO_PIN_SET);
                SetOutput(海贼柜门, GPIO_PIN_SET);
                SetOutput(游戏卡带柜, GPIO_PIN_SET);
                SetOutput(卧室门锁, GPIO_PIN_SET);
                SetOutput(电视电源, GPIO_PIN_SET);
                SetOutput(电脑复位, GPIO_PIN_SET);
                SetOutput(游戏柜门, GPIO_PIN_RESET);
                SetOutput(时钟电源, GPIO_PIN_RESET);
                osDelay(500);
                SetOutput(电脑复位, GPIO_PIN_RESET);
                SetOutput(游戏柜门, GPIO_PIN_SET);
                SetOutput(时钟电源, GPIO_PIN_SET);
                gameFlags[0] = 51;
                break;
            }
            case 53: //复场
            {
                SetOutput(入口门, GPIO_PIN_SET);
                SetOutput(路由器柜门, GPIO_PIN_SET);
                SetOutput(杂志柜门, GPIO_PIN_SET);
                SetOutput(主光源, GPIO_PIN_RESET);
                SetOutput(哈利波特门, GPIO_PIN_SET);
                SetOutput(手办刷卡灯, GPIO_PIN_RESET);
                SetOutput(三摆件门, GPIO_PIN_SET);
                SetOutput(初音cd柜门, GPIO_PIN_SET);
                SetOutput(漫画柜门, GPIO_PIN_SET);
                SetOutput(日记柜门, GPIO_PIN_SET);
                SetOutput(小说柜门, GPIO_PIN_SET);
                SetOutput(海贼柜门, GPIO_PIN_SET);
                SetOutput(游戏卡带柜, GPIO_PIN_SET);
                SetOutput(卧室门锁, GPIO_PIN_SET);
                SetOutput(电视电源, GPIO_PIN_RESET);

                gameFlags[0] = 51;
                break;
            }


        }
        if (gameFlags[0] >= 25 && gameFlags[0] < 51) {
            static int switchState[9];
            EventBits_t bits1 = xEventGroupGetBits(InputEventGroup[手办赫敏 / 24]);
            if ((bits1 & TO_BIT(手办赫敏)) && (switchState[0] == 0)) {
                switchState[0] = 1;
                char *fileName = "/08.mp3";
                PlayMusicName(&MUSIC_1, fileName, strlen(fileName), 单曲停止);
                SetOutput(手办刷卡灯, GPIO_PIN_SET);
            }
            if (((bits1 & TO_BIT(手办赫敏))==0) && (switchState[0] == 1)){
                PauseMusic(&MUSIC_1);
                switchState[0] = 0;
                SetOutput(手办刷卡灯, GPIO_PIN_RESET);
            }
            if ((bits1 & TO_BIT(手办哈里)) && (switchState[1] == 0)) {
                switchState[1] = 1;
                char *fileName = "/09.mp3";
                PlayMusicName(&MUSIC_1, fileName, strlen(fileName), 单曲停止);
                SetOutput(手办刷卡灯, GPIO_PIN_SET);
            }
            if (((bits1 & TO_BIT(手办哈里))==0) && (switchState[1] == 1)) {
                PauseMusic(&MUSIC_1);
                switchState[1] = 0;
                SetOutput(手办刷卡灯, GPIO_PIN_RESET);
            }
            if ((bits1 & TO_BIT(手办斯内普)) && (switchState[2] == 0)) {
                switchState[2] = 1;
                char *fileName = "/10.mp3";
                PlayMusicName(&MUSIC_1, fileName, strlen(fileName), 单曲停止);
                SetOutput(手办刷卡灯, GPIO_PIN_SET);
            }
            if (((bits1 & TO_BIT(手办斯内普))==0) && (switchState[2] == 1)){
                PauseMusic(&MUSIC_1);
                SetOutput(手办刷卡灯, GPIO_PIN_RESET);
                switchState[2] = 0;
            }
            if ((bits1 & TO_BIT(手办纽特)) && (switchState[3] == 0)) {
                switchState[3] = 1;
                char *fileName = "/11.mp3";
                PlayMusicName(&MUSIC_1, fileName, strlen(fileName), 单曲停止);
                SetOutput(手办刷卡灯, GPIO_PIN_SET);
            }
            if (((bits1 & TO_BIT(手办纽特))==0) && (switchState[3] == 1)) {
                PauseMusic(&MUSIC_1);
                switchState[3] = 0;
                SetOutput(手办刷卡灯, GPIO_PIN_RESET);
            }

            if ((bits1 & TO_BIT(手办初音)) && (switchState[4] == 0)) {
                switchState[4] = 1;
                char *fileName = "/13.mp3";
                PlayMusicName(&MUSIC_1, fileName, strlen(fileName), 单曲停止);
                SetOutput(手办刷卡灯, GPIO_PIN_SET);
            }
            if (((bits1 & TO_BIT(手办初音))==0) && (switchState[4] == 1)) {
                PauseMusic(&MUSIC_1);
                switchState[4] = 0;
                SetOutput(手办刷卡灯, GPIO_PIN_RESET);
            }

            if ((bits1 & TO_BIT(手办路飞)) && (switchState[5] == 0)) {
                switchState[5] = 1;
                char *fileName = "/26.mp3";
                PlayMusicName(&MUSIC_1, fileName, strlen(fileName), 单曲停止);
                SetOutput(手办刷卡灯, GPIO_PIN_SET);
            }
            if (((bits1 & TO_BIT(手办路飞))==0) && (switchState[5] == 1)) {
                PauseMusic(&MUSIC_1);
                switchState[5] = 0;
                SetOutput(手办刷卡灯, GPIO_PIN_RESET);
            }
            if ((bits1 & TO_BIT(手办乔巴)) && (switchState[6] == 0)) {
                switchState[6] = 1;
                char *fileName = "/27.mp3";
                PlayMusicName(&MUSIC_1, fileName, strlen(fileName), 单曲停止);
                SetOutput(手办刷卡灯, GPIO_PIN_SET);
            }
            if (((bits1 & TO_BIT(手办乔巴))==0) && (switchState[6] == 1)) {
                PauseMusic(&MUSIC_1);
                switchState[6] = 0;
                SetOutput(手办刷卡灯, GPIO_PIN_RESET);
            }
            if ((bits1 & TO_BIT(手办索隆)) && (switchState[7] == 0)) {
                switchState[7] = 1;
                char *fileName = "/29.mp3";
                PlayMusicName(&MUSIC_1, fileName, strlen(fileName), 单曲停止);
                SetOutput(手办刷卡灯, GPIO_PIN_SET);
            }
            if (((bits1 & TO_BIT(手办索隆))==0) && (switchState[7] == 1)){
                PauseMusic(&MUSIC_1);
                switchState[7] = 0;
                SetOutput(手办刷卡灯, GPIO_PIN_RESET);
            }
            if ((bits1 & TO_BIT(手办娜美)) && (switchState[8] == 0)) {
                switchState[8] = 1;
                char *fileName = "/28.mp3";
                PlayMusicName(&MUSIC_1, fileName, strlen(fileName), 单曲停止);
                SetOutput(手办刷卡灯, GPIO_PIN_SET);
            }
            if (((bits1 & TO_BIT(手办娜美))==0) && (switchState[8] == 1)) {
                PauseMusic(&MUSIC_1);
                switchState[8] = 0;
                SetOutput(手办刷卡灯, GPIO_PIN_RESET);
            }


            if (gameFlags[0] >= 25 && gameFlags[0] < 51) {
                static int CDState[12];
                EventBits_t bits1 = xEventGroupGetBits(InputEventGroup[cd输入1 / 24]);
                if (((bits1 & (TO_BIT(cd输入1) | TO_BIT(cd输入2)))==(TO_BIT(cd输入1) | TO_BIT(cd输入2))) && (CDState[0] == 0)) {
                    CDState[0] = 1;
                    char *fileName = "/70.mp3";
                    PlayMusicName(&MUSIC_1, fileName, strlen(fileName), 单曲停止);
                }
                if (((bits1 & (TO_BIT(cd输入1) | TO_BIT(cd输入2)))==0)&& (CDState[0] == 1)) {
                    PauseMusic(&MUSIC_1);
                    CDState[0] = 0;
                }
                if (((bits1 & (TO_BIT(cd输入2) | TO_BIT(cd输入5)))== (TO_BIT(cd输入2) | TO_BIT(cd输入5))) && (CDState[1] == 0)) {
                    CDState[1] = 1;
                    char *fileName = "/78.mp3";
                    PlayMusicName(&MUSIC_1, fileName, strlen(fileName), 单曲停止);
                }
                if (((bits1 & (TO_BIT(cd输入2) | TO_BIT(cd输入5)))==0)&& (CDState[1] == 1)) {
                    PauseMusic(&MUSIC_1);
                    CDState[1] = 0;
                }
                if (((bits1 & (TO_BIT(cd输入2) | TO_BIT(cd输入6)))==(TO_BIT(cd输入2) | TO_BIT(cd输入6))) && (CDState[2] == 0)) {
                    CDState[2] = 1;
                    char *fileName = "/76.mp3";
                    PlayMusicName(&MUSIC_1, fileName, strlen(fileName), 单曲停止);
                }
                if (((bits1 & (TO_BIT(cd输入2) | TO_BIT(cd输入6)))==0)&& (CDState[2] == 1)) {
                    PauseMusic(&MUSIC_1);
                    CDState[2] = 0;
                }
                if (((bits1 & (TO_BIT(cd输入2) | TO_BIT(cd输入3)))==(TO_BIT(cd输入2) | TO_BIT(cd输入3))) && (CDState[3] == 0)) {
                    CDState[3] = 1;
                    char *fileName = "/77.mp3";
                    PlayMusicName(&MUSIC_1, fileName, strlen(fileName), 单曲停止);
                }
                if (((bits1 & (TO_BIT(cd输入2) | TO_BIT(cd输入3)))==0)&& (CDState[3] == 1)) {
                    PauseMusic(&MUSIC_1);
                    CDState[3] = 0;
                }
                if (((bits1 & (TO_BIT(cd输入3) | TO_BIT(cd输入4)))==(TO_BIT(cd输入3) | TO_BIT(cd输入4))) && (CDState[4] == 0)) {
                    CDState[4] = 1;
                    char *fileName = "/73.mp3";
                    PlayMusicName(&MUSIC_1, fileName, strlen(fileName), 单曲停止);
                }
                if (((bits1 & (TO_BIT(cd输入3) | TO_BIT(cd输入4)))==0)&& (CDState[4] == 1)) {
                    PauseMusic(&MUSIC_1);
                    CDState[4] = 0;
                }
                if (((bits1 & (TO_BIT(cd输入3) | TO_BIT(cd输入5)))==(TO_BIT(cd输入3) | TO_BIT(cd输入5))) && (CDState[5] == 0)) {
                    CDState[5] = 1;
                    char *fileName = "/72.mp3";
                    PlayMusicName(&MUSIC_1, fileName, strlen(fileName), 单曲停止);
                }
                if (((bits1 & (TO_BIT(cd输入3) | TO_BIT(cd输入5)))==0)&& (CDState[5] == 1)) {
                    PauseMusic(&MUSIC_1);
                    CDState[5] = 0;
                }
                if (((bits1 & (TO_BIT(cd输入1) | TO_BIT(cd输入6)))==(TO_BIT(cd输入1) | TO_BIT(cd输入6))) && (CDState[6] == 0)) {
                    CDState[6] = 1;
                    char *fileName = "/74.mp3";
                    PlayMusicName(&MUSIC_1, fileName, strlen(fileName), 单曲停止);
                }
                if (((bits1 & (TO_BIT(cd输入1) | TO_BIT(cd输入6)))==0)&& (CDState[6] == 1)) {
                    PauseMusic(&MUSIC_1);
                    CDState[6] = 0;
                }
                if (((bits1 & (TO_BIT(cd输入3) | TO_BIT(cd输入6)))==(TO_BIT(cd输入3) | TO_BIT(cd输入6))) && (CDState[7] == 0)) {
                    CDState[7] = 1;
                    char *fileName = "/71.mp3";
                    PlayMusicName(&MUSIC_1, fileName, strlen(fileName), 单曲停止);
                }
                if (((bits1 & (TO_BIT(cd输入3) | TO_BIT(cd输入6)))==0)&& (CDState[7] == 1)) {
                    PauseMusic(&MUSIC_1);
                    CDState[7] = 0;
                }
                if (((bits1 & (TO_BIT(cd输入1) | TO_BIT(cd输入3)))==(TO_BIT(cd输入1) | TO_BIT(cd输入3))) && (CDState[8] == 0)) {
                    CDState[8] = 1;
                    char *fileName = "/80.mp3";
                    PlayMusicName(&MUSIC_1, fileName, strlen(fileName), 单曲停止);
                }
                if (((bits1 & (TO_BIT(cd输入1) | TO_BIT(cd输入3)))==0)&& (CDState[8] == 1)) {
                    PauseMusic(&MUSIC_1);
                    CDState[8] = 0;
                }
                if (((bits1 & (TO_BIT(cd输入1) | TO_BIT(cd输入4)))==(TO_BIT(cd输入1) | TO_BIT(cd输入4))) && (CDState[9] == 0)) {
                    CDState[9] = 1;
                    char *fileName = "/75.mp3";
                    PlayMusicName(&MUSIC_1, fileName, strlen(fileName), 单曲停止);
                }
                if (((bits1 & (TO_BIT(cd输入1) | TO_BIT(cd输入4)))==0)&& (CDState[9] == 1)) {
                    PauseMusic(&MUSIC_1);
                    CDState[9] = 0;
                }
                if (((bits1 & (TO_BIT(cd输入1) | TO_BIT(cd输入5)))==(TO_BIT(cd输入1) | TO_BIT(cd输入5))) && (CDState[10] == 0)) {
                    CDState[10] = 1;
                    char *fileName = "/79.mp3";
                    PlayMusicName(&MUSIC_1, fileName, strlen(fileName), 单曲停止);
                }
                if (((bits1 & (TO_BIT(cd输入1) | TO_BIT(cd输入5)))==0)&& (CDState[10] == 1)){
                    PauseMusic(&MUSIC_1);
                    CDState[10] = 0;
                }
                if (((bits1 & (TO_BIT(cd输入2) | TO_BIT(cd输入4)))==(TO_BIT(cd输入2) | TO_BIT(cd输入4))) && (CDState[11] == 0)) {
                    CDState[11] = 1;
                    char *fileName = "/69.mp3";
                    PlayMusicName(&MUSIC_1, fileName, strlen(fileName), 单曲停止);
                }
                if (((bits1 & (TO_BIT(cd输入2) | TO_BIT(cd输入4)))==0)&& (CDState[11] == 1)) {
                    PauseMusic(&MUSIC_1);
                    CDState[11] = 0;
                }
//                帝国 1-2     	70
//                生物股长2-5    78
//                极乐净土2-6   	76
//                超电磁炮2-3    77
//                砒霜机3-4	73
//                灌篮高手3-5  		72
//                转动命运之轮1-6  	74
//                zone3-6			71
//                新垣结衣1-3    80
//                和田光司1-4    75
//                小林位于1-5    79
//                花泽香菜2-4    69

            }



        }
        xSemaphoreGive(xGameSemaphore[0]);


        RunTime++;
        osDelay(1); //等待音频播放
        static int oldGameFlag = 100;
        if (gameFlags[0] != oldGameFlag) {
            uint8_t TxBuff[5] = {0xCC, 0x05, 0x00, gameFlags[0], 0xFF};
            HAL_UART_Transmit(&huart1, TxBuff, 5, HAL_MAX_DELAY);
            oldGameFlag = gameFlags[0];
        }


        static int Flag=51;
        if(gameFlags[0]!=Flag)
        {
            uint8_t TxBuffer[5]={0xCC,0x05,0x00,gameFlags[0],0xFF};
            HAL_UART_Transmit(&huart1,TxBuffer,5,portMAX_DELAY);
            Flag=gameFlags[0];
        }
        xSemaphoreGive(xGameSemaphore[0]);
    }
}