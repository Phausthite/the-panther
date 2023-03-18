#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

void main()
{
    //加载套接字
    WSADATA wsaData;
    char buff[1024];
    memset(buff, 0, sizeof(buff));

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("\nFailed to load Winsock:%d\n", WSAGetLastError());
        return;
    }

    //自定义端口
    int port;
    char ip_address_str[16] = { 0 };
    printf("Customize port number:");
    scanf_s("%d", &port);
    
    //自定义ip地址
    printf("Customize send address:");
    scanf_s("%s", &ip_address_str,16);
    // 将点分十进制字符串转换为 in_addr 结构体
    struct in_addr ip_address;
    ip_address.s_addr = inet_addr(ip_address_str);
    if (ip_address.s_addr == INADDR_NONE) {
        printf("\nInvalid IP address:%d\n", WSAGetLastError());
        return;
    }
    
    SOCKADDR_IN addrSrv;
    addrSrv.sin_family = AF_INET;
    addrSrv.sin_port = htons(port);
    addrSrv.sin_addr.S_un.S_addr = ip_address.s_addr;

    //创建套接字
    SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);
    if (SOCKET_ERROR == sockClient) {
        printf("\nSocket() error:%d\n", WSAGetLastError());
        return;
    }

    //向服务器发出连接请求
    SOCKET sockConn = connect(sockClient, (struct  sockaddr*)&addrSrv, sizeof(addrSrv));
    if (sockConn == INVALID_SOCKET) {
        printf("\nConnect failed:%d\n", WSAGetLastError());
        return;
    }
    //打招呼
    int iRec=recv(sockClient, buff, sizeof(buff), 0);
    if (iRec != SOCKET_ERROR) {
        printf("\nReceived message: %s\n", buff);
    }
    

    //自定义发送协议
    int Protocol = 0; char P;
    printf("\nWhich protocol would you like to use for sending packets?(input 'T' (TCP) or 'U' (UDP) )\n");
    scanf_s(" %c", &P, 2);
    if (P == 'T') {
        Protocol = 0; printf("TCP protocol will be used!\n");
    }
    else if (P == 'U') {
        Protocol = IPPROTO_UDP; printf("UDP protocol will be used!\n");
    }
    
    //发送数据
    char buffSend[1024];
    printf("\nPlease input what you want to send:\n");
    memset(buffSend, 0, sizeof(buffSend));
    scanf_s("%s", buffSend, 1024);
    int SndMsg = sendto(sockClient, buffSend, sizeof(buffSend), 0, (struct sockaddr*)&addrSrv, sizeof(addrSrv));
    if (SndMsg == SOCKET_ERROR) {
        printf("\nSend failed:%d\n", WSAGetLastError());
        return;
    }

    //接收服务器回应
    char buffRecv[1024];
    memset(buffRecv, 0, sizeof(buffRecv));
    int Rev = recv(sockClient, buffRecv, sizeof(buffRecv), 0);
    if (Rev != SOCKET_ERROR) {
        printf("\nReceived message: %s\n", buffRecv);
    }
    
    //关闭套接字
    closesocket(sockClient);
    WSACleanup();
    system("pause");
}
