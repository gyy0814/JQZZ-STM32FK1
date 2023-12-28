//
// Created by 26913 on 2023/12/14.
//

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "user.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "cmsis_os.h"
#include "gpio.h"

extern EventGroupHandle_t InputEventGroup[(INPUT_NUM / 32) + 1];
extern EventGroupHandle_t MusicEventGroup;
extern QueueHandle_t OutputMessageQueueHandle;
bool ASCTaskHandle[8];

void OpenDoor(void *pvParameters) {
    int *pNum = (int *) pvParameters;
    int Anum = *pNum;
    int Dnum;
    switch (Anum) {
        case 0:
            Dnum = 0;
            break;
        case 1:
            Dnum = 1;
            break;
        case 2:
            Dnum = 4;
            break;
        case 3:
            Dnum = 6;
            break;
        case 4:
            Dnum = 7;
            break;
        case 5:
            Dnum = 2;
            break;
        case 6:
            Dnum = 3;
            break;
        default:
            Dnum = Anum;
            break;
    }
    SetOutput(Dnum, GPIO_PIN_SET);
    osDelay(4000);
    SetOutput(Dnum, GPIO_PIN_RESET);
    ASCTaskHandle[Anum] = false;
    vTaskDelete(NULL);
}

void StartASCTask(void const *argument) {
    EventBits_t waitBits = TO_BIT(0) | TO_BIT(1) | TO_BIT(2) | TO_BIT(3) | TO_BIT(4);
    for (;;) {
        EventBits_t bits = xEventGroupWaitBits(InputEventGroup[0], waitBits, pdFALSE, pdFALSE, portMAX_DELAY);
        for (int i = 0; i < 5; i++) {
            if (bits & TO_BIT(i)) {
                if (!ASCTaskHandle[i]) {
                    ASCTaskHandle[i] = true;
                    int ASCNum = i;
                    xTaskCreate(OpenDoor, "Opendoor", 128, &ASCNum, 1, NULL);
                }
            }
        }
    }
}

void BlinkLight(void *argument) {
    for (;;) {
        SetOutput(煤气灯输出, GPIO_PIN_SET);
        osDelay(300);
        SetOutput(煤气灯输出, GPIO_PIN_RESET);
        osDelay(300);
        SetOutput(煤气灯输出, GPIO_PIN_SET);
        osDelay(300);
        SetOutput(煤气灯输出, GPIO_PIN_RESET);
        osDelay(1000);
        SetOutput(煤气灯输出, GPIO_PIN_SET);
        osDelay(300);
        SetOutput(煤气灯输出, GPIO_PIN_RESET);
        osDelay(1000);
    }
}

