//
// Created by 26913 on 2023/12/14.
//

#include <string.h>
#include <stdio.h>
#include "user.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "cmsis_os.h"
#include "gpio.h"

extern EventGroupHandle_t InputEventGroup[(INPUT_NUM / 32) + 1];
extern EventGroupHandle_t MusicEventGroup;
extern QueueHandle_t OutputMessageQueueHandle;

#define NUM_TASKS 12

void DK1(void *parameters);
void DK2(void *parameters);
void DK3(void *parameters);
void DK4(void *parameters);
void DK5(void *parameters);
void DK6(void *parameters);
void DK7(void *parameters);
void DK8(void *parameters);
void DK9(void *parameters);
void DK10(void *parameters);
void DK11(void *parameters);
void DK12(void *parameters);


// 存储任务函数的数组
TaskFunction_t taskFunctions[NUM_TASKS] = {DK1, DK2, DK3, DK4, DK5, DK6, DK7, DK8, DK9, DK10, DK11, DK12};

// 存储任务句柄的数组
TaskHandle_t tasks[NUM_TASKS] = {0};

void signalLight(int num,int state)
{
    switch (state) {
        case L_OFF:
            SetOutput(num*2,GPIO_PIN_RESET);
            SetOutput((num*2)+1,GPIO_PIN_RESET);
            break;
        case L_RED:
            SetOutput(num*2,GPIO_PIN_SET);
            SetOutput((num*2)+1,GPIO_PIN_RESET);
            break;
        case L_GRN:
            SetOutput(num*2,GPIO_PIN_RESET);
            SetOutput((num*2)+1,GPIO_PIN_SET);
            break;
        default:
            break;
    }
}

void CreateTask(int taskId)
{
    if (tasks[taskId - 1] == NULL) // 如果任务句柄为空，表示任务未创建
    {
        SetOutput(窗继电器(taskId),GPIO_PIN_SET);
        SetOutput(射灯(taskId),GPIO_PIN_SET);
//
//        char taskName[configMAX_TASK_NAME_LEN];
//        sprintf(taskName, "DK%d", taskId);
//        TaskHandle_t xTaskHandle;
//        xTaskCreate(DK1,    // 任务函数
//                    "dk",       // 任务名称
//                    256,              // 堆栈大小（以字为单位）
//                    NULL,             // 任务参数
//                    1,                // 任务优先级
//                    &xTaskHandle);    // 任务句柄
        xTaskCreate(taskFunctions[taskId - 1], "DKTask", configMINIMAL_STACK_SIZE, (void *)taskId, tskIDLE_PRIORITY + 1, &tasks[taskId - 1]);
    }
}

void DestroyTask(int taskId)
{
    if (tasks[taskId - 1] != NULL) // 如果任务句柄不为空，表示任务存在
    {
        SetOutput(窗继电器(taskId),GPIO_PIN_RESET);
        SetOutput(射灯(taskId),GPIO_PIN_RESET);
        vTaskDelete(tasks[taskId - 1]); // 删除任务
        tasks[taskId - 1] = NULL;       // 清空任务句柄
    }
}

