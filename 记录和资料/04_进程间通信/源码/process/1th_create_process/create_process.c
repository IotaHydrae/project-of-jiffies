/**********************************************************************
 * 功能描述： 创建一个子进程
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 无
 * 修改日期        版本号     修改人          修改内容
 * -----------------------------------------------
 * 2020/05/16       V1.0      zh(ryan)        创建
 ***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    pid_t pid;

    pid = fork();  // 创建子进程

    if (pid == 0) {  // 子进程
        int i = 0;
        for (i = 0; i < 5; i++) {
            usleep(100);
            printf("this is child process i=%d\n", i);
        }
    }
    
    if (pid > 0) {  // 父进程
        int i = 0;
        for (i = 0; i < 5; i++) {
            usleep(100);
            printf("this is parent process i=%d\n", i);
        }
    }

    while(1);   //不让进程结束，以便我们查看进程的一些状态信息
    return 0;
}