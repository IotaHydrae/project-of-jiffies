/**********************************************************************
 * 功能描述： 1.server打印client发送过来的字符串，并将该字符串回发给client
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
#include <sys/stat.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/un.h>

int main(int argc, char *argv[])
{
    int lfd ,ret, cfd;
    struct sockaddr_un serv, client;
    socklen_t len = sizeof(client);
    char buf[1024] = {0};
    int recvlen;

    //创建socket
    lfd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (lfd == -1) {
        perror("socket error");
        return -1;
    }

    //如果套接字文件存在，删除套接字文件
    unlink("server.sock");

    //初始化server信息
    serv.sun_family = AF_LOCAL;
    strcpy(serv.sun_path, "server.sock");

    //绑定
    ret = bind(lfd, (struct sockaddr *)&serv, sizeof(serv));
    if (ret == -1) {
        perror("bind error");
        return -1;
    }

    //设置监听，设置能够同时和服务端连接的客户端数量
    ret = listen(lfd, 36);
    if (ret == -1) {
        perror("listen error");
        return -1;
    }

    //等待客户端连接
    cfd = accept(lfd, (struct sockaddr *)&client, &len);
    if (cfd == -1) {
        perror("accept error");
        return -1;
    }
    printf("=====client bind file:%s\n", client.sun_path);
    
    while (1) {
        recvlen = recv(cfd, buf, sizeof(buf), 0);
        if (recvlen == -1) {
            perror("recv error");
            return -1;
        } else if (recvlen == 0) {
            printf("client disconnet...\n");
            close(cfd);
            break;
        } else {
            printf("server recv buf: %s\n", buf);
            send(cfd, buf, recvlen, 0);
        }
    }
    
    close(cfd);
    close(lfd);
    return 0;
}
