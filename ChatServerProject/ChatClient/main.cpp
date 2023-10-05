#include "pch.h"
#include <iostream>
#include "SocketUtils.h"

int main()
{
	this_thread::sleep_for(1s);

	SocketUtils::Init();

	SOCKET clientSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET)
		return 0;

	u_long on = 1;
	if (::ioctlsocket(clientSocket, FIONBIO, &on) == INVALID_SOCKET)
		return 0;

	SOCKADDR_IN serverAddr;
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	::inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
	serverAddr.sin_port = ::htons(7777);

	// Connect
	while (true)
	{
		if (::connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
		{
			// 원래 블록했어야 했는데 ... 너가 논블로킹 하라며?
			if (::WSAGetLastError() == WSAEWOULDBLOCK)
				continue;

			// 이미 연결된 상태라면 break;
			if (::WSAGetLastError() == WSAEISCONN)
				break;

			// Error
			return 0;
		}
	}

	cout << "Connected To Server!" << endl;

	char sendBuffer[100] = "hello world";
	int32 sendLen = sizeof(sendBuffer);

	fd_set writes;

	// Send
	while (true)
	{
		// 소켓 셋 초기화
		FD_ZERO(&writes);

		// ListenSocket 등록
		FD_SET(clientSocket, &writes);

		if (FD_ISSET(clientSocket, &writes))
		{
			// 블로킹 모드 -> 모든 데이터 다 보냄
			// 논블로킹 모드 -> 일부만 보낼 수가 있음 (상대방 수신 버퍼 상황에 따라)
			int32 sentDataLen = ::send(clientSocket, sendBuffer, sendLen, 0);
			if (sentDataLen == SOCKET_ERROR)
			{
				// TODO
				continue;
			}

			cout << "Send Data ! Len = " << sentDataLen << endl;
			this_thread::sleep_for(1s);
		}
	}

	SocketUtils::Clear();
}