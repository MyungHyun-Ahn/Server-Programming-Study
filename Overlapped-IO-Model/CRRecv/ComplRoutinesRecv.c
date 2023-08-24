/*
Completion Routine 사용하기
	- IO가 완료되었을때 호출되는 함수를 가리켜 Completion Routine이라 함
	- IO가 완료되면, 등록된 함수가 운영체제에서 자동으로 호출됨
	- Completion Routine이 호출되려면 해당 쓰레드가 alertable wait 상태여야 함
	- Alertable wait은 운영체제가 전달하는 메시지의 수신이 가능한 상태, 특별한 일을 진행하지 않는 상태

	- 등록을 해서 호출이 되는 것은 맞지만 호출의 타이밍은 직접 알려주어야 함

Alertable wait 상태 진입에 사용되는 함수들
	WaitForSingleObjectEx
	WaitForMultipleObjectEx
	WSAWaitForMultipleEvents
	SleepEx

	- Ex가 붙은 함수들은 alertable wait 상태에 진입시키는 기능을 포함한 것
*/

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define BUF_SIZE 1024

// Completion Routine 함수
void CALLBACK CompRoutine(DWORD, DWORD, LPWSAOVERLAPPED, DWORD);
void ErrorHandling(char* message);

WSABUF dataBuf;
char buf[BUF_SIZE];
int recvBytes = 0;

int main()
{
	WSADATA wsaData;
	SOCKET hLisnSock, hRecvSock;
	SOCKADDR_IN lisnAdr, recvAdr;

	WSAOVERLAPPED overlapped;
	WSAEVENT evObj;

	int idx, recvAdrSz, flags = 0;

	/*
	if (argc != 2)
	{
		printf("Usage : %s <port> \n", argv[0]);
		exit(1);
	}
	*/

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	hLisnSock = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	memset(&lisnAdr, 0, sizeof(lisnAdr));
	lisnAdr.sin_family = AF_INET;
	lisnAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	lisnAdr.sin_port = htons(atoi("8080"));

	if (bind(hLisnSock, (SOCKADDR*)&lisnAdr, sizeof(lisnAdr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");

	if (listen(hLisnSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error");

	recvAdrSz = sizeof(recvAdr);
	hRecvSock = accept(hLisnSock, (SOCKADDR*)&recvAdr, &recvAdrSz);
	if (hRecvSock == INVALID_SOCKET)
		ErrorHandling("accept() error");

	memset(&overlapped, 0, sizeof(overlapped));
	dataBuf.len = BUF_SIZE;
	dataBuf.buf = buf;

	evObj = WSACreateEvent(); // Dummy : 함수 호출을 위해 그냥 넣어줌

	// CompRoutine 등록
	if (WSARecv(hRecvSock, &dataBuf, 1, &recvBytes, &flags, &overlapped, CompRoutine) == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSA_IO_PENDING)
			puts("Background data receive");
	}

	// Alertable wait 상태로 진입 : 마지막 인자를 TRUE
	// WSAWaitForMultipleEvents가 반환되면 Recv가 완료된 것
	// WAIT_IO_COMPLETION가 반환되면 Recv가 성공한 것
	idx = WSAWaitForMultipleEvents(1, &evObj, FALSE, WSA_INFINITE, TRUE);
	if (idx == WAIT_IO_COMPLETION)
		puts("Overlapped I/O Completed");
	else
		ErrorHandling("WSARecv() error!");

	WSACloseEvent(evObj);
	closesocket(hRecvSock);
	closesocket(hLisnSock);
	WSACleanup();
	return 0;
}

void CALLBACK CompRoutine(DWORD dwError, DWORD szRecvBytes, LPWSAOVERLAPPED lpOverlapped, DWORD flags)
{
	if (dwError != 0)
	{
		ErrorHandling("CompRoutine error!");
	}
	else
	{
		recvBytes = szRecvBytes;
		printf("Received message : %s \n", buf);
	}
}

void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
