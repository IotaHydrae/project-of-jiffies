/**********************************************************************
 * 功能描述： 1.进程1中创建一个有名管道3rd_fifo，权限是0777
              2.以写方式打开这个有名管道文件，并向其中写入一个值
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
#include <fcntl.h>

int main(int argc, char *argv[])
{
    int i, ret, fd;
    char p_flag = 0;

    /* 创建有名管道 */
    if (access("./3rd_fifo", 0) < 0) {   //先判断有名管道文件是否存在,不存在需要先创建
        ret = mkfifo("./3rd_fifo", 0777);
        if (ret < 0) {
            printf("create named pipe fail\n");
            return -1;
        }
        printf("create named pipe sucess\n");
    }
    
    /* 打开有名管道，以写方式打开 */
    fd=open("./3rd_fifo", O_WRONLY);
    if (fd < 0) {
        printf("open 3rd_fifo fail\n");
        return -1;
    }
    printf("open 3rd_fifo sucess\n");

    for (i = 0; i < 5; i++) {
        printf("this is first process i=%d\n", i);
        usleep(100);
    }
    p_flag = 1;
    sleep(5);
    write(fd, &p_flag, sizeof(p_flag));

    while(1);
    return 0;
}