void StartGameTask(void const *argument) {
    int gameFlag = 0;
    TaskHandle_t BlinkTaskHandle = NULL;
    for (;;) {
        switch (gameFlag) {
            case 0: // 等待钢琴输入
            {
                EventBits_t bits = xEventGroupWaitBits(InputEventGroup[0], TO_BIT(钢琴输入), pdFALSE, pdTRUE,
                                                       0);  // 等待1秒
                if (bits & TO_BIT(钢琴输入)) {
                    gameFlag++;
                }
                break;
            }
            case 1: // 开启水幕投影 关闭灯光
            {
                //开水幕机
                SetOutput(水幕开,GPIO_PIN_SET);
                osDelay(500);
                SetOutput(水幕开,GPIO_PIN_RESET);

                osDelay(500);

                //开投影
                SetOutput(视频播放,GPIO_PIN_SET);


                // 关场灯
                SetOutput(场灯(大厅灯光), GPIO_PIN_SET);
                gameFlag++;
                break;
            }
            case 2://等待插香
            {
                EventBits_t bits = xEventGroupWaitBits(InputEventGroup[0], TO_BIT(插香输入), pdFALSE, pdTRUE,
                                                       0);  // 等待1秒
                if (bits & TO_BIT(插香输入)) {
                    gameFlag++;
                }
                break;
            }
            case 3://开卧室门 留声机播放音频1 抽屉锁断电
            {
                //关水幕机
                SetOutput(水幕关,GPIO_PIN_SET);
                osDelay(500);
                SetOutput(水幕关,GPIO_PIN_RESET);

                // 开卧室门
                if (!ASCTaskHandle[5]) {
                    ASCTaskHandle[5] = true;
                    int ASCNum = 5;
                    xTaskCreate(OpenDoor, "Opendoor", 128, &ASCNum, 1, NULL);
                }

                //恐怖音效1
                char* MusicName="/BGM/001.mp3";
                PlayMusicName(&MUSIC_1,MusicName, strlen(MusicName),单曲循环);

                // 留声机播放音频1

                // 抽屉锁断电
                SetOutput(抽屉锁,GPIO_PIN_SET);
                gameFlag++;
                break;
            }
            case 4://等待抽屉打开检测
            {
                EventBits_t bits = xEventGroupWaitBits(InputEventGroup[0], TO_BIT(抽屉打开输入), pdFALSE, pdTRUE,
                                                       0);  // 等待1秒
                if ((bits & TO_BIT(抽屉打开输入))==0)
                {
                    gameFlag++;
                }
                break;
            }
            case 5://煤气灯闪烁 留声机播放音频2
            {
                // 留声机播放音频2

                //恐怖音效2
                char *MusicName="/BGM/002.mp3";
                PlayMusicName(&MUSIC_1,MusicName, strlen(MusicName),单曲循环);

                //煤气灯闪烁
                if (BlinkTaskHandle == NULL)
                    xTaskCreate(BlinkLight, "Blink", 128, NULL, 1, &BlinkTaskHandle);
                gameFlag++;
                break;
            }
            case 6://等待敲门机关
            {
                EventBits_t bits = xEventGroupWaitBits(InputEventGroup[0], TO_BIT(敲门输入), pdFALSE, pdTRUE,
                                                       0);  // 等待1秒
                if (bits & TO_BIT(敲门输入))
                {
                    gameFlag++;
                }
                break;
            }
            case 7://关闭煤气灯 开密室门 留声机播放音频3
            {
                // 开密室门
                if (!ASCTaskHandle[6]) {
                    ASCTaskHandle[6] = true;
                    int ASCNum = 6;
                    xTaskCreate(OpenDoor, "Opendoor", 128, &ASCNum, 1, NULL);
                }

                //留声机播放音频3

                //关闭煤气灯
                if (BlinkTaskHandle != NULL)
                {
                    vTaskDelete(BlinkTaskHandle);
                    BlinkTaskHandle = NULL;
                    SetOutput(煤气灯输出, GPIO_PIN_RESET);
                }
                gameFlag++;
                break;
            }
            case 8://等待毒药检测
            {

                break;
            }
            case 9:// 开卧室门 留声机播放音频4
            {
                //开卧室门

                //留声机播放音频4

                gameFlag++;
                break;
            }
            case 10: // 等待花砖检测
            {
                break;
            }
            case 11://开地窖门 延时音频/灯效
            {

                char *MusicName="/BGM/001.mp3";
                PlayMusicName(&MUSIC_2,MusicName, strlen(MusicName),单曲停止);
                SetOutput(地窖门电源,GPIO_PIN_SET);
                osDelay(5000);
                SetOutput(地窖门控制,GPIO_PIN_SET);
                osDelay(4000);
                SetOutput(地窖门控制,GPIO_PIN_RESET);
                SetOutput(地窖门电源,GPIO_PIN_RESET);
                osDelay(15000);

                MusicName="/BGM/002.mp3";
                PlayMusicName(&MUSIC_2,MusicName, strlen(MusicName),单曲停止);

                MusicName="/BGM/003.mp3";
                PlayMusicName(&MUSIC_2,MusicName, strlen(MusicName),单曲循环);

                gameFlag++;
                break;
            }
            default:
                break;
        }
    }

}

void StartPerTask1(void const *argument)
{

    bool BlinkLight = false;
        EventBits_t bits = xEventGroupWaitBits(InputEventGroup[0], TO_BIT(1), pdFALSE, pdTRUE,
                                               0);  // 等待1秒
        if (bits & TO_BIT(钢琴输入)) {
            if(!BlinkLight){
                BlinkLight = true;
//                SetOutput(爆闪灯, GPIO_PIN_SET);
            }
        }
        else
        {
            if(BlinkLight){
                BlinkLight = false;
//                SetOutput(爆闪灯, GPIO_PIN_RESET);
            }
        }
}
