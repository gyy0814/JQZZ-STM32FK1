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

int gameFlag= {19};
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
                SetOutput(入口门,GPIO_PIN_RESET);
                SetOutput(视频1, GPIO_PIN_SET);
                osDelay(300);
                SetOutput(视频1, GPIO_PIN_RESET);
                gameFlag++;
            }
            case 2:
            {
                WaitBit(门把手,pdTRUE)
                    gameFlag++;
            }
            case 3:
            {
                PlayMusicA("/31.mp3",单曲停止)
                GameTimeReset;
                gameFlag++;
            }
            case 4:
            {
                delay(12 * 1000)
            }
            case 5:
            {
                PlayMusicA("/32.mp3",单曲停止)
                gameFlag++;
            }
            case 6:
            {
                WaitBit(手机按钮,pdTRUE)
                    gameFlag++;
            }
            case 7:
            {
                PlayMusicA("/33.mp3",单曲停止)
                GameTimeReset;
                gameFlag++;
            }
            case 8:
            {
                delay(13*1000)
            }
            case 9:
            {
                SetOutput(行程管理电源, GPIO_PIN_SET);
                gameFlag++;
            }
            case 10:
            {
                WaitBit(行程管理_唤醒,pdTRUE)
                    gameFlag++;
            }
            case 11:
            {
                PlayMusicA("/34.mp3",单曲停止)
                gameFlag++;
            }
            case 12:
            {
                WaitBit(行程管理_1,pdTRUE)
                PlayMusicA("/35.mp3",单曲停止)
                WaitBit(行程管理_2,pdTRUE)
                PlayMusicA("/36.mp3",单曲停止)
                WaitBit(行程管理_3,pdTRUE)
                PlayMusicA("/37.mp3",单曲停止)
                WaitBit(行程管理_4,pdTRUE)
                {
                    PlayMusicA("/38.mp3",单曲停止)
                    gameFlag++;
                }
            }
            case 13:
            {
                WaitBit(行程管理_1,pdTRUE)
                PlayMusicA("/39.mp3",单曲停止)
                WaitBit(行程管理_3,pdTRUE)
                PlayMusicA("/40.mp3",单曲停止)
                WaitBit(行程管理_4,pdTRUE)
                PlayMusicA("/41.mp3",单曲停止)
                WaitBit(行程管理_2,pdTRUE)
                {
                    PlayMusicA("/42.mp3",单曲停止)
                    gameFlag++;
                }
            }
            case 14:
            {
                WaitBit(行程管理_1,pdTRUE)
                PlayMusicA("/43.mp3",单曲停止)
                WaitBit(行程管理_2,pdTRUE)
                PlayMusicA("/44.mp3",单曲停止)
                WaitBit(行程管理_4,pdTRUE)
                PlayMusicA("/45.mp3",单曲停止)
                WaitBit(行程管理_3,pdTRUE)
                {
                    PlayMusicA("/46.mp3",单曲停止)
                    GameTimeReset;
                    gameFlag++;
                }
            }
            case 15:
            {
                delay(39*1000);
            }
            case 17://快递到达
            {
                SetOutput(门铃音效,GPIO_PIN_SET);
                osDelay(300);
                SetOutput(门铃音效,GPIO_PIN_RESET);
                gameFlag++;
            }
            case 18:
            {
                WaitBit(对讲_播放,pdTRUE)
                gameFlag++;
            }
            case 19:
            {
                SetOutput(快递音效,GPIO_PIN_SET);
                osDelay(300);
                SetOutput(快递音效,GPIO_PIN_RESET);
                GameTimeReset;
                gameFlag++;
            }
            case 20:
            {
                delay(14*1000)
            }
            case 21:
            {
                SetOutput(快递门,GPIO_PIN_RESET);
                gameFlag++;
            }
            case 22:
            {
                WaitBit(快递检测,pdTRUE)
                gameFlag++;
            }
            case 23:
            {
                PlayMusicA("/50.mp3",单曲停止)
                GameTimeReset;
                gameFlag++;
            }
            case 24:
            {
                delay(8*1000)
            }
            case 25:
            {
                PlayMusicA("/51.mp3",单曲停止)
                GameTimeReset;
                gameFlag++;
            }
            case 26:
            {
                delay(62*1000)
            }
            case 27:
            {
                PlayMusicA("/52.mp3",单曲停止)
                GameTimeReset;
                gameFlag++;
            }
            case 28:
            {
                delay(12*1000)
            }
            case 29:
            {
                SetOutput(门铃音效,GPIO_PIN_SET);
                osDelay(300);
                SetOutput(门铃音效,GPIO_PIN_RESET);
                GameTimeReset;
                gameFlag++;
            }
            case 30:
            {
                WaitBit(对讲_播放,pdTRUE)
                    gameFlag++;
            }
            case 31:
            {
                SetOutput(外卖音效,GPIO_PIN_SET);
                osDelay(300);
                SetOutput(外卖音效,GPIO_PIN_RESET);
                GameTimeReset;
                gameFlag++;
            }
            case 32:
            {
                delay(12*1000)
            }
            case 33:
            {
                SetOutput(外卖柜门,GPIO_PIN_SET);
                osDelay(500);
                SetOutput(外卖柜门,GPIO_PIN_RESET);
                GameTimeReset;
                gameFlag++;
            }
            case 34:
            {
                delay(3*1000)
            }
            case 35:
            {
                SetOutput(手机视频1,GPIO_PIN_SET);
                PlayMusicA("/sp9.mp3",单曲停止)
                GameTimeReset;
                gameFlag++;
            }
            case 36:
            {
                delay(72*1000)
            }
            case 37:
            {
                PlayMusicA("/55.mp3",单曲停止)
                GameTimeReset;
                gameFlag++;
            }
            case 38:
            {
                delay(35*1000)
            }
            case 39:
            {
                SetOutput(温度控制电源,GPIO_PIN_SET);
                gameFlag++;
            }
            case 40:
            {
                WaitBit(温度控制_1,pdTRUE)
                PlayMusicA("/56.mp3",单曲停止)
                WaitBit(温度控制_2,pdTRUE)
                {
                    PlayMusicA("/57.mp3",单曲停止)
                    GameTimeReset;
                    gameFlag++;
                }
            }
            case 41:
            {
                SetOutput(窗户推杆,GPIO_PIN_SET);
                osDelay(3000);
                PlayMusicA("/58.mp3",单曲停止)
                //更换BGM
                SetOutput(窗户推杆,GPIO_PIN_RESET);
                osDelay(2000);
                gameFlag++;
            }
            case 42:
            {
                WaitBit(季节切换,pdFALSE)
                {
                    PlayMusicA("/59-1.mp3",单曲停止)
                    SetOutput(暖风,GPIO_PIN_SET);
                    SetOutput(红灯,GPIO_PIN_SET);
                } else
                {
                    PlayMusicA("/59-2.mp3",单曲停止)
                    SetOutput(冷风,GPIO_PIN_SET);
                    SetOutput(蓝灯,GPIO_PIN_SET);
                }
                gameFlag++;
            }
            case 43:
            {
                osDelay(2000);
                SetOutput(视频2,GPIO_PIN_SET);
                SetOutput(手机视频2,GPIO_PIN_SET);
                SetOutput(墙壁视频,GPIO_PIN_SET);
                osDelay(300);
                SetOutput(视频2,GPIO_PIN_RESET);
                SetOutput(手机视频2,GPIO_PIN_RESET);
                SetOutput(墙壁视频,GPIO_PIN_RESET);
                PlayMusicA("/60.mp3",单曲停止)
                GameTimeReset;
                gameFlag++;
            }
            case 44:
            {
                delay(112*1000)
            }
            case 45:
            {
                SetOutput(电箱电源, GPIO_PIN_SET);
                SetOutput(电箱射灯, GPIO_PIN_SET);
                SetOutput(电箱锁, GPIO_PIN_SET);
                gameFlag++;
            }
            case 46:
            {
                WaitBit(电箱开,pdTRUE)
                {
                    PlayMusicA("/61.mp3",单曲停止)
                }
                WaitBit(电箱关,pdTRUE)
                {

                    WaitBit(电箱密码,pdFALSE)
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
            }
            case 47:
            {
                delay(6000);
            }
            case 48:
            {
                PlayMusicA("/64.mp3",单曲停止)
                GameTimeReset;
                gameFlag++;
            }
            case 49:
            {
                delay(22000);
            }
            case 50:
            {
                SetOutput(出口门,GPIO_PIN_RESET);
                gameFlag++;
            }
            case 51://结束
            {

            }
            case 52://复位
            {
                SetOutput(行程管理电源,GPIO_PIN_RESET);
                SetOutput(快递门,GPIO_PIN_SET);
                SetOutput(温度控制电源,GPIO_PIN_RESET);
                SetOutput(窗户推杆,GPIO_PIN_RESET);
                SetOutput(暖风,GPIO_PIN_RESET);
                SetOutput(冷风,GPIO_PIN_RESET);
                SetOutput(红灯,GPIO_PIN_RESET);
                SetOutput(蓝灯,GPIO_PIN_RESET);
                SetOutput(电箱锁,GPIO_PIN_SET);
                SetOutput(电箱射灯,GPIO_PIN_RESET);
                SetOutput(电箱电源,GPIO_PIN_RESET);
            }
            case 53://复场
            {
                SetOutput(出口门,GPIO_PIN_SET);
            }

        }

        if(gameFlag>=21 && gameFlag<=30)
        {
            WaitBit(对接_重播,pdTRUE)
            {
                SetOutput(快递音效,GPIO_PIN_SET);
                osDelay(300);
                SetOutput(快递音效,GPIO_PIN_RESET);
            }
        }
        if(gameFlag>=33)
        {
            WaitBit(对接_重播,pdTRUE)
            {
                SetOutput(外卖音效,GPIO_PIN_SET);
                osDelay(300);
                SetOutput(外卖音效,GPIO_PIN_RESET);
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
