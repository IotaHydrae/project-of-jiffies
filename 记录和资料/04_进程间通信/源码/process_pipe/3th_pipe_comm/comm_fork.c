/**********************************************************************
 * 功能描述： 1.使用无名管道实现父子进程通信
              2.父进程向管道中写入一个值
              3.子进程从管道中读取这个值，如果非零，则执行后面的打印，否则不执行
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
    char process_inter = 0;
    int fd[2], ret = 0;

    ret = pipe(fd);   //创建一个无名管道，必须在创建子进程之前
    if (ret < 0) {
        printf("create pipe fail\n");
        return -1;
    }
    printf("create pipe sucess\n");
    
    pid = fork();  //创建子进程

    if (pid == 0) {  // 子进程
        int i = 0;
        read(fd[0], &process_inter, sizeof(process_inter));  // 如果管道为空，则休眠等待
        while (process_inter == 0);
        for (i = 0; i < 5; i++) {
            usleep(100);
            printf("this is child process i=%d\n", i);
        }
    } else if (pid > 0) {  // 父进程
        int i = 0;
        for (i = 0; i < 5; i++) {
            usleep(100);
            printf("this is parent process i=%d\n", i);
        }
        process_inter = 1;
        sleep(2);
        write(fd[1], &process_inter, sizeof(process_inter));
    }

    while(1);
    return 0;
}