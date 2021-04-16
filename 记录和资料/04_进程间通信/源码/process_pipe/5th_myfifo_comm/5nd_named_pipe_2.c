/**********************************************************************
 * 功能描述： 1.只读方式打开这个有名管道文件，并读取这个值
              2.当这个值非零时，继续执行后面的打印输出语句
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
    int i;
    int fd=open("./3rd_fifo", O_RDONLY);
    char p_flag = 0;
    
    if (fd < 0) {
        printf("open 3rd_fifo fail\n");
        return -1;
    }
    
    printf("open 3rd_fifo sucess\n");
    read(fd, &p_flag, sizeof(p_flag));
    while(!p_flag);
    for (i = 0; i < 5; i++) {
        printf("this is second process i=%d\n", i);
        usleep(100);
    }

    while(1);
    return 0;
}
