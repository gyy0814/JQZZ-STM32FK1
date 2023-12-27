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

#define I0_UP_EVENT (1<<0)
#define I1_UP_EVENT (1<<1)

#define 窗继电器(num) ((num) + 95)
#define 射灯(num) ((num) + 83)

#define TO_BIT(num) (1<<(num%32))

#define L_OFF 0
#define L_RED 1
#define L_GRN 2

// 窗2 拆弹机关
#define C2I_ZS1 2   // 正方炸弹1成功信号
#define C2I_ZF1 3   // 正方炸弹1失败信号
#define C2I_ZS2 4   // 正方炸弹2成功信号
#define C2I_ZF2 5   // 正方炸弹2失败信号
#define C2I_ZS3 6   // 正方炸弹3成功信号
#define C2I_ZF3 7   // 正方炸弹3失败信号
#define C2I_FS1 8   // 反方炸弹1成功信号
#define C2I_FF1 9   // 反方炸弹1失败信号
#define C2I_FS2 10  // 反方炸弹2成功信号
#define C2I_FF2 11  // 反方炸弹2失败信号
#define C2I_FS3 12  // 反方炸弹3成功信号
#define C2I_FF3 13  // 反方炸弹3失败信号
#define C2O_Z201 30 // 正方炸弹1延时信号
#define C2O_Z202 31 // 正方炸弹2延时信号
#define C2O_Z203 32 // 正方炸弹3延时信号
#define C2O_ZY   33 // 正方烟机控制信号
#define C2O_F201 34 // 反方炸弹1延时信号
#define C2O_F202 35 // 反方炸弹2延时信号
#define C2O_F203 36 // 反方炸弹3延时信号
#define C2O_FY   37 // 反方烟机控制信号
#define C2O_ZL   38 // 正方电磁锁
#define C20_FL   39 // 反方电磁锁

// 窗3 迷宫机关
#define C3I_ZS 14 // 正方过关
#define C3I_FS 15 // 反方过关
#define C3O_ZL 40 // 正方电磁锁
#define C3O_FL 41 // 反方电磁锁

// 窗4 拼图机关
#define C4I_ZS 16
#define C4I_FS 17
#define C4O_L 42

// 窗5 枪械机关
#define C5I_ZS 18
#define C5I_FS 19
#define C5O_ZT 43
#define C5O_FT 44
#define C5O_L 45


// 窗6 迷宫机关
#define C6I_ZS 20 // 正方过关
#define C6I_FS 21 // 反方过关
#define C6O_ZL 46 // 正方电磁锁
#define C6O_FL 47 // 反方电磁锁

// 窗7 拼图机关
#define C7I_ZS 22
#define C7I_FS 23
#define C7O_L 48

// 窗8 枪械机关
#define C8I_ZS 24
#define C8I_FS 25
#define C8O_ZT 49
#define C8O_FT 50
#define C8O_L 51

// 窗10 迷宫机关
#define C10I_ZS 38 // 正方过关
#define C10I_FS 39 // 反方过关
#define C10O_ZL 62 // 正方电磁锁
#define C10O_FL 63 // 反方电磁锁

// 窗11 拼图机关
#define C11I_ZS 40
#define C11I_FS 41
#define C11O_L 64

// 窗12 枪械机关
#define C12I_ZS 42
#define C12I_FS 43
#define C12O_ZT 65
#define C12O_FT 66
#define C12O_L 67


void CreateTask(int taskId);

void DestroyTask(int taskId);

#endif //FKS1_USER_H