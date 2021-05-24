Linux下TCP编程示例
----


TCP服务端
----
`(socket)`open a socket
socket函数原型
```c
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>

int socket(int domain, int type, int protocol);
```
参数解释见man手册
返回值：成功将返回一个文件描述符；失败则返回-1，同时设置errno

`(bind)`name the socket
```c

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>

int bind(int sockfd, const struct sockaddr *addr,
socklen_t addrlen);
```
返回值：成功将返回0；失败将返回-1，同时设置errno
这里接收了一个名为`sockaddr`的结构体，其定义在`<sys/socket.h>`如下

```c
struct sockaddr {
    sa_family_t sa_family;	//地址族
    char 		sa_data[14];	//14字节，包含套字节中的目标地址和端口信息
}
```
`sockaddr`缺陷的是：sa_data把目标地址和端口信息
通常用`sockaddr_in`结构体来代替`sockaddr`其定义在`<netinet/in.h>`或`<arpa/inet.h>`，该结构体解决了`sockaddr`的缺陷，把`port`和`addr`分开储存在两个变量中，如下：
```c
struct sockaddr_in {
    sa_family_t 	sin_family;	//地址族
    uint16_t		sin_port;	//16位TCP/UDP端口号
    struct in_addr	sin_addr;	//32位IP地址
    char			sin_zero;	//不使用
}
//该结构体中提到的另一个结构体in_addr定义如下，他用来存放32位地址。
struct in_addr {
    In_addr_t		s_addr;		//32位IPv4地址
}
/*
sin_port和sin_addr都必须是网络字节序(NBO)，一般可视化的数字都是主机字节序(HBO)
这里用到几个函数
*/
htons()将端口号由主机字节序转换为网络字节序的整数值
inet_addr()作用是将一个IP字符串转化为一个网络字节序的整数值，用于sockaddr_in.sin_addr.s_addr。
inet_ntoa()作用是讲一个sin_addr结构体输出成IP字符串（network to ascii）
htonl()和htons()的作用一样，前者针对32位 后者针对16位
与两者相反的函数是ntohl()和ntohs()。

```
两者长度一样，都是16个字节，即占用的内存大小是一致的，因此可以互相转化。二者是并列结构，指向`sockaddr_in`的结构体的指针也可以指向`sockaddr`。

三种给`socket`赋值`地址`的方法

```c
1. inet_aton(server_addr_string, &my_addr.sin_addr);
2. my_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
3. INADDR_ANY转不转NBO随意
   my_addr.sin_addr.s_addr = htons(INADDR_ANY);
   my_addr.sin_addr.s_addr = INADDR_ANY;
```

两种给`socket`赋值`端口`
```c
1. #define MYPORT 3490
   my_addr.sin_port = htons(MYPORT);

2. 0（随机端口）转不转NBO随意
   my_addr.sin_port = htons(0);
   my_addr.sin_port = 0;
```

`(listen)`listen for incoming connections
```c
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>

int listen(int sockfd, int backlog);
```
返回值：成功返回0 失败返回-1同时设置errno
backlog参数定义socket的挂起连接队列可能增长到的最大长度。如果连接请求在队列已满时到达，则客户端可能会收到一个错误，如果底层协议支持从船，则可以忽略请求，异变稍后在连接处重新尝试成功。

`(accept)`accept client connections
函数原型
```c
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>

int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

#define _GNU_SOURCE             /* See feature_test_macros(7) */
#include <sys/socket.h>

int accept4(int sockfd, struct sockaddr *addr,
socklen_t *addrlen, int flags);
```
若使服务器能够接收多个客户端请求，则需要双重for循环，accept函数应处于外层循环中，如果客户端请求到来，则应该调用fork产生子进程，在子进程中循环调用recv函数读取客户端传来的数据。
该函数传入的sockaddr结构体是客户端的参数，客户端既可以传入sockaddr也可以传入sockaddr_in，这是因为他们在内存中的长度都是一样的，但是建议使用`struct sockaddr *`手动进行类型强制转换

`(send revice)`send and receive data
`(close)`close connections



```c
#include <sys/types.h> /* See NOTES */
#include <sys/socket.h>
#include <errno.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#define SERVER_PORT 9998
#define handle_error(msg)   \
    do                      \
    {                       \
        perror(msg);        \
        exit(EXIT_FAILURE); \
    } while (0)
#define BUFSIZE 256
/* 
server:
    (socket)open a socket
    (bind)name the socket
    (listen)listen for incoming connections
    (accept)accept client connections
    (send revice)send and receive data
    (close)close connections
 */
    /* code */

int main(int argc, char const *argv[])
{
    /* code */
    int ret = 0;
    int len = 0;
    int server_socket;
    int client_socket;
    int cid = 0;
    int pid;
    unsigned int client_addr_size;
    char msg_buf[BUFSIZE] = {0};
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;

    /* int socket(int domain, int type,int protocol); */
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        perror("error on socket");
        exit(EXIT_FAILURE);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT);
    memset(server_addr.sin_zero, 0, sizeof(server_addr.sin_zero));

    ret = bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0)
    {
        perror("error on bind");
        exit(EXIT_FAILURE);
    }

    ret = listen(server_socket, 10);
    if (ret < 0)
    {
        perror("error on listen");
        exit(EXIT_FAILURE);
    }

    while (1)
    { /* loop 1 */
        client_addr_size = sizeof(client_addr);
        client_socket = accept(server_socket,
                               (struct sockaddr *)&client_addr,
                               &client_addr_size);
        if (client_socket < 0)
        {
            perror("error on accpet");
        }
        else
        {
            printf("Get connections from client-%d@%s\n",
                   cid,
                   inet_ntoa(client_addr.sin_addr));
            cid++;
            pid = fork();
            if (pid == 0)
            { /* child */
                while (1)
                { /* child loop */
                    len = recv(client_socket, msg_buf, sizeof(msg_buf), 0);
                    if (len <= 0)
                    { /* On error */
                        close(client_socket);
                        return -1;
                    }
                    else
                    {
                        printf("Get message from client-%d: %s\n",
                               cid,
                               msg_buf);
                    }
                }
            }
        }
    }

    return 0;
}
```
> 2021-4-27 14:50

TCP客户端程序
----
TCP客户端与服务端建立连接进行数据传输要经过以下几个步骤
socket创建tcp套字节
设置sockaddr_in结构体
调用connect函数与服务端进行连接
在确定连接成功后即可通过send函数向服务端发送数据
发送完毕后调用close函数关闭客户端套字节