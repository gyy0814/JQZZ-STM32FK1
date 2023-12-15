/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
#include <string.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "main.h"
#include "cmsis_os.h"

extern uint8_t uart1RxBuffer[32];
extern uint8_t uart1RxIndex;

extern uint8_t uart2RxBuffer[32];
extern uint8_t uart2RxIndex;

extern uint8_t uart3RxBuffer[32];
extern uint8_t uart3RxIndex;

extern QueueHandle_t Uart1RxMsgQueueHandle;
extern QueueHandle_t MusicUartMessageQueueHandle;
//extern osMessageQueueId_t Uart1TxMsgQueueHandle;
/* USER CODE END 0 */

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}
/* USART2 init function */

void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}
/* USART3 init function */

void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = LAN_TX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(LAN_TX_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LAN_RX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(LAN_RX_GPIO_Port, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */
    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    GPIO_InitStruct.Pin = MUSIC1_TX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(MUSIC1_TX_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = MUSIC1_RX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(MUSIC1_RX_GPIO_Port, &GPIO_InitStruct);

    /* USART2 interrupt Init */
    HAL_NVIC_SetPriority(USART2_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspInit 1 */

  /* USER CODE END USART2_MspInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspInit 0 */

  /* USER CODE END USART3_MspInit 0 */
    /* USART3 clock enable */
    __HAL_RCC_USART3_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**USART3 GPIO Configuration
    PB10     ------> USART3_TX
    PB11     ------> USART3_RX
    */
    GPIO_InitStruct.Pin = MUSIC2_TX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(MUSIC2_TX_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = MUSIC2_RX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(MUSIC2_RX_GPIO_Port, &GPIO_InitStruct);

    /* USART3 interrupt Init */
    HAL_NVIC_SetPriority(USART3_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspInit 1 */

  /* USER CODE END USART3_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, LAN_TX_Pin|LAN_RX_Pin);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspDeInit 0 */

  /* USER CODE END USART2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();

    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    HAL_GPIO_DeInit(GPIOA, MUSIC1_TX_Pin|MUSIC1_RX_Pin);

    /* USART2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspDeInit 1 */

  /* USER CODE END USART2_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspDeInit 0 */

  /* USER CODE END USART3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART3_CLK_DISABLE();

    /**USART3 GPIO Configuration
    PB10     ------> USART3_TX
    PB11     ------> USART3_RX
    */
    HAL_GPIO_DeInit(GPIOB, MUSIC2_TX_Pin|MUSIC2_RX_Pin);

    /* USART3 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspDeInit 1 */

  /* USER CODE END USART3_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1)
    {
        static BaseType_t packetStart = pdFALSE;

        if (uart1RxBuffer[uart1RxIndex] == 0xCC)
        {
            packetStart = pdTRUE;
            uart1RxIndex = 0;
        }

        if (packetStart)
        {
            uart1RxBuffer[uart1RxIndex++] = huart->Instance->DR;

            if (uart1RxBuffer[uart1RxIndex - 1] == 0xFF)
            {
                // 完整数据包接收完成，添加到消息队�??????????????
                BaseType_t xHigherPriorityTaskWoken = pdFALSE;
                UartMessage newMessage;
                newMessage.from = 0x02;
                newMessage.length = uart1RxIndex;
                memcpy(newMessage.data,uart1RxBuffer,uart1RxIndex);
                xQueueSendFromISR(Uart1RxMsgQueueHandle, &newMessage, &xHigherPriorityTaskWoken);

                packetStart = pdFALSE;
                uart1RxIndex = 0;
            }
        }

        // 启动下一次接�?????????????
        HAL_UART_Receive_IT(&huart1, &uart1RxBuffer[uart1RxIndex], 1);
    }
    else if(huart->Instance == USART2)
    {
        static BaseType_t packetStart = pdFALSE;
        static uint8_t dataSize = 0;
        if(uart2RxIndex == 1)
        {
            if ((uint8_t )uart2RxBuffer[1] == (uint8_t )(~uart2RxBuffer[0]))
            {
                packetStart = pdTRUE;
                uart2RxIndex++;
            }
            else
            {
                uart2RxIndex = 0;
                uart2RxBuffer[0] = uart2RxBuffer[1];
            }
        }
        else if(uart2RxIndex == 2){
            dataSize = uart2RxBuffer[2];
            uart2RxIndex++;
        }
        else if(uart2RxIndex == dataSize+3)
        {
            uart2RxIndex = 0;

            UartMessage newMessage;
            newMessage.from = 0x03;
            newMessage.length = dataSize+4;
            memcpy(newMessage.data,uart2RxBuffer,dataSize+4);

            BaseType_t xHigherPriorityTaskWoken = pdFALSE;
            xQueueSendFromISR(MusicUartMessageQueueHandle, &newMessage, &xHigherPriorityTaskWoken);
        }
        else
        {
            uart2RxIndex++;
        }
        HAL_UART_Receive_IT(&huart2, &uart2RxBuffer[uart2RxIndex], 1);
    }
    else if(huart->Instance == USART3)
    {
        static BaseType_t packetStart = pdFALSE;
        static uint8_t dataSize = 0;
        if(uart3RxIndex == 1)
        {
            if ((uint8_t )uart3RxBuffer[1] == (uint8_t )(~uart3RxBuffer[0]))
            {
                packetStart = pdTRUE;
                uart3RxIndex++;
            }
            else
            {
                uart3RxIndex = 0;
                uart3RxBuffer[0] = uart3RxBuffer[1];
            }
        }
        else if(uart3RxIndex == 2){
            dataSize = uart3RxBuffer[2];
            uart3RxIndex++;
        }
        else if(uart3RxIndex == dataSize+3)
        {
            uart3RxIndex = 0;

            UartMessage newMessage;
            newMessage.from = 0x04;
            newMessage.length = dataSize+4;
            memcpy(newMessage.data,uart3RxBuffer,dataSize+4);

            BaseType_t xHigherPriorityTaskWoken = pdFALSE;
            xQueueSendFromISR(MusicUartMessageQueueHandle, &newMessage, &xHigherPriorityTaskWoken);
        }
        else
        {
            uart3RxIndex++;
        }
        HAL_UART_Receive_IT(&huart3, &uart3RxBuffer[uart3RxIndex], 1);
    }
}

/* USER CODE END 1 */
