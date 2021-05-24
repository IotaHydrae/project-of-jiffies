/**********************************************************************
 * 功能描述： 1.捕捉终端发送过来的信号
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
#include <signal.h>

void handler(int signo)
{
 	switch (signo) {
		case SIGINT:
			printf("I have got SIGINT\n");
			break;
			
		case SIGQUIT:
			printf("I have got SIGQUIT\n");
			break;
			
		default:
			printf("don't respond to this signal[%d]\n", signo);
			exit(0);
	}
}

int main(int argc, char *argv[])
{
	signal(SIGINT, handler);
	signal(SIGQUIT, handler);
	while (1)
		pause();
	return 0;
}

