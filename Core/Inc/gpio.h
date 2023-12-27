/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.h
  * @brief   This file contains all the function prototypes for
  *          the gpio.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

#include "FreeRTOS.h"
#include "queue.h"
#include "main.h"
#include "cmsis_os.h"

#include "usart.h"
/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

#define OUTPUT_NUM 112
#define INPUT_NUM 48

typedef struct {
    uint16_t GPIO_Pin;
    GPIO_PinState PinState;
} GPIOMessage;

//board output write define
#define OUTPUT_0_WRITE(PinState) HAL_GPIO_WritePin(OUTPUT_0_GPIO_Port,OUTPUT_0_Pin,PinState)
#define OUTPUT_1_WRITE(PinState) HAL_GPIO_WritePin(OUTPUT_1_GPIO_Port,OUTPUT_1_Pin,PinState)
#define OUTPUT_2_WRITE(PinState) HAL_GPIO_WritePin(OUTPUT_2_GPIO_Port,OUTPUT_2_Pin,PinState)
#define OUTPUT_3_WRITE(PinState) HAL_GPIO_WritePin(OUTPUT_3_GPIO_Port,OUTPUT_3_Pin,PinState)
#define OUTPUT_4_WRITE(PinState) HAL_GPIO_WritePin(OUTPUT_4_GPIO_Port,OUTPUT_4_Pin,PinState)
#define OUTPUT_5_WRITE(PinState) HAL_GPIO_WritePin(OUTPUT_5_GPIO_Port,OUTPUT_5_Pin,PinState)
#define OUTPUT_6_WRITE(PinState) HAL_GPIO_WritePin(OUTPUT_6_GPIO_Port,OUTPUT_6_Pin,PinState)
#define OUTPUT_7_WRITE(PinState) HAL_GPIO_WritePin(OUTPUT_7_GPIO_Port,OUTPUT_7_Pin,PinState)

#define HC595_DSH                HAL_GPIO_WritePin(HC595_DS_GPIO_Port,HC595_DS_Pin,GPIO_PIN_SET)
#define HC595_STCPH              HAL_GPIO_WritePin(HC595_STCP_GPIO_Port,HC595_STCP_Pin,GPIO_PIN_SET)
#define HC595_SHCPH              HAL_GPIO_WritePin(HC595_SHCP_GPIO_Port,HC595_SHCP_Pin,GPIO_PIN_SET)

#define HC595_DSL                HAL_GPIO_WritePin(HC595_DS_GPIO_Port,HC595_DS_Pin,GPIO_PIN_RESET)
#define HC595_STCPL              HAL_GPIO_WritePin(HC595_STCP_GPIO_Port,HC595_STCP_Pin,GPIO_PIN_RESET)
#define HC595_SHCPL              HAL_GPIO_WritePin(HC595_SHCP_GPIO_Port,HC595_SHCP_Pin,GPIO_PIN_RESET)

#define HC165_DS HAL_GPIO_ReadPin(HC165_DS_GPIO_Port,HC165_DS_Pin)

#define HC165_PLH HAL_GPIO_WritePin(HC165_PL__GPIO_Port,HC165_PL__Pin,GPIO_PIN_SET)
#define HC165_CPH HAL_GPIO_WritePin(HC165_CP_GPIO_Port,HC165_CP_Pin,GPIO_PIN_SET)

#define HC165_PLL HAL_GPIO_WritePin(HC165_PL__GPIO_Port,HC165_PL__Pin,GPIO_PIN_RESET)
#define HC165_CPL HAL_GPIO_WritePin(HC165_CP_GPIO_Port,HC165_CP_Pin,GPIO_PIN_RESET)

/* USER CODE END Private defines */

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */

void StartGPIOTask(void const *argument);

BaseType_t SetOutput(int Pin, GPIO_PinState state);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */

