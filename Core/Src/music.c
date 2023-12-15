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
}


//音乐事件处理任务
void StartMusicTask(void const * argument)
{
    /* USER CODE BEGIN StartMusicTask */
    MusicMessage newMusicMessage;
    UartMessage newUartMessage;
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
        osDelay(1);
    }
    /* USER CODE END StartMusicTask */
}

