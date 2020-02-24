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
** 有4个任务，i演示了事件标志的状态查询与删除
**********************************************************************************************************/
// 任务1和任务2的任务结构，以及用于堆栈空间
tTask tTask1;
tTask tTask2;
tTask tTask3;
tTask tTask4;
tTaskStack task1Env[1024];
tTaskStack task2Env[1024];
tTaskStack task3Env[1024];
tTaskStack task4Env[1024];

int task1Flag;

tMutex mutex;

void task1Entry (void * param) 
{
    tSetSysTickPeriod(10);

    // 初始化互斥信号量
    tMutexInit(&mutex);

    for (;;) 
    {
        tMutexWait(&mutex, 0);
        tMutexWait(&mutex, 0);

        task1Flag = 1;
        tTaskDelay(1);
        task1Flag = 0;
        tTaskDelay(1);

        tMutexNotify(&mutex);
        tMutexNotify(&mutex);
    }
}

int task2Flag;
void task2Entry (void * param) 
{
    for (;;)
    {
        tMutexWait(&mutex, 0);
        tMutexWait(&mutex, 0);

        // 当运行至此处时，由于互斥信号量的优先级继承机制
        // task2的优先级由1变成0
        task2Flag = 1;
        tTaskDelay(1);
        task2Flag = 0;
        tTaskDelay(1);

        tMutexNotify(&mutex);
        tMutexNotify(&mutex);
    }
}

int task3Flag;
void task3Entry (void * param) 
{
	for (;;)
    {
        task3Flag = 1;
        tTaskDelay(1);
        task3Flag = 0;
        tTaskDelay(1);
    }
}

int task4Flag;
void task4Entry (void * param) 
{
    for (;;) 
    {
        task4Flag = 1;
        tTaskDelay(1);
        task4Flag = 0;
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
    tTaskInit(&tTask4, task4Entry, (void *)0x44444444, 1, &task4Env[1024]);
}


