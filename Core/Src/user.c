//
// Created by 26913 on 2023/12/14.
//
//

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "user.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "cmsis_os.h"
#include "gpio.h"

#define GameTaskNum 1
int gameFlags[GameTaskNum] = {100};
int saomiaoFlags[20] = {0};
SemaphoreHandle_t xGameSemaphore[GameTaskNum];

extern QueueHandle_t GameMessageQueueHandle;
extern EventGroupHandle_t InputEventGroup[(INPUT_NUM / 24) + 1];
extern EventGroupHandle_t MusicEventGroup;
extern QueueHandle_t OutputMessageQueueHandle;
bool ASCTaskHandle[9];
TaskHandle_t BlinkTaskHandle = NULL;

void GameInit(void) {
    for (int i = 0; i < GameTaskNum; i++)
        xGameSemaphore[i] = xSemaphoreCreateMutex();
}

void StartGameFlagTask(void const *argument) {
    GameMessage newMessage;
    for (;;) {
        if (xQueueReceive(GameMessageQueueHandle, &newMessage, portMAX_DELAY) == pdTRUE) {
            xSemaphoreTake(xGameSemaphore[newMessage.num], portMAX_DELAY);
            gameFlags[newMessage.num] = newMessage.Data;
            xSemaphoreGive(xGameSemaphore[newMessage.num]);
        }
    }
}


#define GameTimeReset RunTime = 0

bool GameDelay(int *pvRunTime, int waitTime) {
    if (waitTime < *pvRunTime) {
        *pvRunTime = 0;
        return true;
    }
    return false;
}

