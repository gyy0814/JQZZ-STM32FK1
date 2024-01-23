/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include "queue.h"
#include "gpio.h"
#include "music.h"
#include "user.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define CMD_OUTPUT 0x02
#define CMD_MUSIC1 0x03
#define CMD_MUSIC2 0x04
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

QueueHandle_t Uart1RxMsgQueueHandle;
QueueHandle_t OutputMessageQueueHandle;
QueueHandle_t MusicMessageQueueHandle;
QueueHandle_t MusicUartMessageQueueHandle;
QueueHandle_t GameMessageQueueHandle;

SemaphoreHandle_t xConnectSemaphore;

//Input Event 0-15Bit UP  16-31 DOWN
EventGroupHandle_t InputEventGroup[(INPUT_NUM / 24) + 1];

//Music Event 0-15Bit Music1  16-31Bit Music2
EventGroupHandle_t MusicEventGroup;

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId Uart1ReadHandleHandle;
osThreadId GPIOTaskHandle;
osThreadId musicTaskHandle;
osThreadId myASCTaskHandle;
osThreadId GameFlagTaskHandle;
osThreadId GameTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void Uart1ReadHandler(void const * argument);
void StartGPIOTask(void const * argument);
void StartMusicTask(void const * argument);
void StartASCTask(void const * argument);
void StartGameFlagTask(void const * argument);
void StartGameTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}

