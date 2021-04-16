/**********************************************************************
 * 功能描述： 1.client进程使用ftok生成一个key值，利用这个key值打开一个共享内存
              2.client进程收到server进程发送过来的信号之后，开始读取共享内存
              3.子进程读完共享内存后，发送信号通知父进程读取完成
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

    signal(SIGUSR1, myfun);
    p = (struct mybuf *)shmat(shmid, NULL, 0);
    if (p == NULL) {
        printf("shmat fail\n");
        return -1;
    }
    printf("client process shmat sucess\n");
    
    // get server pid
    //read share memory
    pid = p->pid;
    // write client pid to share memory
    p->pid = getpid();
    kill(pid, SIGUSR2);   // tell server process to read data
    
    //client start to read share memory

    while (1) {
        pause();              // wait server process write share memory
        printf("client process read data:%s\n", p->buf); // read data
        kill(pid, SIGUSR2);   // server can  write share memory
    }

    //在用户空间删除共享内存的地址
    shmdt(p);
    
    shmctl(shmid, IPC_RMID, NULL);
    system("ipcs -m");
    return 0;
}
