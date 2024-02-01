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

int gameFlag;
SemaphoreHandle_t xGameSemaphore;

extern QueueHandle_t GameMessageQueueHandle;
extern EventGroupHandle_t InputEventGroup[(INPUT_NUM / 24) + 1];
extern EventGroupHandle_t MusicEventGroup;
extern QueueHandle_t OutputMessageQueueHandle;
void GameInit(void)
{
        xGameSemaphore = xSemaphoreCreateMutex();
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
        xSemaphoreTake(xGameSemaphore, portMAX_DELAY);
        switch (gameFlag) {
            case 1://开入口平移门
            {
                ResetPin(入口平移门关);
                ResetPin(入口平移门开);
                osDelay(300);
                SetPin(入口平移门开);
                GameTimeReset;
                gameFlag++;
                break;
            }
            case 2:
            {
                delay(15*1000)
                break;
            }
            case 3:
            {
                ResetPin(入口平移门关);
                ResetPin(入口平移门开);
                PlayMusicA("/49.mp3",单曲停止);
                SetPin(蓝色聚光灯);
                osDelay(1300);
                PlayMusicA("/47.mp3",单曲停止);
                gameFlag=0;
                break;
            }
            case 4://关入口平移门
            {

                ResetPin(蓝色聚光灯);
                ResetPin(入口平移门关);
                ResetPin(入口平移门开);
                PlayMusicB("/48.mp3",单曲停止);
                osDelay(300);
                SetPin(入口平移门关);
                GameTimeReset;
                gameFlag++;
                break;
            }
            case 5:
            {
                delay(19*1000)
                break;
            }
            case 6:
            {
                ResetPin(入口平移门关);
                ResetPin(入口平移门开);
                PlayMusicA("/49.mp3",单曲停止)
                SetPin(婴儿床灯);
                osDelay(1300);
                PlayMusicB("/50.mp3",单曲循环)
                gameFlag++;
                break;
            }
            case 7:
            {
                if(WaitBit(动物按钮_鸡,pdTRUE))
                {
                    PlayMusicA("/51.mp3",单曲停止)
                }
                if(WaitBit(动物按钮_猫,pdTRUE))
                {
                    PlayMusicA("/52.mp3",单曲停止)
                }
                if(WaitBit(动物按钮_狗,pdTRUE))
                {
                    PlayMusicA("/53.mp3",单曲停止)
                }
                if(WaitBit(动物按钮_蛙,pdTRUE))
                {
                    PlayMusicA("/54.mp3",单曲停止)
                }
                if(WaitBit(动物按钮_牛,pdTRUE))
                {
                    PlayMusicA("/55.mp3",单曲停止)
                }
                if(WaitBit(动物摆放,pdTRUE))
                {
                    PlayMusicA("/56.mp3",单曲停止)
                    GameTimeReset;
                    gameFlag++;
                }
                break;
            }
            case 8:
            {
                delay(41*1000)
                break;
            }
            case 9:
            {
                ResetPin(婴儿床灯);
                PlayMusicA("/49.mp3",单曲停止)
                SetPin(游戏灯);
                PlayMusicB("/57.mp3",单曲循环)
                gameFlag++;
                break;
            }
            case 10:
            {
                if(WaitBit(游戏过关,pdTRUE))
                {
                    gameFlag++;
                }
                break;
            }
            case 11:
            {
                PlayMusicA("/58.mp3",单曲停止)
                GameTimeReset;
                gameFlag++;
                break;
            }
            case 12:
            {
                delay(34*1000)
                break;
            }
            case 13:
            {
                ResetPin(游戏灯);
                PlayMusicA("/49.mp3",单曲停止)
                SetPin(酒吧灯);
                PlayMusicB("/59.mp3",单曲循环)
                gameFlag++;
                break;
            }
            case 14:
            {
                if(WaitBit(酒吧过关,pdTRUE))
                {
                    gameFlag++;
                }
                break;
            }
            case 15:
            {
                PlayMusicA("/60.mp3",单曲停止)
                GameTimeReset;
                gameFlag++;
                break;
            }
            case 16:
            {
                delay(62*1000)
                break;
            }
            case 17:
            {
                ResetPin(酒吧灯);
                PlayMusicA("/49.mp3",单曲停止)
                SetPin(医院灯);
                SetPin(片子灯);
                PlayMusicB("/61.mp3",单曲循环)
                gameFlag++;
                break;
            }
            case 18:
            {
                if(WaitBit(医院过关,pdTRUE))
                {
                    gameFlag++;
                }
                break;
            }
            case 19:
            {
                PlayMusicA("/62.mp3",单曲停止)
                GameTimeReset;
                gameFlag++;
                break;
            }
            case 20:
            {
                delay(41*1000)
                break;
            }
            case 21:
            {
                ResetPin(医院灯);
                ResetPin(片子灯);
                PlayMusicA("/49.mp3",单曲停止)
                SetPin(出口灯);
                osDelay(1300);
                PlayMusicB("/63.mp3",单曲循环)
                PlayMusicA("/64.mp3",单曲停止)
                gameFlag++;
                break;
            }
            case 22:
            {
                if(WaitBit(敲门,pdTRUE))
                {
                    gameFlag++;
                }
                break;
            }
            case 23:
            {
                ResetPin(出口灯);
                PlayMusicB("/65.mp3",单曲停止)
                PlayMusicA("/66-1.mp3",单曲停止)
                ResetPin(出口平移门开);
                ResetPin(出口平移门关);
                osDelay(300);
                SetPin(出口平移门开);
                GameTimeReset;
                gameFlag++;
                break;
            }
            case 24:
            {
                delay(15*1000)
                break;
            }
            case 25:
            {
                ResetPin(出口平移门开);
                ResetPin(出口平移门关);
                PlayMusicA("/49.mp3",单曲停止)
                SetPin(蓝色聚光灯2);
                osDelay(1300);
                PlayMusicA("/66.mp3",单曲停止)
                gameFlag=0;
                break;
            }
            case 26:
            {
                ResetPin(蓝色聚光灯2);
                PlayMusicB("/67-1.mp3",单曲循环)
                PlayMusicA("/67.mp3",单曲停止)
                ResetPin(出口平移门开);
                ResetPin(出口平移门关);
                osDelay(300);
                SetPin(出口平移门关);
                GameTimeReset;
                gameFlag++;
                break;
            }
            case 27:
            {
                delay(15*1000)
                break;
            }
            case 28:
            {
                SetPin(实验室灯);
                ResetPin(出口平移门开);
                ResetPin(出口平移门关);
                PlayMusicB("/68.mp3",单曲循环)
                PlayMusicA("/69.mp3",单曲停止)
                GameTimeReset;
                gameFlag++;
                break;
            }
            case 29:
            {
                delay(16*1000)
                break;
            }
            case 30:
            {
                SetPin(准备升空视频);
                osDelay(300);
                ResetPin(准备升空视频);
                GameTimeReset;
                gameFlag++;
                break;
            }
            case 31://改为视频10时长
            {
                delay(16*1000)
                break;
            }
            case 32:
            {
                PlayMusicA("/70.mp3",单曲停止)
                GameTimeReset;
                gameFlag++;
                break;
            }
            case 33://改为视频10时长
            {
                delay(10*1000)
                break;
            }
            case 34:
            {
                SetPin(实验室台灯);
                gameFlag++;
                break;
            }
            case 35:
            {
                if(WaitBit(储存库过关,pdTRUE))
                {
                    gameFlag++;
                }
                break;
            }
            case 36:
            {
                //开启音效
                //打开金属柜门
                GameTimeReset;
                gameFlag++;
                break;
            }
            case 37:
            {
                //等待打开柜门时间
                delay(10*1000)
                break;
            }
            case 38:
            {
                PlayMusicA("/71.mp3",单曲停止)
                GameTimeReset;
                gameFlag++;
                break;
            }
            case 39:
            {
                delay(13*1000)
                break;
            }
            case 40:
            {
                SetPin(进入飞船视频);
                osDelay(300);
                ResetPin(进入飞船视频);
                GameTimeReset;
                gameFlag++;
                break;
            }
            case 41:
            {
                //改为视频时长
                delay(13*1000)
                break;
            }
            case 42:
            {
                SetPin(柜子白光);
                gameFlag++;
                break;
            }
            case 43:
            {
                if(WaitBit(密码器,pdTRUE))
                {
                    gameFlag++;
                }
                break;
            }
            case 44:
            {
                ResetPin(贝塔锁);
                gameFlag++;
                break;
            }
            case 45:
            {
                if(!WaitBit(记忆卡拿开,pdFALSE))
                {
                    gameFlag++;
                }
                break;
            }
            case 47:
            {
                PlayMusicA("/72.mp3",单曲停止)
                gameFlag++;

                break;
            }
        }
        RunTime++;
        xSemaphoreGive(xGameSemaphore);
        osDelay(1); //等待音频播放
        static int oldGameFlag = 19;
        if(gameFlag!=oldGameFlag)
        {
            uint8_t TxBuff[5] = {0xCC,0x05,0x00,gameFlag,0xFF};
            HAL_UART_Transmit(&huart1,TxBuff,5,HAL_MAX_DELAY);
            oldGameFlag = gameFlag;
        }
    }
}
