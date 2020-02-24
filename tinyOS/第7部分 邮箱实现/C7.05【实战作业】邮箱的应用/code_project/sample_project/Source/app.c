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

// 打印控制命令
typedef struct _printControlCmd
{
    uint32_t cmd;
    uint32_t arg;
}PrintControlCmd;

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

tMbox printCmdMbox;
PrintControlCmd * cmdBufferPtr[20];            // 命令指针缓冲区

// 模拟从tcpip中获取数据包，并解析出打印命令的接口
PrintControlCmd * getCmdFromTcpIp ()
{
    // 模拟实际，数据获取总是需要时间的
    tTaskDelay(1);

    // 因为是模拟，所以随便返回一个指针
    return (PrintControlCmd *) 1;
}

void printerTaskEntry (void * param)
{
    tSetSysTickPeriod(10);

    // 初始没有按键事件发生，所以计数为0
    tSemInit(&keyProcSem, 0, 0);

    tMboxInit(&printCmdMbox, (void **)cmdBufferPtr, sizeof(cmdBufferPtr) / sizeof(PrintControlCmd *));
    for (;;)
    {
        // 获取命令
        PrintControlCmd * cmd = (PrintControlCmd *)0;
        tMboxWait(&printCmdMbox, (void **)&cmd, 0);

        // ... 对命令进行一些处理....
    }
}

void protocolTaskEntry (void * param)
{
    for (;;)
    {
        // 从tcpip中获取数据包
        // 这里没有涉及到cmd的内存管理，将在在【存储块】的实战作业中解决
        PrintControlCmd * cmd = getCmdFromTcpIp();

        // .... 一些对cmd解析的过程，由于是模拟，所以什么也不做

        // 将包发给邮箱
        // 可以看到，由于notify是无等待的，当邮箱满时，会立即返回
        // 为避免命令丢失，任务需要等待邮箱不满后，将消息发出去，才能继续从tcpip获取数据包
        // 这个问题，在【存储块】的实战作业中解决
        tMboxNotify(&printCmdMbox, (void *)cmd, 0);
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


