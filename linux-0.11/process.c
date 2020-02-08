#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/waits.h>


#define HZ 100

void cupio_bound(int last,int cpu_time,int io_time);


/// @brief ������ʹ�õĺ����������linux-0.11Ŀ¼���棬���幦����ʱ�������ر����
/// @param ��������
/// @return ����ֵ����
/// @retval ����ֵ1 ����ֵ1����
/// @retval ����ֵ2 ����ֵ2����
/// @remarks ע������
/// @note ע���������ͬ@remarks,��ʾ������ͬ
/// @par �Զ���ͼ�飬����ɸ�ʾ������֮��
/// @code(����ʹ��@endcode����)
/// ʾ������(��������)
/// @endcode
/// @see �����ο������ָ��ο������ӡ�
/// @author david 2018/12/25


///ִ�й�������:
///< ./run ����
///<  ���ļ�ϵͳ�»���process.c �ļ�
///< ����process.c�ļ�
///< ����process ./process
///< sync ����
///< �鿴log�ļ�
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
        
        ///< IO burst ��sleep(1)ģ��1���ӵ�I/O����
        sleep_time = 0;
        while(sleep_time < io_time)
        {
            sleep(1);
            sleep_time++;
            
        }
        last -= sleep_time;
        
    }

}











