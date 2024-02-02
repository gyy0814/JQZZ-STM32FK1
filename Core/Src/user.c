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
int saomiaoFlags[20] = {0};
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
            case 0://第一次按下遥控
            {
                EventBits_t bits = xEventGroupWaitBits(InputEventGroup[艾琳开始 / 24], TO_BIT(艾琳开始), pdTRUE,
                                                       pdTRUE, 0);
                if (bits & TO_BIT(艾琳开始)) {
                    gameFlags[0]++;
                    SetOutput(开场语音, GPIO_PIN_SET);
                    GameTimeReset;
                }
                break;
            }
            case 1://
            {
                if (GameDelay(&RunTime, 13000)) {
                    gameFlags[0]++;
                    SetOutput(开场语音, GPIO_PIN_RESET);
                }
                break;
            }
            case 2://
            {
                EventBits_t bits = xEventGroupWaitBits(InputEventGroup[游戏开始 / 24], TO_BIT(游戏开始), pdTRUE,
                                                       pdTRUE, 0);
                if (bits & TO_BIT(游戏开始)) {
                    gameFlags[0]++;
                }
                gameFlags[0]++;
                break;
            }
            case 3://播放bgm开场,播放语音,这就是王晗雨的家了,开灯
            {

                char *fileName1 = "/02.mp3";
                PlayMusicName(&MUSIC_1, fileName1, strlen(fileName1), 单曲循环);
                char *fileName2 = "/03.mp3";
                PlayMusicName(&MUSIC_2, fileName2, strlen(fileName2), 单曲停止);
                SetOutput(电视背景灯带, GPIO_PIN_SET);
                SetOutput(客厅房顶灯带, GPIO_PIN_SET);
                gameFlags[0]++;
                break;
            }
            case 4://等待检测遥控器1
            {
                EventBits_t bits = xEventGroupWaitBits(InputEventGroup[遥控器5 / 24], TO_BIT(遥控器5), pdTRUE,
                                                       pdTRUE, 0);
                if (bits & TO_BIT(遥控器5)) {
                    gameFlags[0]++;
                }
                break;
            }
            case 5://检测遥控器1 电视开启播放待机画面雪花
            {
                SetOutput(电视信号1, GPIO_PIN_SET);
                osDelay(1000);
                SetOutput(电视信号1, GPIO_PIN_RESET);
                gameFlags[0]++;
                break;
            }
            case 6://等待打电话
            {
                EventBits_t bits1 = xEventGroupWaitBits(InputEventGroup[打电话 / 24], TO_BIT(打电话), pdTRUE,
                                                        pdTRUE, 0);
                if (bits1 & TO_BIT(打电话)) {
                    gameFlags[0]++;
                }
                break;
            }
            case 7://等待卧室密码器
            {
                EventBits_t bits1 = xEventGroupWaitBits(InputEventGroup[卧室密码器 / 24], TO_BIT(卧室密码器),
                                                        pdTRUE, pdTRUE, 0);
                if (bits1 & TO_BIT(卧室密码器)) {
                    gameFlags[0]++;
                }
                break;
            }
            case 8://卧室密码器
            {
                char *fileName = "/14.mp3";
                PlayMusicName(&MUSIC_1, fileName, strlen(fileName), 单曲循环);
                SetOutput(卧室门锁, GPIO_PIN_RESET);
                SetOutput(卧室月球灯, GPIO_PIN_SET);
                SetOutput(卧室灯带, GPIO_PIN_SET);
                gameFlags[0]++;
                GameTimeReset;
                break;
            }
            case 9://延时触发语音
            {
//                if (GameDelay(&RunTime, 40000)) {
                    gameFlags[0]++;
//                }
                break;
            }
            case 10://
            {
                char *fileName = "/15.mp3";
                PlayMusicName(&MUSIC_2, fileName, strlen(fileName), 单曲停止);
                gameFlags[0]++;
                break;
            }
            case 11://等待卧室抽屉打开
            {
                EventBits_t bits1 = xEventGroupWaitBits(InputEventGroup[卧室抽屉打开 / 24],
                                                        TO_BIT(卧室抽屉打开), pdTRUE, pdTRUE, 0);
                if (bits1 & TO_BIT(卧室抽屉打开)) {
                    gameFlags[0]++;
                }
                break;
            }
            case 12://卧室抽屉打开
            {
                char *fileName = "/16.mp3";
                PlayMusicName(&MUSIC_2, fileName, strlen(fileName), 单曲停止);
                gameFlags[0]++;
                GameTimeReset;
                break;
            }
            case 13://
            {
                if (GameDelay(&RunTime, 40000)) { gameFlags[0]++; }
                break;
            }
            case 14://
            {
                SetOutput(卧室柜上灯, GPIO_PIN_SET);
                gameFlags[0]++;
                break;
            }
            case 15://录取通知
            {
                EventBits_t bits1 = xEventGroupWaitBits(InputEventGroup[录取通知 / 24], TO_BIT(录取通知), pdTRUE,
                                                        pdTRUE, 0);
                if (bits1 & TO_BIT(录取通知)) {
                    gameFlags[0]++;
                }
                break;
            }
            case 16://
            {
                SetOutput(接电话信号, GPIO_PIN_SET);
                osDelay(1000);
                SetOutput(接电话信号, GPIO_PIN_RESET);
                gameFlags[0]++;
                GameTimeReset;
                break;
            }
            case 17://
            {
                if (GameDelay(&RunTime, 30000)) { gameFlags[0]++; }
                break;
            }
            case 18://延时开柜子
            {
                char *fileName = "/4.mp3";
                PlayMusicName(&MUSIC_2, fileName, strlen(fileName), 单曲停止);
                SetOutput(卧室柜锁, GPIO_PIN_RESET);
                SetOutput(卧室柜下灯, GPIO_PIN_SET);
                gameFlags[0]++;
                break;
            }
            case 19://等待记忆卡拿走
            {
                EventBits_t bits1 = xEventGroupWaitBits(InputEventGroup[记忆卡拿走 / 24], TO_BIT(记忆卡拿走),
                                                        pdTRUE, pdTRUE, 0);
                if (bits1 & TO_BIT(记忆卡拿走)) {
                    gameFlags[0]++;
                }
                break;
            }
            case 20://记忆卡拿走
            {
                char *fileName = "/18.mp3";
                PlayMusicName(&MUSIC_2, fileName, strlen(fileName), 单曲停止);
                gameFlags[0]++;
                break;
            }
            case 21://等待记忆卡刷卡1
            {
                EventBits_t bits1 = xEventGroupWaitBits(InputEventGroup[记忆卡刷卡1 / 24], TO_BIT(记忆卡刷卡1),
                                                        pdTRUE, pdTRUE, 0);
                if (bits1 & TO_BIT(记忆卡刷卡1)) {
                    gameFlags[0]++;
                }
                break;
            }
            case 22://记忆卡刷卡1
            {
                SetOutput(电视信号6, GPIO_PIN_SET);
                char *fileName = "/19.mp3";
                PlayMusicName(&MUSIC_2, fileName, strlen(fileName), 单曲停止);
                osDelay(500);
                SetOutput(电视信号6, GPIO_PIN_RESET);
                gameFlags[0]++;
                break;
            }
            case 23://等待敲门信号
            {
                EventBits_t bits1 = xEventGroupWaitBits(InputEventGroup[敲门信号 / 24], TO_BIT(敲门信号),
                                                        pdTRUE, pdTRUE, 0);
                if (bits1 & TO_BIT(敲门信号)) {
                    gameFlags[0]++;
                }
                break;
            }
            case 24://敲门信号
            {
                SetOutput(敲门锁, GPIO_PIN_RESET);
                char *fileName = "/20.mp3";
                PlayMusicName(&MUSIC_2, fileName, strlen(fileName), 单曲停止);
                gameFlags[0]++;
                break;
            }
            case 25://等待数独刷卡
            {
                EventBits_t bits1 = xEventGroupWaitBits(InputEventGroup[书房数独 / 24], TO_BIT(书房数独),
                                                        pdTRUE, pdTRUE, 0);
                if (bits1 & TO_BIT(书房数独)) {
                    gameFlags[0]++;
                }
                break;
            }
            case 26://数独刷卡
            {
                char *fileName = "/21.mp3";
                PlayMusicName(&MUSIC_1, fileName, strlen(fileName), 单曲循环);
                SetOutput(书房门锁, GPIO_PIN_RESET);
                SetOutput(书房灯带, GPIO_PIN_SET);
                SetOutput(书房台灯, GPIO_PIN_SET);
                SetOutput(书房月球灯, GPIO_PIN_SET);
                gameFlags[0]++;
                break;
            }
            case 27://等待记忆卡2
            {
                EventBits_t bits1 = xEventGroupWaitBits(InputEventGroup[记忆卡刷卡2 / 24], TO_BIT(记忆卡刷卡2),
                                                        pdTRUE, pdTRUE, 0);
                if (bits1 & TO_BIT(记忆卡刷卡2)) {
                    gameFlags[0]++;
                }
                break;
            }
            case 28://记忆卡2
            {
                SetOutput(电视信号7, GPIO_PIN_SET);
                osDelay(500);
                SetOutput(电视信号7, GPIO_PIN_RESET);
                gameFlags[0]++;
                GameTimeReset;
                break;
            }
            case 29://
            {
                if (GameDelay(&RunTime, 43000)) { gameFlags[0]++; }
                break;

            }
            case 30://
            {
                char *fileName = "/23.mp3";
                PlayMusicName(&MUSIC_2, fileName, strlen(fileName), 单曲停止);
                gameFlags[0]++;
                break;
            }
            case 31://等待维修密码
            {
                EventBits_t bits1 = xEventGroupWaitBits(InputEventGroup[快递柜密码器 / 24],
                                                        TO_BIT(快递柜密码器), pdTRUE, pdTRUE, 0);
                if (bits1 & TO_BIT(快递柜密码器)) {
                    gameFlags[0]++;
                }
                break;
            }
            case 32://维修密码
            {
                SetOutput(线路轨门锁, GPIO_PIN_RESET);
                SetOutput(线路射灯, GPIO_PIN_SET);
                gameFlags[0]++;
                break;
            }
            case 33://等待线路过关
            {
                EventBits_t bits1 = xEventGroupWaitBits(InputEventGroup[线路过关 / 24], TO_BIT(线路过关),
                                                        pdTRUE, pdTRUE, 0);
                if (bits1 & TO_BIT(线路过关)) {
                    gameFlags[0]++;

                }
                break;
            }
            case 34://线路过关
            {
                char *fileName1 = "/25.mp3";
                PlayMusicName(&MUSIC_2, fileName1, strlen(fileName1), 单曲停止);
                gameFlags[0]++;
                GameTimeReset;
                break;
            }
            case 35://
            {
                if (GameDelay(&RunTime, 26000)) { gameFlags[0]++; }
                break;
            }
            case 36://
            {
                char *fileName2 = "/26.mp3";
                PlayMusicName(&MUSIC_2, fileName2, strlen(fileName2), 单曲停止);
                SetOutput(快递柜门锁, GPIO_PIN_RESET);
                gameFlags[0]++;
                GameTimeReset;
                break;
            }
            case 37://
            {
                if (GameDelay(&RunTime, 6000)) { gameFlags[0]++; }
                break;
            }
            case 38://
            {
                char *fileName3 = "/27.mp3";
                PlayMusicName(&MUSIC_2, fileName3, strlen(fileName3), 单曲停止);
                gameFlags[0]++;
                break;
            }
            case 39://等待艾利刷卡
            {
                EventBits_t bits1 = xEventGroupWaitBits(InputEventGroup[艾利刷卡 / 24], TO_BIT(艾利刷卡),
                                                        pdTRUE, pdTRUE, 0);
                if (bits1 & TO_BIT(艾利刷卡)) {
                    gameFlags[0]++;
                }
                break;
            }
            case 40://艾利刷卡
            {
                SetOutput(卧室柜下灯, GPIO_PIN_RESET);
                SetOutput(卧室柜上灯, GPIO_PIN_RESET);
                SetOutput(书房灯带, GPIO_PIN_RESET);
                SetOutput(线路射灯, GPIO_PIN_RESET);
                SetOutput(书房月球灯, GPIO_PIN_RESET);
                SetOutput(电视背景灯带, GPIO_PIN_RESET);
                SetOutput(书房台灯, GPIO_PIN_RESET);
                SetOutput(卧室月球灯, GPIO_PIN_RESET);
                SetOutput(客厅房顶灯带, GPIO_PIN_RESET);
                SetOutput(卧室灯带, GPIO_PIN_RESET);

                PauseMusic(&MUSIC_1);
                char *fileName2 = "/28.mp3";
                PlayMusicName(&MUSIC_2, fileName2, strlen(fileName2), 单曲停止);

                osDelay(5000);
                SetOutput(扫描激光, GPIO_PIN_SET);
                osDelay(2000);
                SetOutput(扫描灯1, GPIO_PIN_SET);
                osDelay(2000);
                SetOutput(扫描灯2, GPIO_PIN_SET);
                osDelay(2000);
                SetOutput(扫描灯3, GPIO_PIN_SET);
                osDelay(2000);
                SetOutput(扫描灯4, GPIO_PIN_SET);
                osDelay(2000);
                SetOutput(扫描灯5, GPIO_PIN_SET);
                osDelay(2000);
                SetOutput(扫描灯6, GPIO_PIN_SET);
                osDelay(2000);
                SetOutput(扫描灯7, GPIO_PIN_SET);
                osDelay(2000);
                SetOutput(扫描灯8, GPIO_PIN_SET);
                osDelay(2000);
                SetOutput(扫描灯9, GPIO_PIN_SET);
                osDelay(2000);
                SetOutput(扫描灯10, GPIO_PIN_SET);

                SetOutput(扫描激光, GPIO_PIN_RESET);
                SetOutput(灯带, GPIO_PIN_SET);
                SetOutput(扫描仪门锁, GPIO_PIN_RESET);
                char *fileName1 = "/30.mp3";
                PlayMusicName(&MUSIC_1, fileName1, strlen(fileName1), 单曲循环);
                osDelay(200);
                char *fileName3 = "/31.mp3";
                PlayMusicName(&MUSIC_2, fileName3, strlen(fileName3), 单曲停止);
                gameFlags[0]++;
                break;
            }
            case 41://扫描仪
            {
                EventBits_t bits1 = xEventGroupWaitBits(InputEventGroup[扫描仪1 / 24], TO_BIT(扫描仪1), pdTRUE,
                                                        pdTRUE, 0);
                if (bits1 & TO_BIT(扫描仪1)) {
                    saomiaoFlags[1] = 1;
                    SetOutput(信息块1, GPIO_PIN_RESET);
                    SetOutput(扫描灯1, GPIO_PIN_RESET);
                    char *fileName1 = "/32.mp3";
                    PlayMusicName(&MUSIC_2, fileName1, strlen(fileName1), 单曲停止);
                }
                EventBits_t bits2 = xEventGroupWaitBits(InputEventGroup[扫描仪2 / 24], TO_BIT(扫描仪2), pdTRUE,
                                                        pdTRUE, 0);
                if (bits2 & TO_BIT(扫描仪2)) {
                    saomiaoFlags[2] = 1;
                    SetOutput(信息块2, GPIO_PIN_RESET);
                    SetOutput(扫描灯2, GPIO_PIN_RESET);
                    char *fileName1 = "/33.mp3";
                    PlayMusicName(&MUSIC_2, fileName1, strlen(fileName1), 单曲停止);
                }
                EventBits_t bits3 = xEventGroupWaitBits(InputEventGroup[扫描仪3 / 24], TO_BIT(扫描仪3), pdTRUE,
                                                        pdTRUE, 0);
                if (bits3 & TO_BIT(扫描仪3)) {
                    saomiaoFlags[3] = 1;
                    SetOutput(信息块3, GPIO_PIN_RESET);
                    SetOutput(扫描灯3, GPIO_PIN_RESET);
                    char *fileName1 = "/34.mp3";
                    PlayMusicName(&MUSIC_2, fileName1, strlen(fileName1), 单曲停止);
                }
                EventBits_t bits4 = xEventGroupWaitBits(InputEventGroup[扫描仪4 / 24], TO_BIT(扫描仪4), pdTRUE,
                                                        pdTRUE, 0);
                if (bits4 & TO_BIT(扫描仪4)) {
                    saomiaoFlags[4] = 1;
                    SetOutput(信息块4, GPIO_PIN_RESET);
                    SetOutput(扫描灯4, GPIO_PIN_RESET);
                    char *fileName1 = "/35.mp3";
                    PlayMusicName(&MUSIC_2, fileName1, strlen(fileName1), 单曲停止);
                }
                EventBits_t bits5 = xEventGroupWaitBits(InputEventGroup[扫描仪5 / 24], TO_BIT(扫描仪5), pdTRUE,
                                                        pdTRUE, 0);
                if (bits5 & TO_BIT(扫描仪5)) {
                    saomiaoFlags[5] = 1;
                    SetOutput(信息块5, GPIO_PIN_RESET);
                    SetOutput(扫描灯5, GPIO_PIN_RESET);
                    char *fileName1 = "/36.mp3";
                    PlayMusicName(&MUSIC_2, fileName1, strlen(fileName1), 单曲停止);
                }
                EventBits_t bits6 = xEventGroupWaitBits(InputEventGroup[扫描仪6 / 24], TO_BIT(扫描仪6), pdTRUE,
                                                        pdTRUE, 0);
                if (bits6 & TO_BIT(扫描仪6)) {
                    saomiaoFlags[6] = 1;
                    SetOutput(信息块6, GPIO_PIN_RESET);
                    SetOutput(扫描灯6, GPIO_PIN_RESET);
                    char *fileName1 = "/37.mp3";
                    PlayMusicName(&MUSIC_2, fileName1, strlen(fileName1), 单曲停止);
                }
                EventBits_t bits7 = xEventGroupWaitBits(InputEventGroup[扫描仪7 / 24], TO_BIT(扫描仪7), pdTRUE,
                                                        pdTRUE, 0);
                if (bits7 & TO_BIT(扫描仪7)) {
                    saomiaoFlags[7] = 1;
                    SetOutput(信息块7, GPIO_PIN_RESET);
                    SetOutput(扫描灯7, GPIO_PIN_RESET);
                    char *fileName1 = "/38.mp3";
                    PlayMusicName(&MUSIC_2, fileName1, strlen(fileName1), 单曲停止);
                }
                EventBits_t bits8 = xEventGroupWaitBits(InputEventGroup[扫描仪8 / 24], TO_BIT(扫描仪8), pdTRUE,
                                                        pdTRUE, 0);
                if (bits8 & TO_BIT(扫描仪8)) {
                    saomiaoFlags[8] = 1;
                    SetOutput(信息块8, GPIO_PIN_RESET);
                    SetOutput(扫描灯8, GPIO_PIN_RESET);
                    char *fileName1 = "/39.mp3";
                    PlayMusicName(&MUSIC_2, fileName1, strlen(fileName1), 单曲停止);
                }
                EventBits_t bits9 = xEventGroupWaitBits(InputEventGroup[扫描仪9 / 24], TO_BIT(扫描仪9), pdTRUE,
                                                        pdTRUE, 0);
                if (bits9 & TO_BIT(扫描仪9)) {
                    saomiaoFlags[9] = 1;
                    SetOutput(信息块9, GPIO_PIN_RESET);
                    SetOutput(扫描灯9, GPIO_PIN_RESET);
                    char *fileName1 = "/40.mp3";
                    PlayMusicName(&MUSIC_2, fileName1, strlen(fileName1), 单曲停止);
                }
                EventBits_t bits10 = xEventGroupWaitBits(InputEventGroup[扫描仪10 / 24], TO_BIT(扫描仪10),
                                                         pdTRUE, pdTRUE, 0);
                if (bits10 & TO_BIT(扫描仪10)) {
                    saomiaoFlags[10] = 1;
                    SetOutput(信息块10, GPIO_PIN_RESET);
                    SetOutput(扫描灯10, GPIO_PIN_RESET);
                    char *fileName1 = "/41.mp3";
                    PlayMusicName(&MUSIC_2, fileName1, strlen(fileName1), 单曲停止);
                }
                if (saomiaoFlags[1] == 1 &&
                    saomiaoFlags[2] == 1 &&
                    saomiaoFlags[3] == 1 &&
                    saomiaoFlags[4] == 1 &&
                    saomiaoFlags[5] == 1 &&
                    saomiaoFlags[6] == 1 &&
                    saomiaoFlags[7] == 1 &&
                    saomiaoFlags[8] == 1 &&
                    saomiaoFlags[9] == 1 &&
                    saomiaoFlags[10] == 1) {
                    gameFlags[0]++;
                    GameTimeReset;
                }
                break;
            }
            case 42://十个扫描过关
            {
                if (GameDelay(&RunTime, 3000)) { gameFlags[0]++; }
                break;
            }
            case 43://十个扫描过关
            {
                char *fileName1 = "/42.mp3";
                PlayMusicName(&MUSIC_2, fileName1, strlen(fileName1), 单曲停止);
                osDelay(12000);
                SetOutput(信息投影灯, GPIO_PIN_SET);
                gameFlags[0]++;
                break;
            }
            case 44://等待矮桌
            {
                EventBits_t bits = xEventGroupWaitBits(InputEventGroup[矮桌 / 24], TO_BIT(矮桌), pdTRUE, pdTRUE,
                                                       0);
                if (bits & TO_BIT(矮桌)) {
                    gameFlags[0]++;
                }
                break;
            }
            case 45://矮桌过关
            {
                char *fileName1 = "/43.mp3";
                PlayMusicName(&MUSIC_2, fileName1, strlen(fileName1), 单曲停止);
                gameFlags[0]++;
                GameTimeReset;
                break;
            }
            case 46://
            {
                if (GameDelay(&RunTime, 8000)) { gameFlags[0]++; }
                break;
            }
            case 47://
            {
                char *fileName2 = "/44.mp3";
                PlayMusicName(&MUSIC_2, fileName2, strlen(fileName2), 单曲停止);
                SetOutput(前场电脑触发, GPIO_PIN_SET);
                osDelay(500);
                SetOutput(前场电脑触发, GPIO_PIN_RESET);
                gameFlags[0]++;
                GameTimeReset;
                break;
            }
            case 48://
            {
                if (GameDelay(&RunTime, 45000)) { gameFlags[0]++; }
                break;
            }
            case 49://
            {
                char *fileName3 = "/45.mp3";
                PlayMusicName(&MUSIC_1, fileName3, strlen(fileName3), 单曲停止);
                char *fileName4 = "/46.mp3";
                PlayMusicName(&MUSIC_2, fileName4, strlen(fileName4), 单曲停止);
                gameFlags[0]++;
                GameTimeReset;
                break;
            }
            case 50://
            {
                if (GameDelay(&RunTime, 26000)) { gameFlags[0]++; }
                break;
            }
            case 51://
            {
                SetOutput(电脑显示器, GPIO_PIN_RESET);
                SetOutput(信息投影灯, GPIO_PIN_RESET);
                SetOutput(灯带, GPIO_PIN_RESET);
                gameFlags[0]++;
                break;
            }
            case 52://
            {
                break;
            }
            case 53://复位
            {
                PauseMusic(&MUSIC_1);
                PauseMusic(&MUSIC_2);
                SetOutput(电视信号1, GPIO_PIN_RESET);
                SetOutput(电视信号2, GPIO_PIN_RESET);
                SetOutput(电视信号3, GPIO_PIN_RESET);
                SetOutput(电视信号4, GPIO_PIN_RESET);
                SetOutput(电视信号5, GPIO_PIN_RESET);
                SetOutput(电视信号6, GPIO_PIN_RESET);
                SetOutput(接电话信号, GPIO_PIN_RESET);
                SetOutput(卧室门锁, GPIO_PIN_SET);

                SetOutput(卧室柜锁, GPIO_PIN_SET);
                SetOutput(卧室柜下灯, GPIO_PIN_SET);
                SetOutput(卧室柜上灯, GPIO_PIN_SET);
                SetOutput(书房门锁, GPIO_PIN_SET);
                SetOutput(摄像头灯, GPIO_PIN_RESET);
                SetOutput(书房灯带, GPIO_PIN_SET);
                SetOutput(线路轨门锁, GPIO_PIN_SET);
                SetOutput(快递柜门锁, GPIO_PIN_SET);
                SetOutput(扫描仪门锁, GPIO_PIN_SET);
                SetOutput(信息块1, GPIO_PIN_SET);

                SetOutput(信息块2, GPIO_PIN_SET);
                SetOutput(信息块3, GPIO_PIN_SET);
                SetOutput(信息块4, GPIO_PIN_SET);
                SetOutput(信息块5, GPIO_PIN_SET);
                SetOutput(信息块6, GPIO_PIN_SET);
                SetOutput(信息块7, GPIO_PIN_SET);
                SetOutput(信息块8, GPIO_PIN_SET);
                SetOutput(信息块9, GPIO_PIN_SET);
                SetOutput(信息块10, GPIO_PIN_SET);

                SetOutput(灯带, GPIO_PIN_SET);
                SetOutput(线路射灯, GPIO_PIN_SET);
                SetOutput(扫描灯1, GPIO_PIN_RESET);
                SetOutput(扫描灯2, GPIO_PIN_RESET);
                SetOutput(扫描灯3, GPIO_PIN_RESET);
                SetOutput(扫描灯4, GPIO_PIN_RESET);
                SetOutput(扫描灯5, GPIO_PIN_RESET);
                SetOutput(扫描灯6, GPIO_PIN_RESET);
                SetOutput(扫描灯7, GPIO_PIN_RESET);
                SetOutput(扫描灯8, GPIO_PIN_RESET);

                SetOutput(扫描灯9, GPIO_PIN_RESET);
                SetOutput(扫描灯10, GPIO_PIN_RESET);
                SetOutput(扫描激光, GPIO_PIN_RESET);
                SetOutput(电脑显示器, GPIO_PIN_SET);
                SetOutput(书房月球灯, GPIO_PIN_SET);
                SetOutput(电视背景灯带, GPIO_PIN_SET);
                SetOutput(书房台灯, GPIO_PIN_SET);
                SetOutput(信息投影灯, GPIO_PIN_SET);
                SetOutput(卧室月球灯, GPIO_PIN_SET);
                SetOutput(客厅房顶灯带, GPIO_PIN_SET);
                SetOutput(卧室灯带, GPIO_PIN_SET);
                SetOutput(敲门锁, GPIO_PIN_SET);
                SetOutput(前场电脑复位, GPIO_PIN_SET);
                osDelay(1000);
                SetOutput(前场电脑复位, GPIO_PIN_RESET);
                gameFlags[0] = 52;
                break;
            }
            case 54://复场
            {
                SetOutput(电视信号1, GPIO_PIN_RESET);
                SetOutput(电视信号2, GPIO_PIN_RESET);
                SetOutput(电视信号3, GPIO_PIN_RESET);
                SetOutput(电视信号4, GPIO_PIN_RESET);
                SetOutput(电视信号5, GPIO_PIN_RESET);
                SetOutput(电视信号6, GPIO_PIN_RESET);
                SetOutput(接电话信号, GPIO_PIN_RESET);
                SetOutput(卧室门锁, GPIO_PIN_SET);

                SetOutput(卧室柜锁, GPIO_PIN_SET);
                SetOutput(卧室柜下灯, GPIO_PIN_RESET);
                SetOutput(卧室柜上灯, GPIO_PIN_RESET);
                SetOutput(书房门锁, GPIO_PIN_SET);
                SetOutput(摄像头灯, GPIO_PIN_RESET);
                SetOutput(书房灯带, GPIO_PIN_RESET);
                SetOutput(线路轨门锁, GPIO_PIN_SET);
                SetOutput(快递柜门锁, GPIO_PIN_SET);
                SetOutput(扫描仪门锁, GPIO_PIN_SET);
                SetOutput(信息块1, GPIO_PIN_SET);

                SetOutput(信息块2, GPIO_PIN_SET);
                SetOutput(信息块3, GPIO_PIN_SET);
                SetOutput(信息块4, GPIO_PIN_SET);
                SetOutput(信息块5, GPIO_PIN_SET);
                SetOutput(信息块6, GPIO_PIN_SET);
                SetOutput(信息块7, GPIO_PIN_SET);
                SetOutput(信息块8, GPIO_PIN_SET);
                SetOutput(信息块9, GPIO_PIN_SET);
                SetOutput(信息块10, GPIO_PIN_SET);

                SetOutput(灯带, GPIO_PIN_RESET);
                SetOutput(线路射灯, GPIO_PIN_RESET);
                SetOutput(扫描灯1, GPIO_PIN_RESET);
                SetOutput(扫描灯2, GPIO_PIN_RESET);
                SetOutput(扫描灯3, GPIO_PIN_RESET);
                SetOutput(扫描灯4, GPIO_PIN_RESET);
                SetOutput(扫描灯5, GPIO_PIN_RESET);
                SetOutput(扫描灯6, GPIO_PIN_RESET);
                SetOutput(扫描灯7, GPIO_PIN_RESET);
                SetOutput(扫描灯8, GPIO_PIN_RESET);

                SetOutput(扫描灯9, GPIO_PIN_RESET);
                SetOutput(扫描灯10, GPIO_PIN_RESET);
                SetOutput(扫描激光, GPIO_PIN_RESET);
                SetOutput(电脑显示器, GPIO_PIN_SET);
                SetOutput(书房月球灯, GPIO_PIN_RESET);
                SetOutput(电视背景灯带, GPIO_PIN_RESET);
                SetOutput(书房台灯, GPIO_PIN_RESET);
                SetOutput(信息投影灯, GPIO_PIN_RESET);
                SetOutput(卧室月球灯, GPIO_PIN_RESET);
                SetOutput(客厅房顶灯带, GPIO_PIN_RESET);
                SetOutput(卧室灯带, GPIO_PIN_RESET);

                gameFlags[0] = 52;
                break;
            }
        }
        if (gameFlags[0] >= 5 && gameFlags[0] <= 33)//4 个按钮，按不同的按钮会播放不同的视频
        {
            EventBits_t bits1 = xEventGroupWaitBits(InputEventGroup[遥控器4 / 24], TO_BIT(遥控器4), pdTRUE,
                                                    pdTRUE, 0);
            if (bits1 & TO_BIT(遥控器4)) {
                SetOutput(电视信号2, GPIO_PIN_SET);
                char *fileName = "/05.mp3";
                PlayMusicName(&MUSIC_2, fileName, strlen(fileName), 单曲停止);
                osDelay(500);
                SetOutput(电视信号2, GPIO_PIN_RESET);
            }
            EventBits_t bits2 = xEventGroupWaitBits(InputEventGroup[遥控器3 / 24], TO_BIT(遥控器3), pdTRUE,
                                                    pdTRUE, 0);
            if (bits2 & TO_BIT(遥控器3)) {
                SetOutput(电视信号3, GPIO_PIN_SET);
                char *fileName = "/06.mp3";
                PlayMusicName(&MUSIC_2, fileName, strlen(fileName), 单曲停止);
                osDelay(500);
                SetOutput(电视信号3, GPIO_PIN_RESET);
            }
            EventBits_t bits3 = xEventGroupWaitBits(InputEventGroup[遥控器2 / 24], TO_BIT(遥控器2), pdTRUE,
                                                    pdTRUE, 0);
            if (bits3 & TO_BIT(遥控器2)) {
                SetOutput(电视信号4, GPIO_PIN_SET);
                char *fileName = "/11.mp3";
                PlayMusicName(&MUSIC_2, fileName, strlen(fileName), 单曲停止);
                osDelay(500);
                SetOutput(电视信号4, GPIO_PIN_RESET);
            }
            EventBits_t bits4 = xEventGroupWaitBits(InputEventGroup[遥控器1 / 24], TO_BIT(遥控器1), pdTRUE,
                                                    pdTRUE, 0);
            if (bits4 & TO_BIT(遥控器1)) {
                SetOutput(电视信号5, GPIO_PIN_SET);
                char *fileName = "/12.mp3";
                PlayMusicName(&MUSIC_2, fileName, strlen(fileName), 单曲停止);
                osDelay(500);
                SetOutput(电视信号5, GPIO_PIN_RESET);
            }
        }
        if (gameFlags[0] >= 26 && gameFlags[0] <= 33)//等待开关
        {
            static int switchState = 0;
            EventBits_t bits1 = xEventGroupGetBits(InputEventGroup[1]);
            if ((bits1 & TO_BIT(开关面板)) && (switchState == 0)) {
                SetOutput(书房灯带, GPIO_PIN_SET);
                SetOutput(书房月球灯, GPIO_PIN_SET);
                SetOutput(书房台灯, GPIO_PIN_SET);
                SetOutput(摄像头灯, GPIO_PIN_RESET);
                switchState = 1;
            }
            if (!((bits1 & TO_BIT(开关面板)) || (switchState == 0))) {
                SetOutput(书房灯带, GPIO_PIN_RESET);
                SetOutput(书房月球灯, GPIO_PIN_RESET);
                SetOutput(书房台灯, GPIO_PIN_RESET);
                SetOutput(摄像头灯, GPIO_PIN_SET);
                switchState = 0;
            }
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