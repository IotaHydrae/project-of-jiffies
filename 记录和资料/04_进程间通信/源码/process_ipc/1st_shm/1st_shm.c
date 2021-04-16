/**********************************************************************
 * 功能描述： 1.使用IPC_PRIVATE创建共享内存
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

int main(int argc, char *argv[])
{
    int shmid;
    
    shmid = shmget(IPC_PRIVATE, 128, 0777);
    if (shmid < 0) {
        printf("create shared memory fail\n");
        return -1;
    }
    printf("create shared memory sucess, shmid = %d\n", shmid);
    system("ipcs -m");
    return 0;
}
