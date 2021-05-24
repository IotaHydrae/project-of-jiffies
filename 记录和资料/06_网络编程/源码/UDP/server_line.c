1#include <stdio.h>
2#include <stdlib.h>
3#include <string.h>
4//#include <sys/type.h>
5#include <sys/socket.h>
6#include <netinet/in.h>
7#include <arpa/inet.h>
8#include <unistd.h>
9#include <signal.h>
10
11/*服务器端口为8180*/
12#define SERVER_PORT 8180
13
14/************************************************************
15*函数功能描述：从8180端口接收客户端数据
16*输入参数：无
17*输出参数：打印客户IP以及发来的信息
18*返回值：无
19*修改日期			版本号			修改人			修改内容
20*2020/05/13			v1.0.0			zonghzha		creat
21*************************************************************/
22
23
24int main(int argc, char **argv)
25{
26    unsigned char buf[512];
27    int len;
28    int duty_socket;
29    int customer_socket;
30    struct sockaddr_in socket_server_addr;
31    struct sockaddr_in socket_client_addr;
32    int ret;
33    int addr_len;
34
35	/*创建数据报套接字*/
36    duty_socket = socket(AF_INET, SOCK_DGRAM, 0);
37    if (duty_socket == -1)
38    {
39        printf("socket error");
40        return -1;
41    }
42    
43	/* 服务器端填充 sockaddr_in结构 */
44    socket_server_addr.sin_family   = AF_INET;
45    socket_server_addr.sin_port     = htons(SERVER_PORT);
46    socket_server_addr.sin_addr.s_addr  = INADDR_ANY;
47    memset(socket_server_addr.sin_zero, 0, 8);
48    
49	/*绑定套接字*/
50    ret = bind(duty_socket, (const struct sockaddr *)&socket_server_addr, sizeof(struct sockaddr));
51    if (ret == -1)
52    {
53        printf("bind error!\n");
54        return -1;
55    }
56
57    
58    while (1)
59    {
60        addr_len = sizeof(struct sockaddr);
61		  /* 接收客户端数据报，返回的为接收到的字节数 */ 
62        len = recvfrom(duty_socket, buf, sizeof(buf), 0, (struct sockaddr *)&socket_client_addr, &addr_len);
63        if (len > 0)
64        {
65            buf[len] = '\0';
66            printf("Get Msg from %s : %s\n", inet_ntoa(socket_client_addr.sin_addr), buf);
67        }
68   
69    }
70    
71    close(duty_socket);
72    return 0;
73}
74
