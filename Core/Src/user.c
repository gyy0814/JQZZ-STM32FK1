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

        if (gameFlags[0]==0)//第一次按下遥控
        {
            EventBits_t bits = xEventGroupWaitBits(InputEventGroup[0], TO_BIT(艾琳开始),pdTRUE,pdTRUE,0);
            if(bits&TO_BIT(艾琳开始))
            {
                gameFlags[0]++;
            }
        }
        if (gameFlags[0]==1)//艾琳开始音频
        {
            SetOutput(电视背景灯带,GPIO_PIN_SET);
            GameTimeReset;
            if (GameDelay(&RunTime,13000))
            {
                SetOutput(电视背景灯带,GPIO_PIN_RESET);
                gameFlags[0]++;
            }
        }
        if (gameFlags[0]==2)//播放bgm开场,播放语音,这就是王晗雨的家了,开灯
        {
            char *fileName1="/BGM/02.mp3";
            PlayMusicName(&MUSIC_1,fileName1, strlen(fileName1),单曲循环);

            char *fileName2="/BGM/03.mp3";
            PlayMusicName(&MUSIC_2,fileName2, strlen(fileName2),单曲停止);
            SetOutput(电视背景灯带,GPIO_PIN_SET);
            SetOutput(客厅房顶灯带,GPIO_PIN_SET);
            gameFlags[0]++;
        }
        if (gameFlags[0]==3)//等待检测遥控器1
        {
            EventBits_t bits = xEventGroupWaitBits(InputEventGroup[0], TO_BIT(遥控器1),pdTRUE,pdTRUE,0);
            if(bits&TO_BIT(遥控器1)) {
                gameFlags[0]++;
            }
        }
        if (gameFlags[0]==4)//检测遥控器1 电视开启播放待机画面雪花
        {
                SetOutput(电视信号1,GPIO_PIN_SET);
                GameTimeReset;
                while (GameDelay(&RunTime,1000)) {
                    SetOutput(电视信号1, GPIO_PIN_RESET);
                    gameFlags[0]++;
                }
        }
        if (gameFlags[0]>=5&&gameFlags[0]<=6)//4 个按钮，按不同的按钮会播放不同的视频
        {
            EventBits_t bits1 = xEventGroupWaitBits(InputEventGroup[0], TO_BIT(遥控器2),pdTRUE,pdTRUE,0);
            if(bits1&TO_BIT(遥控器2))
            {
                SetOutput(电视信号2,GPIO_PIN_SET);
                char *fileName="/BGM/05.mp3";
                PlayMusicName(&MUSIC_2,fileName, strlen(fileName),单曲停止);
                while (GameDelay(&RunTime,1000))
                {
                    SetOutput(电视信号2,GPIO_PIN_RESET);
                }
            }
            EventBits_t bits2 = xEventGroupWaitBits(InputEventGroup[0], TO_BIT(遥控器3),pdTRUE,pdTRUE,0);
            if(bits2&TO_BIT(遥控器3))
            {
                SetOutput(电视信号3,GPIO_PIN_SET);
                char *fileName="/BGM/06.mp3";
                PlayMusicName(&MUSIC_2,fileName, strlen(fileName),单曲停止);
                while (GameDelay(&RunTime,1000))
                {
                    SetOutput(电视信号3,GPIO_PIN_RESET);
                }
            }
            EventBits_t bits3 = xEventGroupWaitBits(InputEventGroup[0], TO_BIT(遥控器4),pdTRUE,pdTRUE,0);
            if(bits3&TO_BIT(遥控器4))
            {
                SetOutput(电视信号4,GPIO_PIN_SET);
                char *fileName="/BGM/11.mp3";
                PlayMusicName(&MUSIC_2,fileName, strlen(fileName),单曲停止);
                while (GameDelay(&RunTime,1000))
                {
                    SetOutput(电视信号4,GPIO_PIN_RESET);
                }
            }
            EventBits_t bits4 = xEventGroupWaitBits(InputEventGroup[0], TO_BIT(遥控器5),pdTRUE,pdTRUE,0);
            if(bits4&TO_BIT(遥控器5))
            {
                SetOutput(电视信号5,GPIO_PIN_SET);
                char *fileName="/BGM/12.mp3";
                PlayMusicName(&MUSIC_2,fileName, strlen(fileName),单曲停止);
                while (GameDelay(&RunTime,1000))
                {
                    SetOutput(电视信号5,GPIO_PIN_RESET);
                }

            }
        }
        if (gameFlags[0]==5)//等待打电话
        {
            EventBits_t bits1 = xEventGroupWaitBits(InputEventGroup[0], TO_BIT(打电话),pdTRUE,pdTRUE,0);
            if(bits1&TO_BIT(打电话))
            {
                gameFlags[0]++;
            }
        }
        if (gameFlags[0]==6)//等待卧室密码器
        {
            EventBits_t bits1 = xEventGroupWaitBits(InputEventGroup[0], TO_BIT(卧室密码器),pdTRUE,pdTRUE,0);
            if(bits1&TO_BIT(卧室密码器))
            {
                char *fileName="/BGM/14.mp3";
                PlayMusicName(&MUSIC_1,fileName, strlen(fileName),单曲循环);
                SetOutput(卧室门锁,GPIO_PIN_SET);
                SetOutput(卧室月球灯,GPIO_PIN_SET);
                SetOutput(卧室灯带,GPIO_PIN_SET);
                gameFlags[0]++;
            }
        }
        if (gameFlags[0]==7)//卧室密码器
        {
                char *fileName="/BGM/14.mp3";
                PlayMusicName(&MUSIC_1,fileName, strlen(fileName),单曲循环);
                SetOutput(卧室门锁,GPIO_PIN_SET);
                SetOutput(卧室月球灯,GPIO_PIN_SET);
                SetOutput(卧室灯带,GPIO_PIN_SET);
                gameFlags[0]++;
        }
        if (gameFlags[0]==8)//延时触发语音
        {
            char *fileName="/BGM/15.mp3";
            while (GameDelay(&RunTime,40000))
            {
                PlayMusicName(&MUSIC_2,fileName, strlen(fileName),单曲停止);
                gameFlags[0]++;
            }
        }
        if (gameFlags[0]==9)//等待卧室抽屉打开
        {
            EventBits_t bits1 = xEventGroupWaitBits(InputEventGroup[0], TO_BIT(卧室抽屉打开),pdTRUE,pdTRUE,0);
            if(bits1&TO_BIT(卧室抽屉打开))
            {
                    gameFlags[0]++;
            }
        }
        if (gameFlags[0]==10)//卧室抽屉打开
        {
                char *fileName="/BGM/16.mp3";
                PlayMusicName(&MUSIC_2,fileName, strlen(fileName),单曲停止);
                while (GameDelay(&RunTime,40000))
                {
                    SetOutput(卧室柜上灯,GPIO_PIN_SET);
                    gameFlags[0]++;
                }
        }
        if (gameFlags[0]==11)//等待录取通知
        {
            EventBits_t bits1 = xEventGroupWaitBits(InputEventGroup[0], TO_BIT(录取通知),pdTRUE,pdTRUE,0);
            if(bits1&TO_BIT(录取通知))
            {
                    gameFlags[0]++;
            }
        }
        if (gameFlags[0]==12)//录取通知
        {
                SetOutput(接电话信号,GPIO_PIN_SET);
                while (GameDelay(&RunTime,1000))
                {
                    SetOutput(接电话信号,GPIO_PIN_RESET);
                    gameFlags[0]++;
                }
        }
        if (gameFlags[0]==13)//延时开柜子
        {
            while (GameDelay(&RunTime,30000))
            {
                char *fileName="/BGM/4.mp3";
                PlayMusicName(&MUSIC_2,fileName, strlen(fileName),单曲停止);
                SetOutput(卧室柜锁,GPIO_PIN_SET);
                SetOutput(卧室柜下灯,GPIO_PIN_SET);
                gameFlags[0]++;
            }
        }
        if (gameFlags[0]==14)//等待记忆卡拿走
        {
            EventBits_t bits1 = xEventGroupWaitBits(InputEventGroup[0], TO_BIT(记忆卡拿走),pdTRUE,pdTRUE,0);
            if(bits1&TO_BIT(记忆卡拿走))
            {
                gameFlags[0]++;
            }
        }
        if (gameFlags[0]==15)//记忆卡拿走
        {
                char *fileName="/BGM/18.mp3";
                PlayMusicName(&MUSIC_2,fileName, strlen(fileName),单曲停止);
                gameFlags[0]++;
        }
        if (gameFlags[0]==16)//等待记忆卡刷卡1
        {
            EventBits_t bits1 = xEventGroupWaitBits(InputEventGroup[0], TO_BIT(记忆卡刷卡1),pdTRUE,pdTRUE,0);
            if(bits1&TO_BIT(记忆卡刷卡1))
            {
                gameFlags[0]++;
            }
        }
        if (gameFlags[0]==17)//记忆卡刷卡1
        {
            SetOutput(电视信号6,GPIO_PIN_SET);
            while (GameDelay(&RunTime,1000))
            {
                SetOutput(电视信号6,GPIO_PIN_RESET);
            }
        }
        if (gameFlags[0]==18)//播放
        {

        }
        if (gameFlags[0]==14)//播放
        {

        }
        if (gameFlags[0]==15)//播放
        {

        }
        if (gameFlags[0]==16)//播放
        {

        }
        if (gameFlags[0]==17)//播放
        {

        }
        if (gameFlags[0]==18)//播放
        {

        }
        if (gameFlags[0]==19)//播放
        {

        }
        if (gameFlags[0]==20)//播放
        {

        }
        if (gameFlags[0]==21)//播放
        {

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
