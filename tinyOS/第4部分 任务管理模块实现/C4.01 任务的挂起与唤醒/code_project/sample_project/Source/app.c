/*************************************** Copyright (c)******************************************************
** File name            :   app.c
** Latest modified Date :   2016-06-01
** Latest Version       :   0.1
** Descriptions         :   应用代码
**
**--------------------------------------------------------------------------------------------------------
** Created by           :   01课堂 lishutong
** Created date         :   2016-06-01
** Version              :   1.0
** Descriptions         :   The original version
**
**--------------------------------------------------------------------------------------------------------
** Copyright            :   版权所有，禁止用于商业用途
** Website              :   http://course.ztsoftware.win
** Study Video          :   与代码相关的配套学习视频请见 http://study.163.com/u/01course
**********************************************************************************************************/
#include "tinyOS.h"

/**********************************************************************************************************
** 应用示例
** 有3个任务，分别执行task1Entry和task2Entry
** 其中task1Entry的优先级更高，在执行过程中有挂起自己，然后由task2去唤醒
**********************************************************************************************************/
// 任务1和任务2的任务结构，以及用于堆栈空间
tTask tTask1;
tTask tTask2;
tTask tTask3;
tTaskStack task1Env[1024];
tTaskStack task2Env[1024];
tTaskStack task3Env[1024];

int task1Flag;
void task1Entry (void * param) 
{
    tSetSysTickPeriod(10);

    for (;;) 
    {
        task1Flag = 1;
        tTaskSuspend(currentTask);
        task1Flag = 0;
        tTaskSuspend(currentTask);
    }
}

int task2Flag;
void task2Entry (void * param) 
{
    for (;;) 
    {
        task2Flag = 1;
        tTaskDelay(1);
        tTaskWakeUp(&tTask1);
        task2Flag = 0;
        tTaskDelay(1);
        tTaskWakeUp(&tTask1);
    }
}

int task3Flag;
void task3Entry (void * param) 
{
    for (;;) 
    {
        task3Flag = 0;
        tTaskDelay(1);
        task3Flag = 1;
        tTaskDelay(1);
    }
}

/**********************************************************************************************************
** Function name        :   tInitApp
** Descriptions         :   初始化应用接口
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
void tInitApp (void) 
{
    // 初始化任务1和任务2结构，传递运行的起始地址，想要给任意参数，以及运行堆栈空间
    tTaskInit(&tTask1, task1Entry, (void *)0x11111111, 0, &task1Env[1024]);
    tTaskInit(&tTask2, task2Entry, (void *)0x22222222, 1, &task2Env[1024]);
    tTaskInit(&tTask3, task3Entry, (void *)0x33333333, 1, &task3Env[1024]);
}


