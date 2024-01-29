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
int gameFlags[GameTaskNum] = {100};
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
                char *fileName = "/01.mp3";
                PlayMusicName(&MUSIC_1, fileName, strlen(fileName), 单曲停止);
                SetOutput(主光源, GPIO_PIN_SET);
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
            case 16: // 等待路由器重启按钮
            {
                EventBits_t bits = xEventGroupWaitBits(InputEventGroup[路由器重启按钮 / 24], TO_BIT(路由器重启按钮),
                                                       pdTRUE, pdTRUE, 0);
                if (bits & TO_BIT(路由器重启按钮)) {
                    gameFlags[0]++;
                }
                break;
            }
            case 17://路由器重启按钮
            {
                char *fileName = "/05.mp3";
                PlayMusicName(&MUSIC_1, fileName, strlen(fileName), 单曲停止);
                gameFlags[0]++;
                break;
            }
            case 18://等待第二次摸门把手
            {
                EventBits_t bits = xEventGroupWaitBits(InputEventGroup[门把手 / 24], TO_BIT(门把手), pdTRUE, pdTRUE, 0);
                if (bits & TO_BIT(门把手)) {
                    gameFlags[0]++;
                }
                break;
            }
            case 19: //第二次摸门把手
            {
                char *fileName = "/06.mp3";
                PlayMusicName(&MUSIC_1, fileName, strlen(fileName), 单曲停止);
                gameFlags[0]++;
                GameTimeReset;
                break;
            }
            case 20: //
            {
                if (GameDelay(&RunTime, 11000)) { gameFlags[0]++; }
                break;
            }
            case 21: //
            {
                SetOutput(小说柜门, GPIO_PIN_RESET);
                gameFlags[0]++;
                break;
            }
            case 22: //等待小爱2
            {
                EventBits_t bits = xEventGroupWaitBits(InputEventGroup[小爱2 / 24], TO_BIT(小爱2), pdTRUE, pdTRUE, 0);
                if (bits & TO_BIT(小爱2)) {
                    gameFlags[0]++;
                }
                break;
            }
            case 23: //小爱2
            {
                SetOutput(游戏卡带柜, GPIO_PIN_RESET);
                gameFlags[0]++;
                break;
            }
            case 24: //等待小霸王干簧管
            {
                EventBits_t bits = xEventGroupWaitBits(InputEventGroup[小霸王 / 24], TO_BIT(小霸王), pdTRUE, pdTRUE, 0);
                if (bits & TO_BIT(小霸王)) {
                    gameFlags[0]++;
                }
                break;
            }
            case 25: //小霸王干簧管
            {
                SetOutput(电视电源, GPIO_PIN_SET);
                break;
            }
            case 26: //等待小说密码器
            {
                EventBits_t bits = xEventGroupWaitBits(InputEventGroup[小说密码器 / 24], TO_BIT(小说密码器), pdTRUE,
                                                       pdTRUE, 0);
                if (bits & TO_BIT(小说密码器)) {
                    gameFlags[0]++;
                }
                break;
            }
            case 27: //小说密码器
            {
                SetOutput(杂志柜门, GPIO_PIN_RESET);
                gameFlags[0]++;
                GameTimeReset;
                break;
            }
            case 28: //
            {
                if (GameDelay(&RunTime, 5000)) { gameFlags[0]++; }
                break;
            }
            case 29: //
            {
                char *fileName = "/07.mp3";
                PlayMusicName(&MUSIC_1, fileName, strlen(fileName), 单曲停止);
                gameFlags[0]++;
                GameTimeReset;
                break;
            }
            case 30: //
            {
                if (GameDelay(&RunTime, 20000)) { gameFlags[0]++; }
                break;
            }
            case 31: //哈利波特门
            {
                SetOutput(电视电源, GPIO_PIN_SET);
                break;
            }
            case 32: //复位
            {
                break;
            }
            case 33: //空
            {
                break;
            }
            case 34: //复位
            {
                break;
            }
            case 35: //空
            {
                break;
            }
            case 36: //复位
            {
                break;
            }

        }
        if (gameFlags[0] >= 31 && gameFlags[0] <= 31)
        {

        }


        RunTime++;
        xSemaphoreGive(xGameSemaphore[0]);
        osDelay(1); //等待音频播放
        static int oldGameFlag = 100;
        if (gameFlags[0] != oldGameFlag) {
            uint8_t TxBuff[5] = {0xCC, 0x05, 0x00, gameFlags[0], 0xFF};
            HAL_UART_Transmit(&huart1, TxBuff, 5, HAL_MAX_DELAY);
            oldGameFlag = gameFlags[0];
        }
    }
}
