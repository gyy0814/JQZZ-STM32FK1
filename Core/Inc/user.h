//
// Created by 26913 on 2023/12/14.
//

#ifndef FKS1_USER_H
#define FKS1_USER_H

#endif //FKS1_USER_H

#include "usart.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "main.h"
#include "cmsis_os.h"
#include "music.h"

#define I0_UP_EVENT (1<<0)
#define I1_UP_EVENT (1<<1)

void StartUserTask(void const * argument);
void Startuser1Task(void const * argument);