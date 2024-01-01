/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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
#include "gpio.h"

/* USER CODE BEGIN 0 */


GPIO_PinState OutputState[OUTPUT_NUM];
GPIO_PinState InputState[INPUT_NUM];
GPIO_PinState previousInputState[INPUT_NUM];

extern SemaphoreHandle_t xConnectSemaphore;
extern QueueHandle_t OutputMessageQueueHandle;

extern EventGroupHandle_t InputEventGroup[(INPUT_NUM / 32) + 1];

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */
/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, HC595_STCP_Pin|HC595_SHCP_Pin|HC595_DS_Pin|OUTPUT_0_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, HC165_PL__Pin|HC165_CP_Pin|OUTPUT_1_Pin|OUTPUT_2_Pin
                          |OUTPUT_3_Pin|OUTPUT_4_Pin|OUTPUT_5_Pin|OUTPUT_6_Pin
                          |OUTPUT_7_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = MUSIC1_BUSY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(MUSIC1_BUSY_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PAPin PAPin PAPin PAPin */
  GPIO_InitStruct.Pin = HC595_STCP_Pin|HC595_SHCP_Pin|HC595_DS_Pin|OUTPUT_0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = MUSIC2_BUSY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(MUSIC2_BUSY_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PBPin PBPin PBPin PBPin
                           PBPin PBPin PBPin PBPin
                           PBPin */
  GPIO_InitStruct.Pin = HC165_PL__Pin|HC165_CP_Pin|OUTPUT_1_Pin|OUTPUT_2_Pin
                          |OUTPUT_3_Pin|OUTPUT_4_Pin|OUTPUT_5_Pin|OUTPUT_6_Pin
                          |OUTPUT_7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = HC165_DS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(HC165_DS_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */

/* OUTPUT BEGIN */
void BoardOutput(const GPIO_PinState *pOutputState) {
    OUTPUT_0_WRITE(pOutputState[0]);
    OUTPUT_1_WRITE(pOutputState[1]);
    OUTPUT_2_WRITE(pOutputState[2]);
    OUTPUT_3_WRITE(pOutputState[3]);
    OUTPUT_4_WRITE(pOutputState[4]);
    OUTPUT_5_WRITE(pOutputState[5]);
    OUTPUT_6_WRITE(pOutputState[6]);
    OUTPUT_7_WRITE(pOutputState[7]);
}

void HC595Output(const GPIO_PinState *pOutputState, int OutputNum) {
    for (int i = (OutputNum - 1); i > 7; i--) {
        /*--step1准备数据到DS引脚*/
        if (pOutputState[i] == GPIO_PIN_RESET) {
            HC595_DSL;
        } else {
            HC595_DSH;
        }
        /*--step2CLK引脚实现上升�?????????????*/
        HC595_SHCPL;
        osDelay(1);
        //是否�?????????????要延�?????????????
        HC595_SHCPH;
    }
    /*--step3发�?�完成后存储到寄存器*/
    HC595_STCPL;
    osDelay(1);
    //是否�?????????????要延�?????????????
    HC595_STCPH;
}

void OutputSend(const GPIO_PinState *pOutputState) {
    BoardOutput(pOutputState);
    HC595Output(pOutputState, OUTPUT_NUM);
}

BaseType_t SetOutput(int Pin, GPIO_PinState state) {
    GPIOMessage OutputMessage = {
            .GPIO_Pin = Pin,
            .PinState = state,
    };
    BaseType_t result = xQueueSend(OutputMessageQueueHandle, &OutputMessage, 0);
    return result;
}
/* OUTPUT END */

/* INPUT BEGIN */
void HC165Input(GPIO_PinState *pInputState, int InputNum) {
    HC165_PLL;
    osDelay(1);
    HC165_PLH;
    osDelay(1);

    // read number one
    pInputState[0] = HC165_DS ? GPIO_PIN_RESET : GPIO_PIN_SET;

    for (int i = 1; i < InputNum; ++i) {
        HC165_CPL;
        osDelay(1);
        HC165_CPH;

        pInputState[i] = HC165_DS ? GPIO_PIN_RESET : GPIO_PIN_SET;
    }

    //反向数组
    for (int i = 0; i < InputNum;) {
        GPIO_PinState temp;
        for (int j = 0; j < 4; j++) {

            temp = pInputState[j + i];
            pInputState[j + i] = pInputState[(7 - j) + i];
            pInputState[(7 - j) + i] = temp;
        }
        i = i + 8;
    }
}

GPIO_PinState ReadInput(int Pin) {
    return InputState[Pin];
}

// 判断是否有边沿产生的函数
uint8_t IsEdgeDetected(int InputNum) {
    if (InputState[InputNum] != previousInputState[InputNum])
        return 1;
    else
        return 0;

}

void InputRecv(GPIO_PinState *pInputState) {
    static int IsSetup = 0;
    HC165Input(pInputState, INPUT_NUM);
    for (int i = 0; i < INPUT_NUM; i++) {
        if (IsEdgeDetected(i)) {
            if (ReadInput(i) == GPIO_PIN_SET) {
                xEventGroupSetBits(InputEventGroup[i / 32], (1 << (i % 32)));
            } else {
                xEventGroupClearBits(InputEventGroup[i / 32], (1 << (i % 32)));
            }
            if (IsSetup != 0) {
                uint8_t TxBuffer[5] = {0xCC, 0x01, i, ReadInput(i), 0xFF};
                HAL_UART_Transmit(&huart1, TxBuffer, 5, HAL_MAX_DELAY);
            }

            previousInputState[i] = InputState[i];
        }
    }
    IsSetup = 1;

}
/* INPUT END */


void StartGPIOTask(void const *argument) {
    GPIOMessage newMessage;
    while (1) {
        if (xQueueReceive(OutputMessageQueueHandle, &newMessage, 0) == pdTRUE) {
            if (OutputState[newMessage.GPIO_Pin] != newMessage.PinState) {
                OutputState[newMessage.GPIO_Pin] = newMessage.PinState;
                uint8_t TxBuffer[5] = {0xCC, 0x02, newMessage.GPIO_Pin, (newMessage.PinState == GPIO_PIN_SET) ? 1 : 0,
                                       0xFF};
                HAL_UART_Transmit(&huart1, TxBuffer, 5, HAL_MAX_DELAY);
            }
        }
        if (xSemaphoreTake(xConnectSemaphore, 0) == pdPASS) {
            for (int i = 0; i < INPUT_NUM; ++i) {
                if (InputState[i]==GPIO_PIN_SET)
                {
                    uint8_t TxBuffer[5] = {0xCC,0x01,i,0x01,0xFF};
                    HAL_UART_Transmit(&huart1, TxBuffer, 5, HAL_MAX_DELAY);
                }
            }
            for (int i = 0; i < OUTPUT_NUM; ++i) {
                if (OutputState[i]==GPIO_PIN_SET)
                {
                    uint8_t TxBuffer[5] = {0xCC,0x02,i,0x01,0xFF};
                    HAL_UART_Transmit(&huart1, TxBuffer, 5, HAL_MAX_DELAY);
                }
            }
        }
        osDelay(1);
        OutputSend((GPIO_PinState *) OutputState);
        InputRecv((GPIO_PinState *) InputState);
    }
}
/* USER CODE END 2 */
