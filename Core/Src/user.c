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

void GameInit(void)
{
    for(int i=0;i<GameTaskNum;i++)
        xGameSemaphore[i] = xSemaphoreCreateMutex();
}

void StartGameFlagTask(void const *argument)
{
    GameMessage newMessage;
    for(;;)
    {
        if(xQueueReceive(GameMessageQueueHandle, &newMessage, portMAX_DELAY) == pdTRUE)
        {
            xSemaphoreTake(xGameSemaphore[newMessage.num], portMAX_DELAY);
            gameFlags[newMessage.num] = newMessage.Data;
            xSemaphoreGive(xGameSemaphore[newMessage.num]);
        }
    }
}


#define GameTimeReset RunTime = 0
bool GameDelay(int *pvRunTime,int waitTime)
{
    if(waitTime<*pvRunTime)
    {
        *pvRunTime=0;
        return true;
    }
    return false;
}

void StartGameTask(void const *argument)
{
    int RunTime = 0;
    for(;;)
    {
        xSemaphoreTake(xGameSemaphore[0], portMAX_DELAY);
        switch (gameFlags[0]) {
            case 0://第一次按下遥控
            {
                EventBits_t bits = xEventGroupWaitBits(InputEventGroup[0], TO_BIT(艾琳开始),pdTRUE,pdTRUE,0);
                if(bits&TO_BIT(艾琳开始))
                {
                    gameFlags[0]++;
                }
//
//                if(xEventGroupWaitBits(InputEventGroup[0], TO_BIT(艾琳开始),pdTRUE,pdTRUE,0)==pdPASS)
//                {
//                    //打开艾琳门口的语音
//                    gameFlags[0]++;
//                }
                break;
            }
            case 1: // 艾琳开始音频
            {
                if (GameDelay(&RunTime,13000))
                    gameFlags[0]++;
                break;
            }
            case 2: // 播放bgm开场
            {
                char *fileName="/BGM/02.mp3";
                PlayMusicName(&MUSIC_1,fileName, strlen(fileName),单曲循环);
                gameFlags[0]++;
                break;
            }
            case 3:// 播放语音,这就是王晗雨的家了
            {
                char *fileName="/BGM/03.mp3";
                PlayMusicName(&MUSIC_2,fileName, strlen(fileName),单曲停止);

                if (GameDelay(&RunTime,98000))
                    gameFlags[0]++;
                break;
            }
            case 4:// 打开语音识别
            {
                gameFlags[0]++;
                break;
            }
            case 5:// 等待语音识别1
            {

                break;
            }
            case 6:// 关闭语音识别 播放语音2
            {
                char *fileName="/BGM/002.mp3";
                PlayMusicName(&MUSIC_1,fileName, strlen(fileName),单曲停止);
                GameTimeReset;
                gameFlags[0]++;
                break;
            }
            case 7://等待播放结束
            {
                if (GameDelay(&RunTime,55000))
                    gameFlags[0]++;
                break;
            }
            case 8:// 打开语音识别
            {
                gameFlags[0]++;
                break;
            }
            case 9:// 等待语音识别2
            {
                break;
            }
            case 16:
            {
                gameFlags[0]++;
                break;
            }
            case 17://等待钥匙
            {

                break;
            }
            case 18://开门
            {

                gameFlags[0]++;
                break;
            }
            case 19: //空
            {
                break;
            }
            case 20: //复位
            {

                gameFlags[0] = 19;
            }
        }
        RunTime++;
        xSemaphoreGive(xGameSemaphore[0]);
        osDelay(1); //等待音频播放
        static int oldGameFlag = 100;
        if(gameFlags[0]!=oldGameFlag)
        {
            uint8_t TxBuff[5] = {0xCC,0x05,0x00,gameFlags[0],0xFF};
            HAL_UART_Transmit(&huart1,TxBuff,5,HAL_MAX_DELAY);
            oldGameFlag = gameFlags[0];
        }
    }
}
