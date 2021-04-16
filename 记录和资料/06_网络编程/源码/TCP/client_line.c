1#include <stdio.h>
2#include <stdlib.h>
3#include <string.h>
4#include <sys/types.h>
5#include <sys/socket.h>
6#include <netinet/in.h>
7#include <arpa/inet.h>
8#include <unistd.h>
9
10#define SERVER_PORT 8180
11/************************************************************
12*函数功能描述：向指定IP的8180端口发送数据
13*输入参数：点分十进制服务器IP
14*输出参数：无
15*返回值：无
16*修改日期		版本号		修改人		修改内容
17*2020/05/13		v1.0.0		zonghzha	creat
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
37    /* 客户程序开始建立 sockfd描述符 */
38    client_socket = socket(AF_INET, SOCK_STREAM, 0);
39    if (client_socket == -1)
40    {
41        printf("socket error");
42        return -1;
43    }
44    
45	/* 客户程序填充服务端的资料 */
46    socket_server_addr.sin_family   = AF_INET;
47	/*主机字节序转换为网络字节序*/
48    socket_server_addr.sin_port     = htons(SERVER_PORT);
49    if (inet_aton(argv[1], &socket_server_addr.sin_addr) == 0)
50    {
51        printf("invalid server ip\n");
52        return -1;
53    }
54    memset(socket_server_addr.sin_zero, 0, 8);
55    /* 客户程序发起连接请求 */
56    ret = connect(client_socket, (const struct sockaddr *)&socket_server_addr, sizeof(struct sockaddr));
57    if (ret == -1)
58    {
59        printf("connect error!\n");
60        return -1;
61    }
62
63    
64    while (1)
65    {
66        if (fgets(buf, sizeof(buf), stdin))
67        {
68            len = send(client_socket, buf, strlen(buf), 0);
69            if (len <= 0)
70            {
71                close(client_socket);
72                return -1;
73            }
74        }
75    }
76    
77    close(client_socket);
78    return 0;
79}
