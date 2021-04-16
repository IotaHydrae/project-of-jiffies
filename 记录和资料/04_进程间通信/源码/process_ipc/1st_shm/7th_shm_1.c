/**********************************************************************
 * 功能描述： 1.server进程使用ftok生成一个key值，利用这个key值创建一个共享内存
              2.通过标准输入，向共享内存中写入字符串
              3.server进程调用发送信号函数通知client进程
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

struct mybuf
{
    int pid;
    char buf[124];
};

void myfun(int signum)
{
    return;
}

int main(int argc, char *argv[])
{
    int shmid;
    int key;
    struct mybuf *p;
    int pid;

    key = ftok("./a.c", 'a');
    if (key < 0) {
        printf("create key fail\n");
        return -1;
    }
    printf("create key sucess\n");

    shmid = shmget(key, 128, IPC_CREAT | 0777);
    if (shmid < 0) {
        printf("create shared memory fail\n");
        return -1;
    }
    printf("create shared memory sucess, shmid = %d\n", shmid);

    signal(SIGUSR2, myfun);
    p = (struct mybuf *)shmat(shmid, NULL, 0);
    if (p == NULL) {
        printf("shmat fail\n");
        return -1;
    }
    printf("parent process shmat sucess\n");

    p->pid = getpid(); // 将server进程的pid号写入到共享内存
    pause();           // 等待client读取到server pid号
    pid=p->pid;        // 获取client的进程号

    while (1) {
        //write share memory
        printf("parent process begin to write memory data\n");
        fgets(p->buf, 124, stdin);
        kill(pid, SIGUSR1);   // 向client发送信号通知client读取共享内存数据
        pause();              // 等待client读取完共享内存数据
    }

    //在用户空间删除共享内存的地址
    shmdt(p);

    shmctl(shmid, IPC_RMID, NULL);
    system("ipcs -m");
    return 0;
}


