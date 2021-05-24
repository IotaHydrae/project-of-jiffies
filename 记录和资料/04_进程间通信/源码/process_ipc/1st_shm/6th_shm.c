/**********************************************************************
 * 功能描述： 1.在父进程中创建使用key值为IPC_PRIVATE创建一个共享内存
              2.然后在父进程中创建一个子进程
              3.通过标准输入，父进程向共享内存中写入字符串
              4.父进程调用发送信号函数通知子进程可以读取共享内存了
              5.子进程收到父进程发送过来的信号，开始读取共享内存
              6.子进程读完共享内存后，发送信号通知父进程读取完成
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
#include <sys/shm.h>
#include <signal.h>
#include <string.h>

void myfun(int signum)
{
    return;
}

int main(int argc, char *argv[])
{
    int shmid;
    int key;
    char *p;
    int pid;


    shmid = shmget(IPC_PRIVATE, 128, IPC_CREAT | 0777);
    if (shmid < 0) {
        printf("create shared memory fail\n");
        return -1;
    }
    printf("create shared memory sucess, shmid = %d\n", shmid);

    pid = fork();
    if (pid > 0) {    // 父进程
        signal(SIGUSR2, myfun);
        p = (char *)shmat(shmid, NULL, 0);
        if (p == NULL) {
            printf("shmat fail\n");
            return -1;
        }
        printf("parent process shmat sucess\n");
        while (1) {
            //从标准输入获取字符串，将其写入到共享内存
            printf("parent process begin to write memory data:");
            fgets(p, 128, stdin);
            kill(pid, SIGUSR1);   // 发信号通知子进程读共享内存
            pause();              // 等待子进程读完共享内存的信号
        }
    }
    if (pid == 0) { // 子进程
        signal(SIGUSR1, myfun);
        p = (char *)shmat(shmid, NULL, 0);
        if (p == NULL) {
            printf("shmat fail\n");
            return -1;
        }
        printf("child process shmat sucess\n");
        while (1) {
            pause(); // 等待父进程发信号，准备读取共享内存
            //子进程开始读共享内存，并发信号给父进程告知读完成
            printf("child process read share memory data:%s\n", p);
            kill(getppid(), SIGUSR2);
        }
    }

    //在用户空间删除共享内存的地址
    shmdt(p);
    
    //memcpy(p, "abcd", 4);  //执行这个语句会出现segment fault
    
    shmctl(shmid, IPC_RMID, NULL);
    system("ipcs -m");
    return 0;
}

