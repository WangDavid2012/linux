/*************************************** Copyright (c)******************************************************
** File name            :   main.c
** Latest modified Date :   2016-06-01
** Latest Version       :   0.1
** Descriptions         :   主文件，包含应用代码
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
#include <stdio.h>
#include <stdlib.h>

void delay (int count) 
{
    while (--count > 0);
}

void LedFlagFuntion(int time)
{
    static int led =0xA5;
    led = 0x5A;
    delay(time);
    led = 0xCC;
    delay(time);
}



int flag;
int temp = 10;
int a = 0x12345678;
char b = 0;
int count = 0x98765432;
void funcA()
{
    //反汇编代码，会保存一些变量，然后在执行count++
    //执行完毕后
    //会将保存的变量恢复
    //david 2018/12/17 注释
    //0x00000250 4802      LDR      r0,[pc,#8]  ; @0x0000025C
    //0x00000252 6800      LDR      r0,[r0,#0x00]
    //0x00000254 1C40      ADDS     r0,r0,#1
    //0x00000256 4901      LDR      r1,[pc,#4]  ; @0x0000025C
    //0x00000258 6008      STR      r0,[r1,#0x00] 
    count++;
}
void funcB()
{
    funcA();
    count++;
}

void funcC()
{
    funcB();
    count++;
}
void funcD()
{
    funcC();
    count++;
}
int funcTest(int a,int b,int c,int d)
{
    count = a+b+c+d;
    return count;
}






 
typedef struct _Foo{
    int a;
    char b;
    double c;
    float d;
}Foo;
 
Foo test(int a, char b, double c, float d)
{
    Foo *f = (Foo *)malloc(sizeof(Foo));
    f->a = a;
    f->b = b;
    f->c = c;
    f->d = d;
    return *f;
}
 














int main () 
{
    int mm = 0xA5;
    Foo retvalue; 
    delay(100);
    LedFlagFuntion(100);
    b = ((char *)&a)[0];
    mm = funcTest(1,2,3,4);
    funcA();
    test(1,2,3,4);
    
    
    for (;;) {
        flag = 0;
        delay(100);
        flag = 1;
        delay(100);
        temp++;  
    }
    
    return 0;
}
