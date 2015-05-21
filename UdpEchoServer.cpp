// UdpEchoServer.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#include <winsock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>
#include <conio.h>

// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

#include <iostream>
#include <string.h>

using namespace std;


void printhex(char* buf, int bufLen)
{
	for (int i = 0; i < bufLen; i++) {
		printf("%02X ", (unsigned char)buf[i]);
	}
}

int startServer()
{
	// Get datagram socket to send message on
	SOCKET sd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sd < 0) {
		cerr << "socket() failed: " << WSAGetLastError() << endl;
		return 1;
	}

	// Enable broadcasts on the socket
	int bAllow = 1;
	if (setsockopt(sd, SOL_SOCKET, SO_BROADCAST, (char*)&bAllow,
		sizeof(bAllow)) < 0) {
		cerr << "setsockopt() failed: " << WSAGetLastError() << endl;
		closesocket(sd);
		return 1;
	}

	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	int kPort = 20000;
	
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = htonl(ADDR_ANY);
	sin.sin_port = htons(kPort);

	int bind_result = bind(sd, (sockaddr*)&sin, sizeof(sin));

	int bufLen = 1024 * 100;
	char* buf = (char*)malloc(bufLen);
	while (true) {


		sockaddr_in from;
		int fromLen = sizeof(from);

		int received = recvfrom(sd, buf, bufLen, 0, (sockaddr*)&from, &fromLen);
		printf("received:%d\n", received);
		printhex(buf, received);
		printf("\n");
		if (received > 0) {
			Sleep(30);
			int sent = sendto(sd, buf, received, 0, (sockaddr*)&from, fromLen);
			printf("sent:%d\n", sent);
		}
	}

	closesocket(sd);

	return 0;
}

int main(int argc, char* argv[])
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa)) {
		cerr << "Failed to init Winsock!" << endl;
		return 1;
	}

	startServer();

	WSACleanup();

	getch();
	return 0;
}