/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define OSC8_IN_Pin GPIO_PIN_0
#define OSC8_IN_GPIO_Port GPIOD
#define OSC8_OUT_Pin GPIO_PIN_1
#define OSC8_OUT_GPIO_Port GPIOD
#define MUSIC1_TX_Pin GPIO_PIN_2
#define MUSIC1_TX_GPIO_Port GPIOA
#define MUSIC1_RX_Pin GPIO_PIN_3
#define MUSIC1_RX_GPIO_Port GPIOA
#define HC595_STCP_Pin GPIO_PIN_4
#define HC595_STCP_GPIO_Port GPIOA
#define HC595_SHCP_Pin GPIO_PIN_5
#define HC595_SHCP_GPIO_Port GPIOA
#define HC595_DS_Pin GPIO_PIN_7
#define HC595_DS_GPIO_Port GPIOA
#define MUSIC2_TX_Pin GPIO_PIN_10
#define MUSIC2_TX_GPIO_Port GPIOB
#define MUSIC2_RX_Pin GPIO_PIN_11
#define MUSIC2_RX_GPIO_Port GPIOB
#define HC165_PL__Pin GPIO_PIN_12
#define HC165_PL__GPIO_Port GPIOB
#define HC165_CP_Pin GPIO_PIN_13
#define HC165_CP_GPIO_Port GPIOB
#define HC165_DS_Pin GPIO_PIN_14
#define HC165_DS_GPIO_Port GPIOB
#define LAN_TX_Pin GPIO_PIN_9
#define LAN_TX_GPIO_Port GPIOA
#define LAN_RX_Pin GPIO_PIN_10
#define LAN_RX_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define OUTPUT_0_Pin GPIO_PIN_15
#define OUTPUT_0_GPIO_Port GPIOA
#define OUTPUT_1_Pin GPIO_PIN_3
#define OUTPUT_1_GPIO_Port GPIOB
#define OUTPUT_2_Pin GPIO_PIN_4
#define OUTPUT_2_GPIO_Port GPIOB
#define OUTPUT_3_Pin GPIO_PIN_5
#define OUTPUT_3_GPIO_Port GPIOB
#define OUTPUT_4_Pin GPIO_PIN_6
#define OUTPUT_4_GPIO_Port GPIOB
#define OUTPUT_5_Pin GPIO_PIN_7
#define OUTPUT_5_GPIO_Port GPIOB
#define OUTPUT_6_Pin GPIO_PIN_8
#define OUTPUT_6_GPIO_Port GPIOB
#define OUTPUT_7_Pin GPIO_PIN_9
#define OUTPUT_7_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
