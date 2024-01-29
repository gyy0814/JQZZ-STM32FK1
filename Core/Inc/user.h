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
//#include "stdbool.h"

typedef struct {
    uint8_t num;
    uint8_t Data;
} GameMessage;
void GameInit(void);

#define TO_BIT(num) (1<<(num%24))

/*****************************/
static int HP = 16;
#define HP_PIN_START 23
/*****************************/

#define LIGHT_A 28
#define LIGHT_B 29
#define LIGHT_C 30
#define LIGHT_D 31
#define LIGHT_E 32

#define KEY_A 23
#define KEY_B 24
#define KEY_C 23
#define KEY_D 23
#define KEY_E 23
#endif //FKS1_USER_H