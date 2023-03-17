#define _WINSOCK_DEPRECATED_NO_WARNINGS//Ӧ���ǽ�ֹAPI����

#include <WinSock2.h>//windows��<sys/socket.h>

#include <stdio.h>
#include <stdlib.h>//�ڴ����/�ͷź�����ͷ�ļ���Standard Library��

#pragma comment(lib, "ws2_32.lib")//���ÿ�"ws2_32.lib"

void main()
{
    WSADATA wsaData;//���ڴ洢WSAStartup�������ص�socket����
    int port;//�˿�
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

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)//����2.2��Winsock
    {
        printf("\nFailed to load Winsock:%d\n", WSAGetLastError());
        return;
    }
    
    //�������ڼ������׽���
    SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, Protocol);
    if (SOCKET_ERROR == sockSrv) {
        printf("\nSocket() error:%d\n", WSAGetLastError());
        return;
    }

    SOCKADDR_IN addrSrv;//ip��ַ�ṹ��
    addrSrv.sin_family = AF_INET;//Э����
    addrSrv.sin_port = htons(port); //�˿ں�
    addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//ip��ַ//hton���������ݴ�������ת����������

    //���׽���
    int retVal = bind(sockSrv, (LPSOCKADDR)&addrSrv, sizeof(SOCKADDR_IN));//���ض���ַ����socket
    if (retVal == SOCKET_ERROR) {
        printf("\nBind failed:%d\n", WSAGetLastError());
        return;
    }
    if (listen(sockSrv, 10) == SOCKET_ERROR) {
        printf("\nListen failed:%d\n", WSAGetLastError());
        return;
    }

    SOCKADDR_IN addrClient;//�ͻ��˵�ַ
    int len = sizeof(SOCKADDR);
    printf("\nWaiting...\n");

    //�ȴ��ͻ�������    
    SOCKET sockConn = accept(sockSrv, (SOCKADDR*)&addrClient, &len);
    if (sockConn == SOCKET_ERROR) {
        printf("\nAccept failed:%d\n", WSAGetLastError());
        return;
    }

    printf("\nConnection established!\nClient IP:[%s]\n", inet_ntoa(addrClient.sin_addr));

    //���к�
    int iSend = send(sockConn, Hello, sizeof(Hello), Protocol);
    if (iSend == SOCKET_ERROR) {
        printf("\nSend failed:%d\n", WSAGetLastError());
        return;
    }
    
    //��������
    char recvBuf[1024];
    memset(recvBuf, 0, sizeof(recvBuf));
    int rec=recv(sockConn, recvBuf, sizeof(recvBuf), Protocol);
    if (rec != SOCKET_ERROR) {
        printf("\nReceived message: %s\n", recvBuf);
    }
    
    //����
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