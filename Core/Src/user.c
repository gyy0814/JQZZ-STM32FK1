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

int gameFlag= {0};
SemaphoreHandle_t xGameSemaphore;

extern QueueHandle_t GameMessageQueueHandle;
extern EventGroupHandle_t InputEventGroup[(INPUT_NUM / 24) + 1];
extern EventGroupHandle_t MusicEventGroup;
extern QueueHandle_t OutputMessageQueueHandle;
bool ASCTaskHandle[9];
TaskHandle_t BlinkTaskHandle = NULL;

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
            gameFlag = newMessage.Data;
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
            case 1:
            {
                PlayMusicB("/66.mp3",单曲循环)

                SetOutput(红灯, GPIO_PIN_SET);
                SetOutput(绿灯, GPIO_PIN_SET);
                SetOutput(蓝灯, GPIO_PIN_RESET);

                SetOutput(入口门,GPIO_PIN_RESET);
                SetOutput(视频1, GPIO_PIN_SET);
                osDelay(300);
                SetOutput(视频1, GPIO_PIN_RESET);
                gameFlag++;
                break;
            }
            case 2:
            {
                if(WaitBit(门把手,pdTRUE))
                    gameFlag++;
                break;
            }
            case 3:
            {
                PlayMusicA("/31.mp3",单曲停止)
                GameTimeReset;
                gameFlag++;
                break;
            }
            case 4:
            {
                delay(14 * 1000)
                break;
            }
            case 5:
            {
                PlayMusicA("/32.mp3",单曲停止)
                gameFlag++;
                break;
            }
            case 6:
            {
                if(WaitBit(手机按钮,pdTRUE))
                    gameFlag++;
                break;
            }
            case 7:
            {
                PlayMusicA("/33.mp3",单曲停止)
                GameTimeReset;
                gameFlag++;
                break;
            }
            case 8:
            {
                delay(13*1000)
                break;
            }
            case 9:
            {
                SetOutput(行程管理电源, GPIO_PIN_SET);
                gameFlag++;
                break;
            }
            case 10:
            {
                if(WaitBit(行程管理_唤醒,pdTRUE))
                    gameFlag++;
                break;
            }
            case 11:
            {
                PlayMusicA("/34.mp3",单曲停止)
                gameFlag++;
                break;
            }
            case 12:
            {
                if(WaitBit(行程管理_1,pdTRUE))
                PlayMusicA("/35.mp3",单曲停止)
                if(WaitBit(行程管理_2,pdTRUE))
                PlayMusicA("/36.mp3",单曲停止)
                if(WaitBit(行程管理_3,pdTRUE))
                PlayMusicA("/37.mp3",单曲停止)
                if(WaitBit(行程管理_4,pdTRUE))
                {
                    PlayMusicA("/38.mp3",单曲停止)
                    gameFlag++;
                }
                break;
            }
            case 13:
            {
                if(WaitBit(行程管理_1,pdTRUE))
                PlayMusicA("/39.mp3",单曲停止)
                if(WaitBit(行程管理_3,pdTRUE))
                PlayMusicA("/40.mp3",单曲停止)
                if(WaitBit(行程管理_4,pdTRUE))
                PlayMusicA("/41.mp3",单曲停止)
                if(WaitBit(行程管理_2,pdTRUE))
                {
                    PlayMusicA("/42.mp3",单曲停止)
                    gameFlag++;
                }
                break;
            }
            case 14:
            {
                if(WaitBit(行程管理_1,pdTRUE))
                PlayMusicA("/43.mp3",单曲停止)
                if(WaitBit(行程管理_2,pdTRUE))
                PlayMusicA("/44.mp3",单曲停止)
                if(WaitBit(行程管理_4,pdTRUE))
                PlayMusicA("/45.mp3",单曲停止)
                if(WaitBit(行程管理_3,pdTRUE))
                {
                    PlayMusicA("/46.mp3",单曲停止)
                    GameTimeReset;
                    gameFlag++;
                }
                break;
            }
            case 15:
            {
                delay(39*1000);
                break;
            }
            case 16:
                gameFlag++;
                break;
            case 17://快递到达
            {
                SetOutput(门铃音效,GPIO_PIN_SET);
                osDelay(300);
                SetOutput(门铃音效,GPIO_PIN_RESET);
                gameFlag++;
                break;
            }
            case 18:
            {
                if(WaitBit(对讲_播放,pdTRUE))
                gameFlag++;
                break;
            }
            case 19:
            {
                SetOutput(快递音效,GPIO_PIN_SET);
                osDelay(300);
                SetOutput(快递音效,GPIO_PIN_RESET);
                GameTimeReset;
                gameFlag++;
                break;
            }
            case 20:
            {
                delay(14*1000)
                break;
            }
            case 21:
            {
                SetOutput(快递门,GPIO_PIN_RESET);
                gameFlag++;
                break;
            }
            case 22:
            {
                if(WaitBit(快递检测,pdTRUE))
                gameFlag++;
                break;
            }
            case 23:
            {
                PlayMusicA("/50.mp3",单曲停止)
                GameTimeReset;
                gameFlag++;
                break;
            }
            case 24:
            {
                delay(9*1000)
                break;
            }
            case 25:
            {
                SetOutput(电视视频,GPIO_PIN_SET);
                osDelay(300);
                SetOutput(电视视频,GPIO_PIN_RESET);
                osDelay(500);
                PlayMusicA("/51.mp3",单曲停止)
                GameTimeReset;
                gameFlag++;
                break;
            }
            case 26:
            {
                delay(62*1000)
                break;
            }
            case 27:
            {
                PlayMusicA("/52.mp3",单曲停止)
                GameTimeReset;
                gameFlag++;
                break;
            }
            case 28:
            {
                delay(12*1000)
                break;
            }
            case 29:
            {
                SetOutput(门铃音效,GPIO_PIN_SET);
                osDelay(300);
                SetOutput(门铃音效,GPIO_PIN_RESET);
                GameTimeReset;
                gameFlag++;
                break;
            }
            case 30:
            {
                if(WaitBit(对讲_播放,pdTRUE))
                    gameFlag++;
                break;
            }
            case 31:
            {
                SetOutput(外卖音效,GPIO_PIN_SET);
                osDelay(300);
                SetOutput(外卖音效,GPIO_PIN_RESET);
                GameTimeReset;
                gameFlag++;
                break;
            }
            case 32:
            {
                delay(15*1000)
                break;
            }
            case 33:
            {
                SetOutput(外卖柜门,GPIO_PIN_SET);
                osDelay(500);
                SetOutput(外卖柜门,GPIO_PIN_RESET);
                GameTimeReset;
                gameFlag++;
                break;
            }
            case 34:
            {
                delay(3*1000)
                break;
            }
            case 35:
            {


                SetOutput(手机视频1,GPIO_PIN_SET);
                osDelay(500);
                PlayMusicA("/81.mp3",单曲停止)
                GameTimeReset;
                gameFlag++;
                break;
            }
            case 36:
            {
                delay(74*1000)
                break;
            }
            case 37:
            {
                PlayMusicA("/55.mp3",单曲停止)
                GameTimeReset;
                gameFlag++;
                break;
            }
            case 38:
            {
                delay(36*1000)
                break;
            }
            case 39:
            {
                SetOutput(温度控制电源,GPIO_PIN_SET);
                gameFlag++;
                break;
            }
            case 40:
            {
                if(WaitBit(温度控制_2,pdTRUE))
                PlayMusicA("/56.mp3",单曲停止)
                if(WaitBit(温度控制_1,pdTRUE))
                {
                    PlayMusicA("/57.mp3",单曲停止)
                    GameTimeReset;
                    gameFlag++;
                }
                break;
            }
            case 41:
            {
                SetOutput(窗户开,GPIO_PIN_SET);
                osDelay(6000);
                PlayMusicA("/58.mp3",单曲停止)
                PlayMusicB("/67.mp3",单曲循环)
                SetOutput(窗户开,GPIO_PIN_RESET);
                SetOutput(窗户关,GPIO_PIN_RESET);
                osDelay(500);
                SetOutput(窗户关,GPIO_PIN_SET);
                osDelay(14000);
                SetOutput(窗户关,GPIO_PIN_RESET);
                gameFlag++;
                break;
            }
            case 42:
            {
                if(WaitBit(季节切换,pdFALSE))
                {
                    PlayMusicA("/59-1.mp3",单曲停止)
                    SetOutput(暖风,GPIO_PIN_SET);
                    SetOutput(红灯,GPIO_PIN_SET);
                    SetOutput(蓝灯,GPIO_PIN_RESET);
                    SetOutput(绿灯,GPIO_PIN_RESET);

                } else
                {
                    PlayMusicA("/59-2.mp3",单曲停止)
                    SetOutput(冷风,GPIO_PIN_SET);
                    SetOutput(红灯,GPIO_PIN_RESET);
                    SetOutput(蓝灯,GPIO_PIN_SET);
                    SetOutput(绿灯,GPIO_PIN_RESET);
                }
                gameFlag++;
                break;
            }
            case 43:
            {
                osDelay(4000);
                SetOutput(视频2,GPIO_PIN_SET);
                SetOutput(手机视频2,GPIO_PIN_SET);
                SetOutput(墙壁视频,GPIO_PIN_SET);
                SetOutput(墙壁视频2,GPIO_PIN_SET);
                osDelay(500);
                SetOutput(视频2,GPIO_PIN_RESET);
                SetOutput(墙壁视频,GPIO_PIN_RESET);
                SetOutput(墙壁视频2,GPIO_PIN_RESET);
                PlayMusicA("/60.mp3",单曲停止)
                GameTimeReset;
                gameFlag++;
                break;
            }
            case 44:
            {
                delay(112*1000)
                break;
            }
            case 45:
            {
                SetOutput(电箱电源, GPIO_PIN_SET);
                SetOutput(电箱射灯, GPIO_PIN_SET);
                SetOutput(电箱锁, GPIO_PIN_RESET);
                gameFlag++;
                break;
            }
            case 46:
            {
                if(WaitBit(电箱开,pdTRUE))
                {
                    PlayMusicA("/61.mp3",单曲停止)
                }
                if(WaitBit(电箱关,pdTRUE))
                {

                    if(WaitBit(电箱密码,pdFALSE))
                    {
                        PlayMusicA("/63.mp3",单曲停止)
                        GameTimeReset;
                        gameFlag++;
                    }
                    else
                    {
                        PlayMusicA("/62.mp3",单曲停止)
                    }
                }
                break;
            }
            case 47:
            {
                delay(6000)
                break;
            }
            case 48:
            {
                PlayMusicA("/64.mp3",单曲停止)
                StopMusicB;
                SetOutput(电箱电源, GPIO_PIN_RESET);
                SetOutput(电箱射灯, GPIO_PIN_RESET);
                SetOutput(红灯,GPIO_PIN_RESET);
                SetOutput(绿灯,GPIO_PIN_RESET);
                SetOutput(蓝灯,GPIO_PIN_RESET);
                SetOutput(温度控制电源, GPIO_PIN_RESET);
                SetOutput(行程管理电源, GPIO_PIN_RESET);

                GameTimeReset;
                gameFlag++;
                break;
            }
            case 49:
            {
                delay(22000);
                break;
            }
            case 50:
            {
                SetOutput(出口门,GPIO_PIN_RESET);
                gameFlag++;
                break;
            }
            case 51://结束
            {
                break;
            }
            case 52://复位
            {
                StopMusicA;
                StopMusicB;
                SetUsbMusic(&MUSIC_1);
                SetUsbMusic(&MUSIC_2);
                SetPlayMode(&MUSIC_1,单曲停止);
                SetPlayMode(&MUSIC_2,单曲循环);

                SetOutput(手机视频1,GPIO_PIN_RESET);
                SetOutput(手机视频2,GPIO_PIN_RESET);
                SetOutput(行程管理电源,GPIO_PIN_RESET);
                SetOutput(快递门,GPIO_PIN_SET);
                SetOutput(温度控制电源,GPIO_PIN_RESET);
                SetOutput(暖风,GPIO_PIN_RESET);
                SetOutput(冷风,GPIO_PIN_RESET);
                SetOutput(红灯,GPIO_PIN_SET);
                SetOutput(绿灯,GPIO_PIN_SET);
                SetOutput(蓝灯,GPIO_PIN_RESET);
                SetOutput(电箱锁,GPIO_PIN_SET);
                SetOutput(电箱射灯,GPIO_PIN_SET);
                SetOutput(电箱电源,GPIO_PIN_RESET);
                SetOutput(出口门,GPIO_PIN_SET);
                SetOutput(入口门,GPIO_PIN_SET);
                gameFlag=51;
                break;
            }
            case 53://复场
            {
                SetOutput(电箱射灯,GPIO_PIN_RESET);
                SetOutput(红灯,GPIO_PIN_RESET);
                SetOutput(绿灯,GPIO_PIN_RESET);
                SetOutput(蓝灯,GPIO_PIN_RESET);
                gameFlag=51;
                break;
            }

        }

        if(gameFlag>=21 && gameFlag<=28)
        {
            if(WaitBit(对接_重播,pdTRUE))
            {
                SetOutput(快递音效,GPIO_PIN_SET);
                osDelay(300);
                SetOutput(快递音效,GPIO_PIN_RESET);
            }
        }
        if(gameFlag>=33)
        {
            if(WaitBit(对接_重播,pdTRUE))
            {
                SetOutput(外卖音效,GPIO_PIN_SET);
                osDelay(300);
                SetOutput(外卖音效,GPIO_PIN_RESET);
            }
        }

        RunTime++;
        xSemaphoreGive(xGameSemaphore);
        osDelay(1); //等待音频播放
        static int oldGameFlag = 0;
        if(gameFlag!=oldGameFlag)
        {
            uint8_t TxBuff[5] = {0xCC,0x05,0x00,gameFlag,0xFF};
            HAL_UART_Transmit(&huart1,TxBuff,5,HAL_MAX_DELAY);
            oldGameFlag = gameFlag;
        }
    }
}
