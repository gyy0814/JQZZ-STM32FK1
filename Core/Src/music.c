//
// Created by 26913 on 2023/12/13.
//

#include "music.h"
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include <string.h>

extern QueueHandle_t MusicMessageQueueHandle;
extern QueueHandle_t MusicUartMessageQueueHandle;

extern EventGroupHandle_t MusicEventGroup;

// 音乐发送数据函数
void MusicSendData(UART_HandleTypeDef *huart, uint8_t CMD, const uint8_t *data, size_t dataLength)
{
    uint8_t txBuffer[dataLength+4];

    txBuffer[0] = CMD;
    txBuffer[1] = ~CMD;
    txBuffer[2] = (uint8_t)dataLength;
    memcpy(&txBuffer[3],data,dataLength);
    txBuffer[dataLength+3] = 0;
    for (int i = 0; i < dataLength+3; i++)
    {
        txBuffer[dataLength+3] += txBuffer[i];
    }

    HAL_UART_Transmit(huart,txBuffer, dataLength+4,HAL_MAX_DELAY);
    osDelay(200);
}

void PlayMusicName(UART_HandleTypeDef *huart,const char* FileName,size_t FileNameSize, uint8_t PlayMode)
{
    if (FileNameSize>64)
        return;


    /* Set playMode */
    MusicMessage newMusicMessage = (MusicMessage){
            .huart = huart
    };

//    newMusicMessage.CMD = 0x04;
//    newMusicMessage.Data[0] = 0x0A;
//    newMusicMessage.DataLength=1;
//    xQueueSend(MusicMessageQueueHandle,&newMusicMessage,0);

    /* Play music form fileName */
    newMusicMessage.CMD = 0x04;
    newMusicMessage.Data[0] = 0x07;
    memcpy(&newMusicMessage.Data[1],FileName,FileNameSize);
    newMusicMessage.DataLength = FileNameSize+1;
    xQueueSend(MusicMessageQueueHandle,&newMusicMessage,0);

//    newMusicMessage.CMD = 0x0B;
//    newMusicMessage.Data[0] = 0x01;
//    newMusicMessage.Data[1] = PlayMode;
//    newMusicMessage.DataLength=2;
//    xQueueSend(MusicMessageQueueHandle,&newMusicMessage,0);

}

void SetUsbMusic(UART_HandleTypeDef *huart)
{
    MusicMessage newMusicMessage = (MusicMessage){
            .huart = huart,
            .CMD = 0x04,
            .DataLength=1
    };

    newMusicMessage.Data[0] = 0x0A;
    xQueueSend(MusicMessageQueueHandle,&newMusicMessage,0);
}
void SetPlayMode(UART_HandleTypeDef *huart, uint8_t PlayMode)
{
    MusicMessage newMusicMessage = (MusicMessage){
            .huart = huart,
            .CMD = 0x0B,
            .DataLength=2
    };

    newMusicMessage.Data[0] = 0x01;
    newMusicMessage.Data[1] = PlayMode;
    xQueueSend(MusicMessageQueueHandle,&newMusicMessage,0);
}
void PlayMusic(UART_HandleTypeDef *huart)
{
    /* Plau Music */
    MusicMessage newMusicMessage = (MusicMessage){
            .huart = huart,
            .CMD = 0x04,
            .DataLength=1
    };

    newMusicMessage.Data[0] = 0x01;
    xQueueSend(MusicMessageQueueHandle,&newMusicMessage,0);
}
void StopMusic(UART_HandleTypeDef *huart)
{
    /* Plau Music */
    MusicMessage newMusicMessage = (MusicMessage){
            .huart = huart,
            .CMD = 0x04,
            .DataLength=1
    };

    newMusicMessage.Data[0] = 0x03;
    xQueueSend(MusicMessageQueueHandle,&newMusicMessage,0);
}

void PauseMusic(UART_HandleTypeDef *huart)
{
    /* Pause Music */
    MusicMessage newMusicMessage = (MusicMessage){
            .huart = huart,
            .CMD = 0x04,
            .DataLength=1
    };

    newMusicMessage.Data[0] = 0x02;
    xQueueSend(MusicMessageQueueHandle,&newMusicMessage,0);
}

//音乐事件处理任务
void StartMusicTask(void const * argument)
{
    /* USER CODE BEGIN StartMusicTask */
    MusicMessage newMusicMessage;
    UartMessage newUartMessage;
    GPIO_PinState Music1BusyState = HAL_GPIO_ReadPin(MUSIC1_BUSY_GPIO_Port,MUSIC1_BUSY_Pin);
    GPIO_PinState Music2BusyState = HAL_GPIO_ReadPin(MUSIC2_BUSY_GPIO_Port,MUSIC2_BUSY_Pin);
    /* Infinite loop */
    for(;;)
    {
        if(xQueueReceive(MusicMessageQueueHandle,&newMusicMessage,0)==pdTRUE)
        {
            MusicSendData(newMusicMessage.huart,newMusicMessage.CMD,newMusicMessage.Data,newMusicMessage.DataLength);
        }
        if(xQueueReceive(MusicUartMessageQueueHandle,&newUartMessage,0) == pdTRUE) {
            uint8_t TxBuffer[67];
            TxBuffer[0] = 0xCC;
            TxBuffer[1] = newUartMessage.from;
            memcpy(&TxBuffer[2],newUartMessage.data,newUartMessage.length);
            TxBuffer[newUartMessage.length+2] = 0xFF;
            HAL_UART_Transmit(&huart1,TxBuffer,newUartMessage.length+3,HAL_MAX_DELAY);
        }
        if ((Music1BusyState==GPIO_PIN_SET)&&(HAL_GPIO_ReadPin(MUSIC1_BUSY_GPIO_Port,MUSIC1_BUSY_Pin)==GPIO_PIN_RESET))
        {
            //Music1开始播放
            xEventGroupClearBits(MusicEventGroup, MUSIC1_BUSY_EVENT);
        }
        if ((Music1BusyState==GPIO_PIN_RESET)&&(HAL_GPIO_ReadPin(MUSIC1_BUSY_GPIO_Port,MUSIC1_BUSY_Pin)==GPIO_PIN_SET))
        {
            //Music1播
            xEventGroupSetBits(MusicEventGroup, MUSIC1_BUSY_EVENT);
        }
        if ((Music2BusyState==GPIO_PIN_SET)&&(HAL_GPIO_ReadPin(MUSIC2_BUSY_GPIO_Port,MUSIC2_BUSY_Pin)==GPIO_PIN_RESET))
        {
            //Music2开始播放
            xEventGroupClearBits(MusicEventGroup, MUSIC2_BUSY_EVENT);
        }
        if ((Music2BusyState==GPIO_PIN_RESET)&&(HAL_GPIO_ReadPin(MUSIC2_BUSY_GPIO_Port,MUSIC2_BUSY_Pin)==GPIO_PIN_SET))
        {
            //Music2播放结束
            xEventGroupSetBits(MusicEventGroup, MUSIC2_BUSY_EVENT);
        }
        Music1BusyState = HAL_GPIO_ReadPin(MUSIC1_BUSY_GPIO_Port,MUSIC1_BUSY_Pin);
        Music2BusyState = HAL_GPIO_ReadPin(MUSIC2_BUSY_GPIO_Port,MUSIC2_BUSY_Pin);

        osDelay(1);
    }
    /* USER CODE END StartMusicTask */
}