void DK1(void *parameters)
{
    osDelay(25000);


    SetOutput(窗继电器(1),GPIO_PIN_RESET);
    SetOutput(射灯(1),GPIO_PIN_RESET);
    vTaskDelete(NULL);
}
void DK2(void *parameters)
{
    osDelay(5000);

    SetOutput(窗继电器(2),GPIO_PIN_RESET);
    SetOutput(射灯(2),GPIO_PIN_RESET);
    vTaskDelete(NULL);
}
void DK3(void *parameters)
{
    SetOutput(C3O_ZL,GPIO_PIN_SET);
    SetOutput(C3O_FL,GPIO_PIN_SET);

    EventBits_t bits = xEventGroupWaitBits(InputEventGroup[0], TO_BIT(C3I_ZS) | TO_BIT(C3I_FS),
                                           pdTRUE, pdFALSE, portMAX_DELAY);
    if((bits&TO_BIT(C3I_ZS)) != 0)
    {
        //正方胜利
    }
    else if ((bits&TO_BIT(C3I_FS))!=0)
    {
        //反方胜利
    }

    SetOutput(窗继电器(2),GPIO_PIN_RESET);
    SetOutput(射灯(2),GPIO_PIN_RESET);
    vTaskDelete(NULL);
}
void DK4(void *parameters)
{
    SetOutput(C4O_L,GPIO_PIN_SET);

    EventBits_t bits = xEventGroupWaitBits(InputEventGroup[0], TO_BIT(C4I_ZS) | TO_BIT(C4I_FS),
                                           pdTRUE, pdFALSE, portMAX_DELAY);
    if((bits&TO_BIT(C4I_ZS)) != 0)
    {
        //正方胜利
    }
    else if ((bits&TO_BIT(C4I_FS))!=0)
    {
        //反方胜利
    }
    SetOutput(窗继电器(4),GPIO_PIN_RESET);
    SetOutput(射灯(4),GPIO_PIN_RESET);
    vTaskDelete(NULL);
}
void DK5(void *parameters)
{
    SetOutput(C5O_L,GPIO_PIN_SET);

    EventBits_t bits = xEventGroupWaitBits(InputEventGroup[0], TO_BIT(C5I_ZS) | TO_BIT(C5I_FS),
                                           pdTRUE, pdFALSE, portMAX_DELAY);
    if((bits&TO_BIT(C5I_ZS)) != 0)
    {
        //正方胜利
    }
    else if ((bits&TO_BIT(C5I_FS))!=0)
    {
        //反方胜利
    }
    SetOutput(窗继电器(5),GPIO_PIN_RESET);
    SetOutput(射灯(5),GPIO_PIN_RESET);
    vTaskDelete(NULL);
}
void DK6(void *parameters)
{
    SetOutput(C6O_ZL,GPIO_PIN_SET);
    SetOutput(C6O_FL,GPIO_PIN_SET);

    EventBits_t bits = xEventGroupWaitBits(InputEventGroup[0], TO_BIT(C6I_ZS) | TO_BIT(C6I_FS),
                                           pdTRUE, pdFALSE, portMAX_DELAY);
    if((bits&TO_BIT(C6I_ZS)) != 0)
    {
        //正方胜利
    }
    else if ((bits&TO_BIT(C6I_FS))!=0)
    {
        //反方胜利
    }

    SetOutput(窗继电器(6),GPIO_PIN_RESET);
    SetOutput(射灯(6),GPIO_PIN_RESET);
    vTaskDelete(NULL);
}
void DK7(void *parameters)
{
    SetOutput(C7O_L,GPIO_PIN_SET);

    EventBits_t bits = xEventGroupWaitBits(InputEventGroup[0], TO_BIT(C7I_ZS) | TO_BIT(C7I_FS),
                                           pdTRUE, pdFALSE, portMAX_DELAY);
    if((bits&TO_BIT(C7I_ZS)) != 0)
    {
        //正方胜利
    }
    else if ((bits&TO_BIT(C7I_FS))!=0)
    {
        //反方胜利
    }
    SetOutput(窗继电器(7),GPIO_PIN_RESET);
    SetOutput(射灯(7),GPIO_PIN_RESET);
    vTaskDelete(NULL);
}
void DK8(void *parameters)
{
    SetOutput(C8O_L,GPIO_PIN_SET);

    EventBits_t bits = xEventGroupWaitBits(InputEventGroup[0], TO_BIT(C8I_ZS) | TO_BIT(C8I_FS),
                                           pdTRUE, pdFALSE, portMAX_DELAY);
    if((bits&TO_BIT(C8I_ZS)) != 0)
    {
        //正方胜利
    }
    else if ((bits&TO_BIT(C8I_FS))!=0)
    {
        //反方胜利
    }
    SetOutput(窗继电器(8),GPIO_PIN_RESET);
    SetOutput(射灯(8),GPIO_PIN_RESET);
    vTaskDelete(NULL);
}
void DK9(void *parameters)
{
    osDelay(5000);

    SetOutput(窗继电器(9),GPIO_PIN_RESET);
    SetOutput(射灯(9),GPIO_PIN_RESET);
    vTaskDelete(NULL);
}
void DK10(void *parameters)
{
    SetOutput(C10O_ZL,GPIO_PIN_SET);
    SetOutput(C10O_FL,GPIO_PIN_SET);

    EventBits_t bits = xEventGroupWaitBits(InputEventGroup[1], TO_BIT(C10I_ZS) | TO_BIT(C10I_FS),
                                           pdTRUE, pdFALSE, portMAX_DELAY);
    if((bits&TO_BIT(C10I_ZS)) != 0)
    {
        //正方胜利
    }
    else if ((bits&TO_BIT(C10I_FS))!=0)
    {
        //反方胜利
    }

    SetOutput(窗继电器(10),GPIO_PIN_RESET);
    SetOutput(射灯(10),GPIO_PIN_RESET);
    vTaskDelete(NULL);
}
void DK11(void *parameters)
{
    SetOutput(C11O_L,GPIO_PIN_SET);

    EventBits_t bits = xEventGroupWaitBits(InputEventGroup[0], TO_BIT(C11I_ZS) | TO_BIT(C11I_FS),
                                           pdTRUE, pdFALSE, portMAX_DELAY);
    if((bits&TO_BIT(C11I_ZS)) != 0)
    {
        //正方胜利
    }
    else if ((bits&TO_BIT(C11I_FS))!=0)
    {
        //反方胜利
    }
    SetOutput(窗继电器(11),GPIO_PIN_RESET);
    SetOutput(射灯(11),GPIO_PIN_RESET);
    vTaskDelete(NULL);
}
void DK12(void *parameters)
{
    SetOutput(C12O_L,GPIO_PIN_SET);

    EventBits_t bits = xEventGroupWaitBits(InputEventGroup[2], TO_BIT(C12I_ZS) | TO_BIT(C12I_FS),
                                           pdTRUE, pdFALSE, portMAX_DELAY);
    if((bits&TO_BIT(C12I_ZS)) != 0)
    {
        //正方胜利
    }
    else if ((bits&TO_BIT(C12I_FS))!=0)
    {
        //反方胜利
    }
    SetOutput(窗继电器(12),GPIO_PIN_RESET);
    SetOutput(射灯(12),GPIO_PIN_RESET);
    vTaskDelete(NULL);
}

