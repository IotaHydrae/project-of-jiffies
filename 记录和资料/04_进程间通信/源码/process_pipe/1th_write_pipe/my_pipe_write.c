/**********************************************************************
 * 功能描述： 创建一个管道，并向管道中写入字符串，然后从管道中读取，验证
              能否读取之前写入的字符串
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

int main(int argc, char *argv[])
{
    int fd[2];
    int ret = 0;
    char write_buf[] = "Hello linux";
    char read_buf[128] = {0};
    
    ret = pipe(fd);
    if (ret < 0) {
        printf("create pipe fail\n");
        return -1;
    }
    printf("create pipe sucess fd[0]=%d fd[1]=%d\n", fd[0], fd[1]);
    
    //向文件描述符fd[1]写管道
    write(fd[1], write_buf, sizeof(write_buf));
    
    //从文件描述符fd[0]读管道
    read(fd[0], read_buf, sizeof(read_buf));
    printf("read_buf=%s\n", read_buf);
    
    close(fd[0]);
    close(fd[1]);
    return 0;
}
