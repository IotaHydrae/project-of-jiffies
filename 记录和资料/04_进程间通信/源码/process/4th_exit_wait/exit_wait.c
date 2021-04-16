/**********************************************************************
 * 功能描述： 使用exit退出子进程，父进程使用waitpid回收子进程的资源
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 无
 * 修改日期        版本号     修改人          修改内容
 * -----------------------------------------------
 * 2020/05/16       V1.0      zh(ryan)        创建
 ***********************************************************************/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	int status = -1;
	pid_t pid;

	pid = fork();
	if (pid == 0){ //子进程
		printf("fork\n");
		exit(1);
	} else if (pid > 0) { //父进程
		pid = waitpid(pid, &status, 0);
		printf("status=0x%x\n", status);
	} else {
		perror("fork\n");
	}

	return 0;
}
