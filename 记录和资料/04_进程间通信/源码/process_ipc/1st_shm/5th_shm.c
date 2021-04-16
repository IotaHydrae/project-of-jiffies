/**********************************************************************
 * 功能描述： 1.创建共享内存，将该共享内存地址通过shmat映射到用户地址空间
              2.用户通过标准输入向这个共享内存中输入一行字符串
              3.然后从该共享内存中读取内容
              4.调用shmdt解除地址映射，此时应用程序继续访问会出错
              5.最后调用shmctl函数删除内核中的共享内存
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

int main(int argc, char *argv[])
{
    int shmid;
    int key;
    char *p;
    
    key = ftok("./a.c", 'b');
    if (key < 0) {
        printf("create key fail\n");
        return -1;
    }
    printf("create key sucess key = 0x%X\n",key);
    
    shmid = shmget(key, 128, IPC_CREAT | 0777);
    if (shmid < 0) {
        printf("create shared memory fail\n");
        return -1;
    }
    printf("create shared memory sucess, shmid = %d\n", shmid);
    system("ipcs -m");
    
    p = (char *)shmat(shmid, NULL, 0);
    if (p == NULL) {
        printf("shmat fail\n");
        return -1;
    }
    printf("shmat sucess\n");
    
    //write share memory
    fgets(p, 128, stdin);
    
    //start read share memory
    printf("share memory data:%s\n", p);
    
    //start read share memory again
    printf("share memory data:%s\n", p);

    //在用户空间删除共享内存的地址
    shmdt(p);
    
    //memcpy(p, "abcd", 4);  //执行这个语句会出现segment fault
    
    shmctl(shmid, IPC_RMID, NULL);
    system("ipcs -m");
    return 0;
}
