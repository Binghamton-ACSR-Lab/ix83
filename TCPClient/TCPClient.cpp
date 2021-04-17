//  

#include "stdafx.h"
#include <WinSock2.h>
#include <stdio.h>
#include <string>
#include <iostream>
#define SERVER_PORT 8888 //服务器端口号  
#define BUFF_SIZE 2048
#pragma comment(lib, "WS2_32.lib")
using namespace std;
int _tmain()
{
	WORD wVersionRequested;
	WSAData wsaData;
	int err;
	printf("This is a Client side application!\n");
	wVersionRequested = MAKEWORD(2, 2);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		//Tell the user that we could not find a usable WinSock Dll.  
		printf("WSAStartup() called failed!\n");
		return -1;

	}
	else {
		printf("WSAStartup called successful!\n");

	}
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
		//Tell the user that we could not find a usable WinSock Dll.  
		WSACleanup();
		return -1;

	}
	//The WinSock Dll is acceptable. Proceed  
		 //创建套接字  
	SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);
	if (sockClient == INVALID_SOCKET) {
		printf("socket() called failed!, error code is %d\n", WSAGetLastError());
		return -1;

	}
	else {
		printf("socket() called successful!\n");

	}
	//需要连接的服务端套接字结构信息  
	SOCKADDR_IN addrServer;
	//设定服务器IP  
	addrServer.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	addrServer.sin_family = AF_INET;
	//设定服务器的端口号(使用网络字节序)   
	addrServer.sin_port = htons(SERVER_PORT);
	//向服务器发出连接请求  
	err = connect(sockClient, (SOCKADDR*)&addrServer, sizeof(SOCKADDR));
	if (err == SOCKET_ERROR) {
		printf("connect() called failed! The error code is %d\n", WSAGetLastError());

	}
	else {
		printf("connect() called successful\n");

	}

	//char buf[] = "This is a client side!\n";
	//string str;
	//std::getline(cin, str);
	//send(sockClient, str.c_str(), strlen(str.c_str()) + 1, 0);

		//接收数据  
	while (1) {
	char recvBuf[BUFF_SIZE] = { 0 };
	recv(sockClient, recvBuf, 100, 0);
	printf("receive data from server side is: %s\n", recvBuf);
	//char buf[] = "This is a client side!\n";
	string str="pppppppppppppppppppppp";
	//::getline(cin, str);
	send(sockClient, str.c_str(), strlen(str.c_str()) + 1, 0);
}
	    //发送数据  
	   
	   //关闭套接字  
		   closesocket(sockClient);
	    //终止套接字库的使用  
		   WSACleanup();

	    return 0;
	}
