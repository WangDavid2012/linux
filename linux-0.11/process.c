#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/waits.h>


#define HZ 100

void cupio_bound(int last,int cpu_time,int io_time);


/// @brief 试验四使用的函数，添加在linux-0.11目录下面，具体功能暂时还不是特别清楚
/// @param 参数描述
/// @return 返回值描述
/// @retval 返回值1 返回值1描述
/// @retval 返回值2 返回值2描述
/// @remarks 注意事项
/// @note 注意事项，功能同@remarks,显示字样不同
/// @par 自定义图块，后面可跟示例代码之类
/// @code(必须使用@endcode结束)
/// 示例代码(无需缩进)
/// @endcode
/// @see 其他参考项【产生指向参考的链接】
/// @author david 2018/12/25


///执行过程如下:
///< ./run 运行
///<  根文件系统下会有process.c 文件
///< 编译process.c文件
///< 运行process ./process
///< sync 命令
///< 查看log文件
int main(int argc,char * argv[])
{
    pid_t pid;
    int i = 0;
    for(i=0;i<3;i++)
    {
        pid = fork();
        if(pid<0)
            printf("error in fork\n");
        else if(pid == 0)
        {
            printf("process id is %d\n",getpid());
            cupio_bound(10,i,10-i);
            return;
        }
    }
    wait(NULL);
    wait(NULL);
    wait(NULL);
    return 0;
}

void cupio_bound(int last,int cpu_time,int io_time)
{
    struct tms start_time,current_time;
    clock_t utime,stime;
    int sleep_time;

    while(last>0)
    {
        times(&start_time);
        do
        {
            times(&current_time);
            utime = current_time.tms_utime - start_time.tms_utime;
            stime = current_time.tms_stime - start_time.tms_stime;
        }while((utime+stime)/HZ < cpu_time);
        last -= cpu_time;
        if(last <= 0)
            break;
        
        ///< IO burst 用sleep(1)模拟1秒钟的I/O操作
        sleep_time = 0;
        while(sleep_time < io_time)
        {
            sleep(1);
            sleep_time++;
            
        }
        last -= sleep_time;
        
    }

}