/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
    for (int i = 0; i <= INPUT_NUM / 24; ++i) {

        InputEventGroup[i] = xEventGroupCreate();
    }
    MusicEventGroup = xEventGroupCreate();
    GameInit();
    xConnectSemaphore = xSemaphoreCreateBinary();
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
    Uart1RxMsgQueueHandle = xQueueCreate(15, sizeof(UartMessage));  // 创建队列，可以容�??????????????????10个uint8_t大小的元�??????????????????
    MusicUartMessageQueueHandle = xQueueCreate(15, sizeof(UartMessage));  // 创建队列，可以容�??????????????????10个uint8_t大小的元�??????????????????
    OutputMessageQueueHandle = xQueueCreate(15, sizeof(GPIOMessage));  // 创建队列，可以容�??????????????????10个uint8_t大小的元�??????????????????
    MusicMessageQueueHandle = xQueueCreate(15, sizeof(MusicMessage));  // 创建队列，可以容�??????????????????10个uint8_t大小的元�??????????????????
    GameMessageQueueHandle = xQueueCreate(15, sizeof(GameMessage));  // 创建队列，可以容�??????????????????10个uint8_t大小的元�??????????????????
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of Uart1ReadHandle */
  osThreadDef(Uart1ReadHandle, Uart1ReadHandler, osPriorityRealtime, 0, 128);
  Uart1ReadHandleHandle = osThreadCreate(osThread(Uart1ReadHandle), NULL);

  /* definition and creation of GPIOTask */
  osThreadDef(GPIOTask, StartGPIOTask, osPriorityBelowNormal, 0, 128);
  GPIOTaskHandle = osThreadCreate(osThread(GPIOTask), NULL);

  /* definition and creation of musicTask */
  osThreadDef(musicTask, StartMusicTask, osPriorityIdle, 0, 256);
  musicTaskHandle = osThreadCreate(osThread(musicTask), NULL);

  /* definition and creation of myASCTask */
  osThreadDef(myASCTask, StartASCTask, osPriorityIdle, 0, 128);
  myASCTaskHandle = osThreadCreate(osThread(myASCTask), NULL);

  /* definition and creation of GameFlagTask */
  osThreadDef(GameFlagTask, StartGameFlagTask, osPriorityIdle, 0, 128);
  GameFlagTaskHandle = osThreadCreate(osThread(GameFlagTask), NULL);

  /* definition and creation of GameTask */
  osThreadDef(GameTask, StartGameTask, osPriorityIdle, 0, 128);
  GameTaskHandle = osThreadCreate(osThread(GameTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_Uart1ReadHandler */
/**
* @brief Function implementing the Uart1ReadHandle thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Uart1ReadHandler */
void Uart1ReadHandler(void const * argument)
{
  /* USER CODE BEGIN Uart1ReadHandler */
    UartMessage newMessage;
  /* Infinite loop */
    for (;;)
  {
      if (xQueueReceive(Uart1RxMsgQueueHandle, &newMessage, portMAX_DELAY) == pdTRUE)
      {
          // 处理接收到的数据
          switch (newMessage.data[1]) {

              case 0x01:
              {
                  if(newMessage.data[3]==0x01)
                    xEventGroupSetBits(InputEventGroup[newMessage.data[2] / 32], (1 << (newMessage.data[2] % 32)));
                  else if(newMessage.data[3]==0x00)
                      xEventGroupClearBits(InputEventGroup[newMessage.data[2] / 32], (1 << (newMessage.data[2] % 32)));
                  break;
              }
              case CMD_OUTPUT:
              {
                  GPIOMessage OutputMessage = {
                          .GPIO_Pin = newMessage.data[2],
                          .PinState = (newMessage.data[3] == 0)?GPIO_PIN_RESET:GPIO_PIN_SET,
                  };
                  BaseType_t result = xQueueSend(OutputMessageQueueHandle,&OutputMessage,0);
                  if (result != pdPASS)
                  {
                      newMessage.data[0]=0xEE;
                      HAL_UART_Transmit(&huart1,newMessage.data,newMessage.length,HAL_MAX_DELAY);
                  }
                  break;
              }

              case CMD_MUSIC1:
              {
                  // 0xCC CMD MUSIC_CMD DATA0 DATA1 DATA2... 0xFF

                  MusicMessage MusicMessage;
                  MusicMessage.huart = &MUSIC_1;
                  MusicMessage.CMD = newMessage.data[2];
                  MusicMessage.DataLength = newMessage.length-4;
                  memcpy(MusicMessage.Data,&newMessage.data[3],MusicMessage.DataLength);
                  BaseType_t result = xQueueSend(MusicMessageQueueHandle,&MusicMessage,0);
                  if (result != pdPASS)
                  {
                      newMessage.data[0]=0xEE;
                      HAL_UART_Transmit(&huart1,newMessage.data,newMessage.length,HAL_MAX_DELAY);
                  }

                  break;
              }
              case CMD_MUSIC2:
              {
// 0xCC CMD MUSIC_CMD DATA0 DATA1 DATA2... 0xFF

                  MusicMessage MusicMessage;
                  MusicMessage.huart = &MUSIC_2;
                  MusicMessage.CMD = newMessage.data[2];
                  MusicMessage.DataLength = newMessage.length-4;
                  memcpy(MusicMessage.Data,&newMessage.data[3],MusicMessage.DataLength);
                  BaseType_t result = xQueueSend(MusicMessageQueueHandle,&MusicMessage,0);
                  if (result != pdPASS)
                  {
                      newMessage.data[0]=0xEE;
                      HAL_UART_Transmit(&huart1,newMessage.data,newMessage.length,HAL_MAX_DELAY);
                  }

                  break;
              }
              case 0x05:
              {
                  GameMessage newGameMessage;
                  newGameMessage.num = newMessage.data[2];
                  newGameMessage.Data = newMessage.data[3];
                  xQueueSend(GameMessageQueueHandle,&newGameMessage,0);
                  break;
              }
              case 0x10:
              {
                  xSemaphoreGive(xConnectSemaphore);
                  break;
              }
              default:
                  break;
          }
      }

  }
  /* USER CODE END Uart1ReadHandler */
}

/* USER CODE BEGIN Header_StartGPIOTask */
/**
* @brief Function implementing the GPIOTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartGPIOTask */
__weak void StartGPIOTask(void const * argument)
{
  /* USER CODE BEGIN StartGPIOTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartGPIOTask */
}

/* USER CODE BEGIN Header_StartMusicTask */
/**
* @brief Function implementing the musicTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartMusicTask */
__weak void StartMusicTask(void const * argument)
{
  /* USER CODE BEGIN StartMusicTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartMusicTask */
}

/* USER CODE BEGIN Header_StartASCTask */
/**
* @brief Function implementing the myASCTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartASCTask */
__weak void StartASCTask(void const * argument)
{
  /* USER CODE BEGIN StartASCTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartASCTask */
}

/* USER CODE BEGIN Header_StartGameFlagTask */
/**
* @brief Function implementing the GameFlagTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartGameFlagTask */
__weak void StartGameFlagTask(void const * argument)
{
  /* USER CODE BEGIN StartGameFlagTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartGameFlagTask */
}

/* USER CODE BEGIN Header_StartGameTask */
/**
* @brief Function implementing the GameTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartGameTask */
__weak void StartGameTask(void const * argument)
{
  /* USER CODE BEGIN StartGameTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartGameTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

