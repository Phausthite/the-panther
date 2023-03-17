#define _WINSOCK_DEPRECATED_NO_WARNINGS//应该是禁止API警告

#include <WinSock2.h>//windows版<sys/socket.h>

#include <stdio.h>
#include <stdlib.h>//内存分配/释放函数的头文件（Standard Library）

#pragma comment(lib, "ws2_32.lib")//调用库"ws2_32.lib"

void main()
{
    WSADATA wsaData;//用于存储WSAStartup函数返回的socket数据
    int port;//端口
    char Hello[] = "Hello,I'm a repeater.";
    int Protocol=0;
    char P ='N';

    printf("Customize port number:");
    scanf_s("%d", &port);


    printf("Will receiving UDP packets be supported?( input 'Y' or 'N' )\n");
    scanf_s(" %c", &P,2);
    if (P == 'Y') {
        Protocol = 0; printf("Receiving UDP packets will be supported!\n");
    }
    else if(P == 'N') {
        Protocol = IPPROTO_TCP; printf("Receiving UDP packets will not be supported!\n");
    }

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)//调用2.2版Winsock
    {
        printf("\nFailed to load Winsock:%d\n", WSAGetLastError());
        return;
    }
    
    //创建用于监听的套接字
    SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, Protocol);
    if (SOCKET_ERROR == sockSrv) {
        printf("\nSocket() error:%d\n", WSAGetLastError());
        return;
    }

    SOCKADDR_IN addrSrv;//ip地址结构体
    addrSrv.sin_family = AF_INET;//协议族
    addrSrv.sin_port = htons(port); //端口号
    addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//ip地址//hton函数把数据从主机序转换成网络序

    //绑定套接字
    int retVal = bind(sockSrv, (LPSOCKADDR)&addrSrv, sizeof(SOCKADDR_IN));//把特定地址赋给socket
    if (retVal == SOCKET_ERROR) {
        printf("\nBind failed:%d\n", WSAGetLastError());
        return;
    }
    if (listen(sockSrv, 10) == SOCKET_ERROR) {
        printf("\nListen failed:%d\n", WSAGetLastError());
        return;
    }

    SOCKADDR_IN addrClient;//客户端地址
    int len = sizeof(SOCKADDR);
    printf("\nWaiting...\n");

    //等待客户请求到来    
    SOCKET sockConn = accept(sockSrv, (SOCKADDR*)&addrClient, &len);
    if (sockConn == SOCKET_ERROR) {
        printf("\nAccept failed:%d\n", WSAGetLastError());
        return;
    }

    printf("\nConnection established!\nClient IP:[%s]\n", inet_ntoa(addrClient.sin_addr));

    //打招呼
    int iSend = send(sockConn, Hello, sizeof(Hello), Protocol);
    if (iSend == SOCKET_ERROR) {
        printf("\nSend failed:%d\n", WSAGetLastError());
        return;
    }
    
    //接收数据
    char recvBuf[1024];
    memset(recvBuf, 0, sizeof(recvBuf));
    int rec=recv(sockConn, recvBuf, sizeof(recvBuf), Protocol);
    if (rec != SOCKET_ERROR) {
        printf("\nReceived message: %s\n", recvBuf);
    }
    
    //复读
    int resp = send(sockConn, recvBuf, sizeof(recvBuf), Protocol);
    if (resp == SOCKET_ERROR) {
        printf("\nSend failed:%d\n", WSAGetLastError());
        return;
    }
    
    closesocket(sockConn);

    closesocket(sockSrv);
    WSACleanup();
    system("pause");

}