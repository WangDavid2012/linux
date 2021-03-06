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
**********************************************************************************************************/
tTask printerTask;
tTask protocolTask;
tTask tcpipTask;
tTask guiTask;
tTaskStack printerTaskEnv[1024];
tTaskStack protocolTaskEnv[1024];
tTaskStack tcpipTaskEnv[1024];
tTaskStack guiTaskEnv[1024];

tSem keyProcSem;

void printerTaskEntry (void * param)
{
    tSetSysTickPeriod(10);

    // 初始没有按键事件发生，所以计数为0
    tSemInit(&keyProcSem, 0, 0);
    for (;;)
    {
        tTaskDelay(1);
    }
}

void protocolTaskEntry (void * param)
{

    for (;;)
    {
        tTaskDelay(1);
    }
}

void tcpipTaskEntry (void * param)
{
    for (;;)
    {
        tTaskDelay(1);
    }
}

void guiTaskEntry (void * param)
{
    for (;;)
    {
        // 等待按键事件
        tSemWait(&keyProcSem, 0);

        // 处理按键事件
        // .... 一些代码 ....
    }
}

// 模拟按键中断处理的任务。在实际应用中，对应按键中断处理服务函数
tTask keySimulatorTask;
tTaskStack keySimulatorTaskEnv[1024];
void keySimulatorTaskEntry (void * param)
{
    for (;;)
    {
        // 模拟按键中断，每个tick产生一次
        tSemNotify(&keyProcSem);
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
    tTaskInit(&printerTask, printerTaskEntry, (void *)0, 0, &printerTaskEnv[1024]);
    tTaskInit(&protocolTask, protocolTaskEntry, (void *)0, 1, &protocolTaskEnv[1024]);
    tTaskInit(&tcpipTask, tcpipTaskEntry, (void *)0, 2, &tcpipTaskEnv[1024]);
    tTaskInit(&guiTask, guiTaskEntry, (void *)0, 3, &guiTaskEnv[1024]);
    tTaskInit(&keySimulatorTask, keySimulatorTaskEntry, (void *)0, 1, &keySimulatorTaskEnv[1024]);
}
