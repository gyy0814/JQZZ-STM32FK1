//
// Created by 26913 on 2023/12/14.
//

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "user.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "cmsis_os.h"
#include "gpio.h"

extern EventGroupHandle_t InputEventGroup[(INPUT_NUM / 32) + 1];
extern EventGroupHandle_t MusicEventGroup;
extern QueueHandle_t OutputMessageQueueHandle;
bool ASCTaskHandle[7];
void OpenDoor(void *pvParameters)
{
    int *pNum = (int *)pvParameters;
    int num = *pNum;
    SetOutput(LOOK(num),GPIO_PIN_SET);
    osDelay(4000);
    SetOutput(LOOK(num),GPIO_PIN_RESET);
    ASCTaskHandle[num] = false;
    vTaskDelete(NULL);
}

void StartASCTask(void const * argument)
{
    EventBits_t waitBits = TO_BIT(ASC(1))| TO_BIT(ASC(2))| TO_BIT(ASC(3))| TO_BIT(ASC(4))| TO_BIT(ASC(5))| TO_BIT(ASC(6))| TO_BIT(ASC(7));
    for(;;) {
        EventBits_t bits = xEventGroupWaitBits(InputEventGroup[0], waitBits, pdFALSE, pdFALSE, portMAX_DELAY);
        for (int i = 1; i <= 7; i++) {
            if (bits & TO_BIT(i - 1)) {
                int ASCNum = i - 1;
                if (!ASCTaskHandle[i - 1]) {
                    ASCTaskHandle[i - 1] = true;
                    xTaskCreate(OpenDoor, "Opendoor", 128, &ASCNum, 1, NULL);
                }
            }
        }
    }
}