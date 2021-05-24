1#include <stdio.h>
2#include <stdlib.h>
3#include <string.h>
4#include <sys/types.h>
5#include <sys/socket.h>
6#include <netinet/in.h>
7#include <arpa/inet.h>
8#include <unistd.h>
9#include <signal.h>
10
11#define SERVER_PORT 8180
12#define C_QUEUE     10 
13
14/************************************************************
15*函数功能描述：从8180端口接收客户端数据
16*输入参数：无
17*输出参数：打印客户IP以及发来的信息
18*返回值：无
19*修改日期		版本号		修改人		修改内容
20*2020/05/13		v1.0.0		zonghzha	reat
21*************************************************************/
22
23int main(int argc, char **argv)
24{
25    char buf[512];
26    int len;
27    int duty_socket;
28    int customer_socket;
29    struct sockaddr_in socket_server_addr;
30    struct sockaddr_in socket_client_addr;
31    int ret;
32    int addr_len;
33
34    signal(SIGCHLD, SIG_IGN);
35	
36	/* 服务器端开始建立socket描述符 */
37    duty_socket = socket(AF_INET, SOCK_STREAM, 0);
38    if (duty_socket == -1)
39    {
40        printf("socket error");
41        return -1;
42    }
43    
44	/* 服务器端填充 sockaddr_in结构 */
45    socket_server_addr.sin_family   = AF_INET;
46	/*端口号转换为网络字节序*/
47    socket_server_addr.sin_port     = htons(SERVER_PORT);
48	/*接收本机所有网口的数据*/
49    socket_server_addr.sin_addr.s_addr  = INADDR_ANY;
50    memset(socket_server_addr.sin_zero, 0, 8);
51    
52	/* 捆绑sockfd描述符 */
53    ret = bind(duty_socket, (const struct sockaddr *)&socket_server_addr, sizeof(struct sockaddr));
54    if (ret == -1)
55    {
56        printf("bind error!\n");
57        return -1;
58    }
59    ret = listen(duty_socket, C_QUEUE);
60    if (ret == -1)
61    {
62        printf("listen error!\n");
63        return -1;
64    }
65    
66    while (1)
67    {
68        addr_len = sizeof(struct sockaddr);
69		/* 服务器阻塞,直到客户程序建立连接 */
70        customer_socket = accept(duty_socket, (struct sockaddr *)&socket_client_addr, &addr_len);
71        if (customer_socket != -1)
72        {
73			/*inet_ntoa的作用是将一个32位Ipv4地址转换为相应的点分十进制数串*/
74            printf("Get connect from %s\n", inet_ntoa(socket_client_addr.sin_addr));
75        }
76        if (!fork())
77        {
78            while (1)
79            {
80                memset(buf, 512, 0);
81				/*接收数据*/
82                len = recv(customer_socket, buf, sizeof(buf), 0);
83                buf[len] = '\0';
84                if (len <= 0)
85                {
86                    close(customer_socket);
87                    return -1;
88                }
89                else
90                {
91                    printf("Get connect from %s, Msg is %s\n", inet_ntoa(socket_client_addr.sin_addr), buf);
92                }
93            }
94        }
95    }
96    
97    close(duty_socket);
98    return 0;
99}
