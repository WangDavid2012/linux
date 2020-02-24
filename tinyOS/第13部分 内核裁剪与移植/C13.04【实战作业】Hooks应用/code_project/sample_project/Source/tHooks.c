/*************************************** Copyright (c)******************************************************
** File name            :   tHooks.c
** Latest modified Date :   2016-06-01
** Latest Version       :   0.1
** Descriptions         :   tinyOS的Hooks扩展接口
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
#include "tHooks.h"
#include "tinyOS.h"
#include "ARMCM3.h"

#if TINYOS_ENABLE_HOOKS == 1

/**********************************************************************************************************
** Function name        :   tHooksCpuIdle
** Descriptions         :   cpu空闲时的hooks
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
void tHooksCpuIdle (void)
{
	// 进入低功耗模式
	__WFI();
}

/**********************************************************************************************************
** Function name        :   tHooksSysTick
** Descriptions         :   时钟节拍Hooks
** parameters           :   无
** Returned value       :   无
***********************************************************************************************************/
void tHooksSysTick (void)
{

}

/**********************************************************************************************************
** Function name        :   tHooksTaskSwitch
** Descriptions         :   任务切换hooks
** parameters           :   from 从哪个任务开始切换
** parameters           :   to 切换至哪个任务
** Returned value       :   无
***********************************************************************************************************/
void tHooksTaskSwitch (tTask * from, tTask * to)
{
	static uint32_t switchCount = 0;
	
	// 统计任务切换次数
	switchCount++;
}

/**********************************************************************************************************
** Function name        :   tHooksTaskInit
** Descriptions         :   任务初始化的Hooks
** parameters           :   task 等待初始化的任务
** Returned value       :   无
***********************************************************************************************************/
void tHooksTaskInit (tTask * task)
{

}

#endif