void StartGameTask(void const *argument) {
    int RunTime = 0;
    for (;;) {
        xSemaphoreTake(xGameSemaphore[0], portMAX_DELAY);
        if (gameFlags[0] == 0)//第一次按下遥控
        {
            EventBits_t bits = xEventGroupWaitBits(InputEventGroup[0], TO_BIT(艾琳开始), pdTRUE, pdTRUE, 0);
            if (bits & TO_BIT(艾琳开始)) {
                gameFlags[0]++;
            }
        }
        if (gameFlags[0] == 1)//艾琳开始音频
        {
            SetOutput(电视背景灯带, GPIO_PIN_SET);
            osDelay(13000);
            SetOutput(电视背景灯带, GPIO_PIN_RESET);
            gameFlags[0]++;
        }
        if (gameFlags[0] == 2)//播放bgm开场,播放语音,这就是王晗雨的家了,开灯
        {
            char *fileName1 = "/02.mp3";
            PlayMusicName(&MUSIC_1, fileName1, strlen(fileName1), 单曲循环);
            char *fileName2 = "/03.mp3";
            PlayMusicName(&MUSIC_2, fileName2, strlen(fileName2), 单曲停止);
            SetOutput(电视背景灯带, GPIO_PIN_SET);
            SetOutput(客厅房顶灯带, GPIO_PIN_SET);
            gameFlags[0]++;
        }
        if (gameFlags[0] == 3)//等待检测遥控器1
        {
            EventBits_t bits = xEventGroupWaitBits(InputEventGroup[0], TO_BIT(遥控器1), pdTRUE, pdTRUE, 0);
            if (bits & TO_BIT(遥控器1)) {
                gameFlags[0]++;
            }
        }
        if (gameFlags[0] == 4)//检测遥控器1 电视开启播放待机画面雪花
        {
            SetOutput(电视信号1, GPIO_PIN_SET);
            osDelay(1000);
            SetOutput(电视信号1, GPIO_PIN_RESET);
            gameFlags[0]++;
        }
        if (gameFlags[0] >= 5 && gameFlags[0] <= 27)//4 个按钮，按不同的按钮会播放不同的视频
        {
            EventBits_t bits1 = xEventGroupWaitBits(InputEventGroup[0], TO_BIT(遥控器2), pdTRUE, pdTRUE, 0);
            if (bits1 & TO_BIT(遥控器2)) {
                SetOutput(电视信号2, GPIO_PIN_SET);
                char *fileName = "/05.mp3";
                PlayMusicName(&MUSIC_2, fileName, strlen(fileName), 单曲停止);
                osDelay(1000);
                SetOutput(电视信号2, GPIO_PIN_RESET);
            }
            EventBits_t bits2 = xEventGroupWaitBits(InputEventGroup[0], TO_BIT(遥控器3), pdTRUE, pdTRUE, 0);
            if (bits2 & TO_BIT(遥控器3)) {
                SetOutput(电视信号3, GPIO_PIN_SET);
                char *fileName = "/06.mp3";
                PlayMusicName(&MUSIC_2, fileName, strlen(fileName), 单曲停止);
                osDelay(1000);
                SetOutput(电视信号3, GPIO_PIN_RESET);
            }
            EventBits_t bits3 = xEventGroupWaitBits(InputEventGroup[0], TO_BIT(遥控器4), pdTRUE, pdTRUE, 0);
            if (bits3 & TO_BIT(遥控器4)) {
                SetOutput(电视信号4, GPIO_PIN_SET);
                char *fileName = "/11.mp3";
                PlayMusicName(&MUSIC_2, fileName, strlen(fileName), 单曲停止);
                osDelay(1000);
                SetOutput(电视信号4, GPIO_PIN_RESET);
            }
            EventBits_t bits4 = xEventGroupWaitBits(InputEventGroup[0], TO_BIT(遥控器5), pdTRUE, pdTRUE, 0);
            if (bits4 & TO_BIT(遥控器5)) {
                SetOutput(电视信号5, GPIO_PIN_SET);
                char *fileName = "/12.mp3";
                PlayMusicName(&MUSIC_2, fileName, strlen(fileName), 单曲停止);
                osDelay(1000);
                SetOutput(电视信号5, GPIO_PIN_RESET);
            }
        }
        if (gameFlags[0] == 5)//等待打电话
        {
            EventBits_t bits1 = xEventGroupWaitBits(InputEventGroup[0], TO_BIT(打电话), pdTRUE, pdTRUE, 0);
            if (bits1 & TO_BIT(打电话)) {
                gameFlags[0]++;
            }
        }
        if (gameFlags[0] == 6)//等待卧室密码器
        {
            EventBits_t bits1 = xEventGroupWaitBits(InputEventGroup[0], TO_BIT(卧室密码器), pdTRUE, pdTRUE, 0);
            if (bits1 & TO_BIT(卧室密码器)) {
                gameFlags[0]++;
            }
        }
        if (gameFlags[0] == 7)//卧室密码器
        {
            char *fileName = "/14.mp3";
            PlayMusicName(&MUSIC_1, fileName, strlen(fileName), 单曲循环);
            SetOutput(卧室门锁, GPIO_PIN_RESET);
            SetOutput(卧室月球灯, GPIO_PIN_SET);
            SetOutput(卧室灯带, GPIO_PIN_SET);
            gameFlags[0]++;
        }
        if (gameFlags[0] == 8)//延时触发语音
        {
            char *fileName = "/15.mp3";
            osDelay(40000);
            PlayMusicName(&MUSIC_2, fileName, strlen(fileName), 单曲停止);
            gameFlags[0]++;
        }
        if (gameFlags[0] == 9)//等待卧室抽屉打开
        {
            EventBits_t bits1 = xEventGroupWaitBits(InputEventGroup[0], TO_BIT(卧室抽屉打开), pdTRUE, pdTRUE, 0);
            if (bits1 & TO_BIT(卧室抽屉打开)) {
                gameFlags[0]++;
            }
        }
        if (gameFlags[0] == 10)//卧室抽屉打开
        {
            char *fileName = "/16.mp3";
            PlayMusicName(&MUSIC_2, fileName, strlen(fileName), 单曲停止);
            osDelay(40000);
            SetOutput(卧室柜上灯, GPIO_PIN_SET);
            gameFlags[0]++;
        }
        if (gameFlags[0] == 11)//等待录取通知
        {
            EventBits_t bits1 = xEventGroupWaitBits(InputEventGroup[0], TO_BIT(录取通知), pdTRUE, pdTRUE, 0);
            if (bits1 & TO_BIT(录取通知)) {
                gameFlags[0]++;
            }
        }
        if (gameFlags[0] == 12)//录取通知
        {
            SetOutput(接电话信号, GPIO_PIN_SET);
            osDelay(1000);
            SetOutput(接电话信号, GPIO_PIN_RESET);
            gameFlags[0]++;
        }
        if (gameFlags[0] == 13)//延时开柜子
        {
            osDelay(30000);
            char *fileName = "/4.mp3";
            PlayMusicName(&MUSIC_2, fileName, strlen(fileName), 单曲停止);
            SetOutput(卧室柜锁, GPIO_PIN_RESET);
            SetOutput(卧室柜下灯, GPIO_PIN_SET);
            gameFlags[0]++;
        }
        if (gameFlags[0] == 14)//等待记忆卡拿走
        {
            EventBits_t bits1 = xEventGroupWaitBits(InputEventGroup[0], TO_BIT(记忆卡拿走), pdTRUE, pdTRUE, 0);
            if (bits1 & TO_BIT(记忆卡拿走)) {
                gameFlags[0]++;
            }
        }
        if (gameFlags[0] == 15)//记忆卡拿走
        {
            char *fileName = "/18.mp3";
            PlayMusicName(&MUSIC_2, fileName, strlen(fileName), 单曲停止);
            gameFlags[0]++;
        }
        if (gameFlags[0] == 16)//等待记忆卡刷卡1
        {
            EventBits_t bits1 = xEventGroupWaitBits(InputEventGroup[0], TO_BIT(记忆卡刷卡1), pdTRUE, pdTRUE, 0);
            if (bits1 & TO_BIT(记忆卡刷卡1)) {
                gameFlags[0]++;
            }
        }
        if (gameFlags[0] == 17)//记忆卡刷卡1
        {
            SetOutput(电视信号6, GPIO_PIN_SET);
            char *fileName = "/19.mp3";
            PlayMusicName(&MUSIC_2, fileName, strlen(fileName), 单曲停止);
            osDelay(1000);
            SetOutput(电视信号6, GPIO_PIN_RESET);

        }
        if (gameFlags[0] == 18)//等待敲门信号
        {
            EventBits_t bits1 = xEventGroupWaitBits(InputEventGroup[0], TO_BIT(敲门信号), pdTRUE, pdTRUE, 0);
            if (bits1 & TO_BIT(敲门信号)) {
                gameFlags[0]++;
            }
        }
        if (gameFlags[0] == 19)//敲门信号
        {
            SetOutput(敲门锁, GPIO_PIN_RESET);
            char *fileName = "/20.mp3";
            PlayMusicName(&MUSIC_2, fileName, strlen(fileName), 单曲停止);
        }
        if (gameFlags[0] == 20)//等待数独刷卡
        {
            EventBits_t bits1 = xEventGroupWaitBits(InputEventGroup[0], TO_BIT(书房数独), pdTRUE, pdTRUE, 0);
            if (bits1 & TO_BIT(书房数独)) {
                gameFlags[0]++;
            }
        }
        if (gameFlags[0] == 21)//数独刷卡
        {
            char *fileName = "/21.mp3";
            PlayMusicName(&MUSIC_1, fileName, strlen(fileName), 单曲循环);
            SetOutput(书房门锁, GPIO_PIN_RESET);
            SetOutput(书房灯带, GPIO_PIN_SET);
            SetOutput(书房台灯, GPIO_PIN_SET);
            SetOutput(书房月球灯, GPIO_PIN_SET);
            gameFlags[0]++;
        }
        if (gameFlags[0] >= 22 && gameFlags[0] <= 27)//等待开关
        {
            EventBits_t bits1 = xEventGroupWaitBits(InputEventGroup[0], TO_BIT(开关面板), pdTRUE, pdTRUE, 0);
            if (bits1 & TO_BIT(开关面板)) {

//            }
//            static int a = 1;
//            if (((bits & TO_BIT(开关面板)) == 0) && (a == 0)) {
//                SetOutput(书房灯带, GPIO_PIN_RESET);
//                SetOutput(书房台灯, GPIO_PIN_RESET);
//                SetOutput(书房月球灯, GPIO_PIN_RESET);
//                a = 1;
//            }
//            if (bits & TO_BIT(开关面板)) {
//                a = 0;
//            }
            }
            if (gameFlags[0] == 22)//等待记忆卡2
            {
                EventBits_t bits1 = xEventGroupWaitBits(InputEventGroup[0], TO_BIT(记忆卡刷卡2), pdTRUE, pdTRUE, 0);
                if (bits1 & TO_BIT(记忆卡刷卡2)) {
                    gameFlags[0]++;
                }
            }
            if (gameFlags[0] == 23)//记忆卡2
            {
                //SetOutput(电视信号7, GPIO_PIN_SET);
                osDelay(1000);
                //SetOutput(电视信号7, GPIO_PIN_RESET);
                osDelay(43000);
                char *fileName = "/23.mp3";
                PlayMusicName(&MUSIC_2, fileName, strlen(fileName), 单曲停止);
                gameFlags[0]++;
            }
            if (gameFlags[0] == 24)//等待维修密码
            {
                EventBits_t bits1 = xEventGroupWaitBits(InputEventGroup[0], TO_BIT(快递柜密码器), pdTRUE, pdTRUE, 0);
                if (bits1 & TO_BIT(快递柜密码器)) {
                    gameFlags[0]++;
                }
            }
            if (gameFlags[0] == 25)//维修密码
            {
                SetOutput(线路轨门锁, GPIO_PIN_RESET);
                SetOutput(线路射灯, GPIO_PIN_SET);
                gameFlags[0]++;
            }
            if (gameFlags[0] == 26)//等待线路过关
            {
                EventBits_t bits1 = xEventGroupWaitBits(InputEventGroup[0], TO_BIT(线路过关), pdTRUE, pdTRUE, 0);
                if (bits1 & TO_BIT(线路过关)) {
                    gameFlags[0]++;
                }
            }
            if (gameFlags[0] == 27)//线路过关
            {
                char *fileName1 = "/25.mp3";
                PlayMusicName(&MUSIC_2, fileName1, strlen(fileName1), 单曲停止);
                osDelay(26000);
                char *fileName2 = "/26.mp3";
                PlayMusicName(&MUSIC_2, fileName2, strlen(fileName2), 单曲停止);
                SetOutput(快递柜门锁, GPIO_PIN_RESET);
                osDelay(6000);
                char *fileName3 = "/27.mp3";
                PlayMusicName(&MUSIC_2, fileName3, strlen(fileName3), 单曲停止);
                gameFlags[0]++;
            }
            if (gameFlags[0] == 28)//等待艾利刷卡
            {
                EventBits_t bits1 = xEventGroupWaitBits(InputEventGroup[0], TO_BIT(艾利刷卡), pdTRUE, pdTRUE, 0);
                if (bits1 & TO_BIT(艾利刷卡)) {
                    gameFlags[0]++;
                }
            }
            if (gameFlags[0] == 29)//艾利刷卡
            {
                PauseMusic(&MUSIC_1);
                char *fileName2 = "/29.mp3";
                PlayMusicName(&MUSIC_2, fileName2, strlen(fileName2), 单曲停止);

                SetOutput(卧室柜下灯, GPIO_PIN_RESET);
                SetOutput(卧室柜上灯, GPIO_PIN_RESET);
                SetOutput(书房灯带, GPIO_PIN_RESET);
                SetOutput(线路射灯, GPIO_PIN_RESET);
                SetOutput(书房月球灯, GPIO_PIN_RESET);
                SetOutput(电视背景灯带, GPIO_PIN_RESET);
                SetOutput(书房台灯, GPIO_PIN_RESET);
                SetOutput(卧室月球灯, GPIO_PIN_RESET);
                SetOutput(客厅房顶灯带, GPIO_PIN_RESET);
                SetOutput(卧室灯带, GPIO_PIN_RESET);


                osDelay(5000);
                SetOutput(扫描激光, GPIO_PIN_SET);
                osDelay(2000);
                SetOutput(扫描灯1, GPIO_PIN_SET);
                osDelay(2000);
                SetOutput(扫描灯2, GPIO_PIN_SET);
                osDelay(2000);
                SetOutput(扫描灯3, GPIO_PIN_SET);
                osDelay(2000);
                SetOutput(扫描灯4, GPIO_PIN_SET);
                osDelay(2000);
                SetOutput(扫描灯5, GPIO_PIN_SET);
                osDelay(2000);
                SetOutput(扫描灯6, GPIO_PIN_SET);
                osDelay(2000);
                SetOutput(扫描灯7, GPIO_PIN_SET);
                osDelay(2000);
                SetOutput(扫描灯8, GPIO_PIN_SET);
                osDelay(2000);
                SetOutput(扫描灯9, GPIO_PIN_SET);
                osDelay(2000);
                SetOutput(扫描灯10, GPIO_PIN_SET);

                SetOutput(扫描激光, GPIO_PIN_RESET);
                SetOutput(灯带, GPIO_PIN_SET);
                SetOutput(扫描仪门锁, GPIO_PIN_RESET);
                char *fileName1 = "/30.mp3";
                PlayMusicName(&MUSIC_1, fileName1, strlen(fileName1), 单曲循环);
                char *fileName3 = "/31.mp3";
                PlayMusicName(&MUSIC_2, fileName3, strlen(fileName3), 单曲停止);
                gameFlags[0]++;
            }
            if (gameFlags[0] == 30)//扫描仪
            {
                EventBits_t bits1 = xEventGroupWaitBits(InputEventGroup[0], TO_BIT(扫描仪1), pdTRUE, pdTRUE, 0);
                if (bits1 & TO_BIT(扫描仪1)) {
                    saomiaoFlags[1] = 1;
                    SetOutput(信息块1, GPIO_PIN_RESET);
                    SetOutput(扫描灯1, GPIO_PIN_RESET);
                    char *fileName1 = "/32.mp3";
                    PlayMusicName(&MUSIC_2, fileName1, strlen(fileName1), 单曲停止);
                }
                EventBits_t bits2 = xEventGroupWaitBits(InputEventGroup[0], TO_BIT(扫描仪2), pdTRUE, pdTRUE, 0);
                if (bits2 & TO_BIT(扫描仪1)) {
                    saomiaoFlags[2] = 1;
                    SetOutput(信息块2, GPIO_PIN_RESET);
                    SetOutput(扫描灯2, GPIO_PIN_RESET);
                    char *fileName1 = "/33.mp3";
                    PlayMusicName(&MUSIC_2, fileName1, strlen(fileName1), 单曲停止);
                }
                EventBits_t bits3 = xEventGroupWaitBits(InputEventGroup[0], TO_BIT(扫描仪3), pdTRUE, pdTRUE, 0);
                if (bits3 & TO_BIT(扫描仪3)) {
                    saomiaoFlags[3] = 1;
                    SetOutput(信息块3, GPIO_PIN_RESET);
                    SetOutput(扫描灯3, GPIO_PIN_RESET);
                    char *fileName1 = "/34.mp3";
                    PlayMusicName(&MUSIC_2, fileName1, strlen(fileName1), 单曲停止);
                }
                EventBits_t bits4 = xEventGroupWaitBits(InputEventGroup[0], TO_BIT(扫描仪4), pdTRUE, pdTRUE, 0);
                if (bits4 & TO_BIT(扫描仪4)) {
                    saomiaoFlags[4] = 1;
                    SetOutput(信息块4, GPIO_PIN_RESET);
                    SetOutput(扫描灯4, GPIO_PIN_RESET);
                    char *fileName1 = "/35.mp3";
                    PlayMusicName(&MUSIC_2, fileName1, strlen(fileName1), 单曲停止);
                }
                EventBits_t bits5 = xEventGroupWaitBits(InputEventGroup[0], TO_BIT(扫描仪5), pdTRUE, pdTRUE, 0);
                if (bits5 & TO_BIT(扫描仪5)) {
                    saomiaoFlags[5] = 1;
                    SetOutput(信息块5, GPIO_PIN_RESET);
                    SetOutput(扫描灯5, GPIO_PIN_RESET);
                    char *fileName1 = "/36.mp3";
                    PlayMusicName(&MUSIC_2, fileName1, strlen(fileName1), 单曲停止);
                }
                EventBits_t bits6 = xEventGroupWaitBits(InputEventGroup[0], TO_BIT(扫描仪6), pdTRUE, pdTRUE, 0);
                if (bits6 & TO_BIT(扫描仪6)) {
                    saomiaoFlags[6] = 1;
                    SetOutput(信息块6, GPIO_PIN_RESET);
                    SetOutput(扫描灯6, GPIO_PIN_RESET);
                    char *fileName1 = "/37.mp3";
                    PlayMusicName(&MUSIC_2, fileName1, strlen(fileName1), 单曲停止);
                }
                EventBits_t bits7 = xEventGroupWaitBits(InputEventGroup[0], TO_BIT(扫描仪7), pdTRUE, pdTRUE, 0);
                if (bits7 & TO_BIT(扫描仪7)) {
                    saomiaoFlags[7] = 1;
                    SetOutput(信息块7, GPIO_PIN_RESET);
                    SetOutput(扫描灯7, GPIO_PIN_RESET);
                    char *fileName1 = "/38.mp3";
                    PlayMusicName(&MUSIC_2, fileName1, strlen(fileName1), 单曲停止);
                }
                EventBits_t bits8 = xEventGroupWaitBits(InputEventGroup[0], TO_BIT(扫描仪8), pdTRUE, pdTRUE, 0);
                if (bits8 & TO_BIT(扫描仪8)) {
                    saomiaoFlags[8] = 1;
                    SetOutput(信息块8, GPIO_PIN_RESET);
                    SetOutput(扫描灯8, GPIO_PIN_RESET);
                    char *fileName1 = "/39.mp3";
                    PlayMusicName(&MUSIC_2, fileName1, strlen(fileName1), 单曲停止);
                }
                EventBits_t bits9 = xEventGroupWaitBits(InputEventGroup[0], TO_BIT(扫描仪9), pdTRUE, pdTRUE, 0);
                if (bits9 & TO_BIT(扫描仪9)) {
                    saomiaoFlags[9] = 1;
                    SetOutput(信息块9, GPIO_PIN_RESET);
                    SetOutput(扫描灯9, GPIO_PIN_RESET);
                    char *fileName1 = "/40.mp3";
                    PlayMusicName(&MUSIC_2, fileName1, strlen(fileName1), 单曲停止);
                }
                EventBits_t bits10 = xEventGroupWaitBits(InputEventGroup[0], TO_BIT(扫描仪10), pdTRUE, pdTRUE, 0);
                if (bits10 & TO_BIT(扫描仪10)) {
                    saomiaoFlags[10] = 1;
                    SetOutput(信息块10, GPIO_PIN_RESET);
                    SetOutput(扫描灯10, GPIO_PIN_RESET);
                    char *fileName1 = "/41.mp3";
                    PlayMusicName(&MUSIC_2, fileName1, strlen(fileName1), 单曲停止);
                }
                if (saomiaoFlags[1] == 1 &&
                    saomiaoFlags[2] == 1 &&
                    saomiaoFlags[3] == 1 &&
                    saomiaoFlags[4] == 1 &&
                    saomiaoFlags[5] == 1 &&
                    saomiaoFlags[6] == 1 &&
                    saomiaoFlags[7] == 1 &&
                    saomiaoFlags[8] == 1 &&
                    saomiaoFlags[9] == 1 &&
                    saomiaoFlags[10] == 1) {
                    gameFlags[0]++;

                }
            }
        }
        if (gameFlags[0] == 31)//十个扫描过关
        {
            osDelay(3000);
            char *fileName1 = "/42.mp3";
            PlayMusicName(&MUSIC_2, fileName1, strlen(fileName1), 单曲停止);
            osDelay(12000);
            SetOutput(信息投影灯, GPIO_PIN_SET);
            gameFlags[0]++;
        }
        if (gameFlags[0] == 32)//等待矮桌
        {
            EventBits_t bits = xEventGroupWaitBits(InputEventGroup[0], TO_BIT(矮桌), pdTRUE, pdTRUE, 0);
            if (bits & TO_BIT(矮桌)) {
                gameFlags[0]++;
            }
        }
        if (gameFlags[0] == 33)//矮桌过关
        {

            char *fileName1 = "/43.mp3";
            PlayMusicName(&MUSIC_2, fileName1, strlen(fileName1), 单曲停止);
            osDelay(7000);
            char *fileName2 = "/44.mp3";
            PlayMusicName(&MUSIC_2, fileName2, strlen(fileName2), 单曲停止);
            SetOutput(前场电脑触发, GPIO_PIN_SET);
            osDelay(1000);
            SetOutput(前场电脑触发, GPIO_PIN_RESET);

            osDelay(45000);
            char *fileName3 = "/45.mp3";
            PlayMusicName(&MUSIC_1, fileName3, strlen(fileName3), 单曲停止);
            char *fileName4 = "/46.mp3";
            PlayMusicName(&MUSIC_2, fileName4, strlen(fileName4), 单曲停止);
            osDelay(25000);
            SetOutput(电脑显示器, GPIO_PIN_RESET);
            SetOutput(信息投影灯, GPIO_PIN_RESET);
            gameFlags[0]++;
        }
        if (gameFlags[0] == 34)//空
        {

        }
        if (gameFlags[0] == 35)//复位
        {
            SetOutput(电视信号1, GPIO_PIN_RESET);
            SetOutput(电视信号2, GPIO_PIN_RESET);
            SetOutput(电视信号3, GPIO_PIN_RESET);
            SetOutput(电视信号4, GPIO_PIN_RESET);
            SetOutput(电视信号5, GPIO_PIN_RESET);
            SetOutput(电视信号6, GPIO_PIN_RESET);
            SetOutput(接电话信号, GPIO_PIN_RESET);
            SetOutput(卧室门锁, GPIO_PIN_SET);

            SetOutput(卧室柜锁, GPIO_PIN_SET);
            SetOutput(卧室柜下灯, GPIO_PIN_SET);
            SetOutput(卧室柜上灯, GPIO_PIN_SET);
            SetOutput(书房门锁, GPIO_PIN_SET);
            SetOutput(摄像头灯, GPIO_PIN_RESET);
            SetOutput(书房灯带, GPIO_PIN_SET);
            SetOutput(线路轨门锁, GPIO_PIN_SET);
            SetOutput(快递柜门锁, GPIO_PIN_SET);
            SetOutput(扫描仪门锁, GPIO_PIN_SET);
            SetOutput(信息块1, GPIO_PIN_SET);

            SetOutput(信息块2, GPIO_PIN_SET);
            SetOutput(信息块3, GPIO_PIN_SET);
            SetOutput(信息块4, GPIO_PIN_SET);
            SetOutput(信息块5, GPIO_PIN_SET);
            SetOutput(信息块6, GPIO_PIN_SET);
            SetOutput(信息块7, GPIO_PIN_SET);
            SetOutput(信息块8, GPIO_PIN_SET);
            SetOutput(信息块9, GPIO_PIN_SET);
            SetOutput(信息块10, GPIO_PIN_SET);

            SetOutput(灯带, GPIO_PIN_SET);
            SetOutput(线路射灯, GPIO_PIN_SET);
            SetOutput(扫描灯1, GPIO_PIN_RESET);
            SetOutput(扫描灯2, GPIO_PIN_RESET);
            SetOutput(扫描灯3, GPIO_PIN_RESET);
            SetOutput(扫描灯4, GPIO_PIN_RESET);
            SetOutput(扫描灯5, GPIO_PIN_RESET);
            SetOutput(扫描灯6, GPIO_PIN_RESET);
            SetOutput(扫描灯7, GPIO_PIN_RESET);
            SetOutput(扫描灯8, GPIO_PIN_RESET);

            SetOutput(扫描灯9, GPIO_PIN_RESET);
            SetOutput(扫描灯10, GPIO_PIN_RESET);
            SetOutput(扫描激光, GPIO_PIN_RESET);
            SetOutput(电脑显示器, GPIO_PIN_SET);
            SetOutput(书房月球灯, GPIO_PIN_SET);
            SetOutput(电视背景灯带, GPIO_PIN_SET);
            SetOutput(书房台灯, GPIO_PIN_SET);
            SetOutput(信息投影灯, GPIO_PIN_SET);
            SetOutput(卧室月球灯, GPIO_PIN_SET);
            SetOutput(客厅房顶灯带, GPIO_PIN_SET);
            SetOutput(卧室灯带, GPIO_PIN_SET);
            SetOutput(前场电脑复位, GPIO_PIN_SET);
            osDelay(1000);
            SetOutput(前场电脑复位, GPIO_PIN_RESET);
            gameFlags[0] = 34;
        }
        if (gameFlags[0] == 36)//复场
        {SetOutput(电视信号1, GPIO_PIN_RESET);
            SetOutput(电视信号2, GPIO_PIN_RESET);
            SetOutput(电视信号3, GPIO_PIN_RESET);
            SetOutput(电视信号4, GPIO_PIN_RESET);
            SetOutput(电视信号5, GPIO_PIN_RESET);
            SetOutput(电视信号6, GPIO_PIN_RESET);
            SetOutput(接电话信号, GPIO_PIN_RESET);
            SetOutput(卧室门锁, GPIO_PIN_SET);

            SetOutput(卧室柜锁, GPIO_PIN_SET);
            SetOutput(卧室柜下灯, GPIO_PIN_RESET);
            SetOutput(卧室柜上灯, GPIO_PIN_RESET);
            SetOutput(书房门锁, GPIO_PIN_SET);
            SetOutput(摄像头灯, GPIO_PIN_RESET);
            SetOutput(书房灯带, GPIO_PIN_RESET);
            SetOutput(线路轨门锁, GPIO_PIN_SET);
            SetOutput(快递柜门锁, GPIO_PIN_SET);
            SetOutput(扫描仪门锁, GPIO_PIN_SET);
            SetOutput(信息块1, GPIO_PIN_SET);

            SetOutput(信息块2, GPIO_PIN_SET);
            SetOutput(信息块3, GPIO_PIN_SET);
            SetOutput(信息块4, GPIO_PIN_SET);
            SetOutput(信息块5, GPIO_PIN_SET);
            SetOutput(信息块6, GPIO_PIN_SET);
            SetOutput(信息块7, GPIO_PIN_SET);
            SetOutput(信息块8, GPIO_PIN_SET);
            SetOutput(信息块9, GPIO_PIN_SET);
            SetOutput(信息块10, GPIO_PIN_SET);

            SetOutput(灯带, GPIO_PIN_RESET);
            SetOutput(线路射灯, GPIO_PIN_RESET);
            SetOutput(扫描灯1, GPIO_PIN_RESET);
            SetOutput(扫描灯2, GPIO_PIN_RESET);
            SetOutput(扫描灯3, GPIO_PIN_RESET);
            SetOutput(扫描灯4, GPIO_PIN_RESET);
            SetOutput(扫描灯5, GPIO_PIN_RESET);
            SetOutput(扫描灯6, GPIO_PIN_RESET);
            SetOutput(扫描灯7, GPIO_PIN_RESET);
            SetOutput(扫描灯8, GPIO_PIN_RESET);

            SetOutput(扫描灯9, GPIO_PIN_RESET);
            SetOutput(扫描灯10, GPIO_PIN_RESET);
            SetOutput(扫描激光, GPIO_PIN_RESET);
            SetOutput(电脑显示器, GPIO_PIN_SET);
            SetOutput(书房月球灯, GPIO_PIN_RESET);
            SetOutput(电视背景灯带, GPIO_PIN_RESET);
            SetOutput(书房台灯, GPIO_PIN_RESET);
            SetOutput(信息投影灯, GPIO_PIN_RESET);
            SetOutput(卧室月球灯, GPIO_PIN_RESET);
            SetOutput(客厅房顶灯带, GPIO_PIN_RESET);
            SetOutput(卧室灯带, GPIO_PIN_RESET);

            gameFlags[0] = 34;
        }
        //RunTime++;
        xSemaphoreGive(xGameSemaphore[0]);
        //osDelay(1); //等待音频播放
        static int oldGameFlag = 100;
        if (gameFlags[0] != oldGameFlag) {
            uint8_t TxBuff[5] = {0xCC, 0x05, 0x00, gameFlags[0], 0xFF};
            HAL_UART_Transmit(&huart1, TxBuff, 5, HAL_MAX_DELAY);
            oldGameFlag = gameFlags[0];
        }
    }
}
