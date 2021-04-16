1#include <stdio.h>
2#include <stdlib.h>
3#include <string.h>
4#include <sys/socket.h>
5#include <netinet/in.h>
6#include <arpa/inet.h>
7#include <unistd.h>
8
9#define SERVER_PORT 8180
10
11/************************************************************
12*函数功能描述：向指定IP的8180端口发送数据
13*输入参数：点分十进制服务器IP
14*输出参数：无
15*返回值：无
16*修改日期			版本号			修改人			修改内容
17*2020/05/13			v1.0.0			zonghzha		creat
18*************************************************************/
19
20int main(int argc, char **argv)
21{
22    unsigned char buf[512];
23    int len;
24    struct sockaddr_in socket_server_addr;
25    int ret;
26    int addr_len;
27    int client_socket;
28
29    
30    if (argc != 2)
31    {
32        printf("Usage:\n");
33        printf("%s <server_ip>\n", argv[0]);
34        return -1;
35    }
36    
37    /*创建套接字*/
38    client_socket = socket(AF_INET, SOCK_DGRAM, 0);
39    if (client_socket == -1)
40    {
41        printf("socket error");
42        return -1;
43    }
44    
45	/* 填充服务端的资料 */
46    socket_server_addr.sin_family   = AF_INET;
47    socket_server_addr.sin_port     = htons(SERVER_PORT);
48    if (inet_aton(argv[1], &socket_server_addr.sin_addr) == 0)
49    {
50        printf("invalid server ip\n");
51        return -1;
52    }
53    memset(socket_server_addr.sin_zero, 0, 8);
54    
55
56
57    
58    while (1)
59    {
60        if (fgets(buf, sizeof(buf), stdin))
61        {
62 //           len = send(client_socket, buf, strlen(buf), 0);
63			/*向服务器端发送数据报*/
64            addr_len = sizeof(struct sockaddr);
65            len = sendto(client_socket, buf, sizeof(buf), 0, (struct sockaddr *)&socket_server_addr, addr_len);
66            if (len <= 0)
67            {
68                close(client_socket);
69                return -1;
70            }
71        }
72    }
73    
74    close(client_socket);
75    return 0;
76}
77
