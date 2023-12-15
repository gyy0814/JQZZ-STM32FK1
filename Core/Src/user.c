//
// Created by 26913 on 2023/12/14.
//

#include "user.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "cmsis_os.h"
#include "gpio.h"

extern EventGroupHandle_t InputEventGroup;
extern QueueHandle_t OutputMessageQueueHandle;

void UserInit()
{

}

void StartUserTask(void const * argument)
{
    /* USER CODE BEGIN StartUserTask */
    while (1)
    {
        EventBits_t xEventGroupValue = xEventGroupWaitBits(InputEventGroup,I0_UP_EVENT, pdFALSE, pdFALSE, portMAX_DELAY);
        if(xEventGroupValue && I0_UP_EVENT)
        {
            SetOutput(0,GPIO_PIN_SET);
            xEventGroupClearBits(InputEventGroup, I0_UP_EVENT);
        }
    }
    /* USER CODE END StartUserTask */
}

void Startuser1Task(void const * argument)
{
    /* USER CODE BEGIN StartUserTask */
    while (1)
    {
        EventBits_t xEventGroupValue = xEventGroupWaitBits(InputEventGroup,I1_UP_EVENT, pdFALSE, pdFALSE, portMAX_DELAY);
        if(xEventGroupValue && I1_UP_EVENT)
        {

            SetOutput(0,GPIO_PIN_RESET);

            xEventGroupClearBits(InputEventGroup, I1_UP_EVENT);
        }
    }
    /* USER CODE END StartUserTask */
}