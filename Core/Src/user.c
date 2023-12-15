//
// Created by 26913 on 2023/12/14.
//

#include <string.h>
#include "user.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "cmsis_os.h"
#include "gpio.h"

extern EventGroupHandle_t InputEventGroup;
extern EventGroupHandle_t MusicEventGroup;
extern QueueHandle_t OutputMessageQueueHandle;

void StartUserTask(void const * argument)
{
    /* USER CODE BEGIN StartUserTask */
    while (1)
    {
        xEventGroupWaitBits(InputEventGroup,I0_UP_EVENT, pdFALSE, pdFALSE, portMAX_DELAY);
        xEventGroupClearBits(InputEventGroup, I0_UP_EVENT);
        char *MusicName = "/BGM/001.mp3";
        PlayMusicName(&MUSIC_1,MusicName, strlen(MusicName),单曲停止);
        osDelay(300);
        xEventGroupWaitBits(MusicEventGroup,MUSIC1_BUSY_EVENT,pdFALSE,pdFALSE,portMAX_DELAY);
        SetOutput(0,GPIO_PIN_SET);
    }
    /* USER CODE END StartUserTask */
}

void Startuser1Task(void const * argument)
{
    /* USER CODE BEGIN StartUserTask */
    while (1)
    {
        xEventGroupWaitBits(InputEventGroup,I1_UP_EVENT, pdFALSE, pdFALSE, portMAX_DELAY);
        xEventGroupClearBits(InputEventGroup, I1_UP_EVENT);
        SetOutput(0,GPIO_PIN_RESET);
        xEventGroupClearBits(InputEventGroup, I1_UP_EVENT);
    }
    /* USER CODE END StartUserTask */
}