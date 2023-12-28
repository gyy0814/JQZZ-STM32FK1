//
// Created by 26913 on 2023/12/14.
//

#ifndef FKS1_USER_H
#define FKS1_USER_H


#include "usart.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "main.h"
#include "cmsis_os.h"
#include "music.h"

#define TO_BIT(num) (1<<(num%32))

#define ASC(num) (num-1)
#define LOOK(num) (num+25)
#endif //FKS1_USER_H