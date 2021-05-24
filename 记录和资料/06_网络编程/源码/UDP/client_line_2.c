1#include <stdio.h>
2#include <stdlib.h>
3#include <string.h>
4#include <sys/socket.h>
5#include <netinet/in.h>
6#include <arpa/inet.h>
7#include <unistd.h>
8
9/*服务器端口为8180*/
10#define SERVER_PORT 8180
11
12/************************************************************
13*函数功能描述：向指定IP的8180端口发送数据
14*输入参数：点分十进制服务器IP
15*输出参数：无
16*返回值：无
17*修改日期			版本号			修改人			修改内容
18*2020/05/13			v1.0.0			zonghzha		creat
19*************************************************************/
20
21int main(int argc, char **argv)
22{
23    unsigned char buf[512];
24    int len;
25    struct sockaddr_in socket_server_addr;
26    int ret;
27    int addr_len;
28    int client_socket;
29
30    
31    if (argc != 2)
32    {
33        printf("Usage:\n");
34        printf("%s <server_ip>\n", argv[0]);
35        return -1;
36    }
37    
38    /*创建数据报套接字*/
39    client_socket = socket(AF_INET, SOCK_DGRAM, 0);
40    if (client_socket == -1)
41    {
42        printf("socket error");
43        return -1;
44    }
45    
46    socket_server_addr.sin_family   = AF_INET;
47    socket_server_addr.sin_port     = htons(SERVER_PORT);
48    if (inet_aton(argv[1], &socket_server_addr.sin_addr) == 0)
49    {
50        printf("invalid server ip\n");
51        return -1;
52    }
53    memset(socket_server_addr.sin_zero, 0, 8);
54    
55    ret = connect(client_socket, (const struct sockaddr *)&socket_server_addr, sizeof(struct sockaddr));
56    if (ret == -1)
57    {
58        printf("connect error!\n");
59        return -1;
60    }
61
62    
63    while (1)
64    {
65        if (fgets(buf, sizeof(buf), stdin))
66        {
67            len = send(client_socket, buf, strlen(buf), 0);
68            if (len <= 0)
69            {
70                close(client_socket);
71                return -1;
72            }
73        }
74    }
75    
76    close(client_socket);
77    return 0;
78}
79
