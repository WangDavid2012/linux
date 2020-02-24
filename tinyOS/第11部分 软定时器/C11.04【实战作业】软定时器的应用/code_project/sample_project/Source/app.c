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

tMemBlock cmdMemBlock;
PrintControlCmd cmdBuffer[20];                 // 注意：数量要与cmdBufferPtr相同

tFlagGroup printerEventFlag;                    // 打印异常事件标志组

uint8_t shareBuffer[20];                        // 控制任务与协议处理任务共享的存储区域
tMutex shareBufferMutex;                        // 用于避免同时写的互斥信号量

// 用于按键消抖的定时器
tTimer keyTimer;

// 模拟从tcpip中获取数据包，并解析出打印命令的接口
PrintControlCmd * getCmdFromTcpIp ()
{
    static uint32_t cmdSeq = 0;

    PrintControlCmd * cmd = 0;

    // 模拟实际，数据获取总是需要时间的
    tTaskDelay(1);

    // 从邮箱中获取消息
    tMemBlockWait(&cmdMemBlock, (uint8_t **)&cmd, 0);

    // 模拟命令的不同，随便增加计数
    cmd->cmd = cmdSeq++;
    cmd->arg = cmdSeq++;

    // 因为是模拟，所以随便返回一个指针
    return cmd;
}

// 处理打开异常事件
void processEvent (uint32_t flag)
{

}

// 按键消抖的定时器
void keyTimerFun (void * arg)
{
    // 检查按键是否真的按下

    // 真的按下，发送信号量通知guiTask
    tSemNotify(&keyProcSem);
}

#define FLAG_NO_PAPER       (1 << 0)            // 缺纸事件
#define FLAG_COVER_OPEN     (1 << 1)            // 盖被打开
#define FLAG_TOO_HOT        (1 << 2)            // 打印头过热

void printerTaskEntry (void * param)
{
    tSetSysTickPeriod(10);

    // 初始没有按键事件发生，所以计数为0
    tSemInit(&keyProcSem, 0, 0);

    tMboxInit(&printCmdMbox, (void **)cmdBufferPtr, sizeof(cmdBufferPtr) / sizeof(PrintControlCmd *));
    tMemBlockInit(&cmdMemBlock, (uint8_t *)cmdBuffer, sizeof(PrintControlCmd), sizeof(cmdBuffer) / sizeof(PrintControlCmd));

    // 初始化打印异常事件标志组，没有任何事件发生
    tFlagGroupInit(&printerEventFlag, 0);

    tMutexInit(&shareBufferMutex);

    tTimerInit(&keyTimer, 2, 0, keyTimerFun, 0, TIMER_CONFIG_TYPE_SOFT);
    for (;;)
    {
        uint32_t resultFlag = 0;

        // 获取命令
        PrintControlCmd * cmd = (PrintControlCmd *)0;
        tMboxWait(&printCmdMbox, (void **)&cmd, 0);

        // ... 对命令进行一些处理....
        tMutexWait(&shareBufferMutex, 0);
        shareBuffer[0] = 0;
        tMutexNotify(&shareBufferMutex);

        // 打印过程中，定期对事件标志进行检查，如果发现事件发生，则立即停止打印
        tFlagGroupNoWaitGet(&printerEventFlag, TFLAGGROUP_SET_ANY|TFLAGGROUP_CONSUME,
                            FLAG_NO_PAPER|FLAG_COVER_OPEN|FLAG_TOO_HOT, &resultFlag);
        if (resultFlag != 0)
        {
            // 事件发生，停止打印，并做一些处理.....
            processEvent(resultFlag);
        }

        // 处理完后，回收掉的存储块
        tMemBlockNotify(&cmdMemBlock, (uint8_t *)cmd);
    }
}

void protocolTaskEntry (void * param)
{
    for (;;)
    {
        // 从tcpip中获取数据包
        // 由于使用了存储块，所以cmd的内存管理问题已经得到解决
        // getCmdFromTcpIp从存储块中获取存储块，而printerTaskEntry则负责释放
        PrintControlCmd * cmd = getCmdFromTcpIp();

        // .... 一些对cmd解析的过程，由于是模拟，所以什么也不做
        tMutexWait(&shareBufferMutex, 0);
        shareBuffer[0] = 1;
        tMutexNotify(&shareBufferMutex);

        // 将包发给邮箱
        // 当运行到这里时，邮箱必然不会为满，为什么 ？
        // 因为邮箱所能容纳的消息数和存储块最大的存储块数量一样
        // 如果邮箱满的话，那么存储块中的所以存储块都已经在邮箱里了，前面调用getCmdFromTcpIp必然会
        // 在调用tMemBlockWait等待，当获得存储块时，显然邮箱已经不为空了
        // 因而，这就解决了邮箱可能为满的问题，不会造成消息丢失
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
    static int seq = 0;
    uint32_t flag = 0;

    for (;;)
    {
        // 模拟按键中断，每个tick产生一次
        // 延是2个tick检查按键是否真的按下
        tTimerStart(&keyTimer);

        // 模拟打印异常事件
        switch (seq++ % 3)
        {
            case 0: flag = FLAG_NO_PAPER; break;
            case 1: flag = FLAG_COVER_OPEN; break;
            case 2: flag = FLAG_TOO_HOT; break;
        }
        tFlagGroupNotify(&printerEventFlag, 1, flag);

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


