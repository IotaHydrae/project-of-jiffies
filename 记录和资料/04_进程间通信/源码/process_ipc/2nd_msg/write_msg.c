/**********************************************************************
 * 功能描述： 1.server进程向消息队列中写数据
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
#include <sys/msg.h>
#include <signal.h>
#include <string.h>

struct msgbuf {
    long type;        //消息类型
    char voltage[124];     //消息正文
    char ID[4];
};

int main(int argc, char *argv[])
{
    int msgid, readret, key;
    struct msgbuf sendbuf;

    key = ftok("./a.c", 'a');
    if (key < 0){
        printf("create key fail\n");
        return -1;
    }
    msgid = msgget(key, IPC_CREAT|0777);
    if (msgid < 0) {
        printf("create msg queue fail\n");
        return -1;
    }
    printf("create msg queue sucess, msgid = %d\n", msgid);
    system("ipcs -q");

    // write message queue
    sendbuf.type = 100;
    while(1) {
        memset(sendbuf.voltage, 0, 124);  //clear send buffer
        printf("please input message:");
        fgets(sendbuf.voltage, 124, stdin);
        //start write msg to msg queue
        msgsnd(msgid, (void *)&sendbuf, strlen(sendbuf.voltage), 0);
    }

    return 0;
}
