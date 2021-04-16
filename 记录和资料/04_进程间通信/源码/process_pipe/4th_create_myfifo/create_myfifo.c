/**********************************************************************
 * 功能描述： 1.创建一个有名管道
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
    int ret;

    ret = mkfifo("./myfifo", 0777);   //创建有名管道，文件权限为777
    if (ret < 0) {
        printf("create myfifo fail\n");
        return -1;
    }
    printf("create myfifo sucess\n");
    
    return 0;
}
