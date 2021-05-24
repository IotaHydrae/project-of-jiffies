/**********************************************************************
 * 功能描述： 1.client进程从消息队列中读数据
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
    int msgid, key;
    struct msgbuf readbuf;

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

    // read message queue
    while(1) {
        memset(readbuf.voltage, 0, 124);  //clear recv buffer
        //start read msg to msg queue
        msgrcv(msgid, (void *)&readbuf, 124, 100, 0);
        printf("recv data from message queue:%s", readbuf.voltage);
    }

    return 0;
}

