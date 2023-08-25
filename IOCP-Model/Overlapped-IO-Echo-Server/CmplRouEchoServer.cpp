/*
Completion Routine Echo Server

Non-Blocking mode의 소켓 구성
	SOCKET hLisnSock;
	int mode = 1; // Non-Blocking 소켓의 옵션지정에 사용

	hLisnSock = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	ioctlsocket(hLisnSock, FIONBIO, &mode);
		- 입출력 모드(FIONBIO)를 변수 mode에 저장된 값으로 바꿈
		- 클라이언트 연결요청이 없을 때 accpet 호출 -> INVALID_SOCKET 반환
		- WSAGetLastError 호출 -> WSAEWOULDBLOCK 반환
		- accept 함수 호출로 새로 생성되는 소켓 또한 Non-Blocking 속성을 지님

TCP는 경계가 있음
	- TCP는 경계가 있으므로 데이터가 일부만 들어온 상태에서 C-R이 수행될 확률이 매우 높음
	- 클라이언트는 일부만 데이터를 읽게 됨 - 절대 데이터가 다 도착했다고 가정을 하고 프로그래밍을 하면 안됨
	- 클라이언트는 데이터를 카운트 해야함

Overlapped IO 모델의 문제점
	- Non-Blocking 모드의 accept 함수와 alertable wait 상태로의 진입을 위한 SleepEx 함수를 번갈아 가며 반복 호출 되므로 성능에 영향
해결책
	- accept 쓰레드를 하나두고
	- 별도의 쓰레드에서 클라이언트의 입출력을 담당하게 함
*/

#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

#define BUF_SIZE 1024

void CALLBACK ReadCompRoutine(DWORD, DWORD, LPWSAOVERLAPPED, DWORD);
void CALLBACK WriteCompRoutine(DWORD, DWORD, LPWSAOVERLAPPED, DWORD);
void ErrorHandling(const char* message);

// 클라이언트의 정보를 담기위한 구조체
typedef struct
{
	SOCKET hClntSock; // 연결요청이 생성된 소켓의 핸들
	char buf[BUF_SIZE]; // 데이터를 주고받을 때 사용할 버퍼
	WSABUF wsaBuf;
} PER_IO_DATA, *LPPER_IO_DATA;

int main()
{
	WSADATA wsaData;
	SOCKET hLisnSock, hRecvSock;
	SOCKADDR_IN lisnAdr, recvAdr;
	LPWSAOVERLAPPED lpOvLp;
	DWORD recvBytes;
	LPPER_IO_DATA hbInfo;
	int mode = 1, recvAdrSz, flagInfo = 0;

	/*if (argc != 2)
	{
		printf("Usage : %s <port> \n");
		exit(1);
	}*/

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	hLisnSock = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	ioctlsocket(hLisnSock, FIONBIO, (u_long*)&mode);

	memset(&lisnAdr, 0, sizeof(lisnAdr));
	lisnAdr.sin_family = AF_INET;
	lisnAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	lisnAdr.sin_port = htons(atoi("8080"));

	if (bind(hLisnSock, (SOCKADDR*)&lisnAdr, sizeof(lisnAdr)) == SOCKET_ERROR)
		ErrorHandling("bind() error!");

	if (listen(hLisnSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error!");

	recvAdrSz = sizeof(recvAdr);

	while (1)
	{
		SleepEx(100, TRUE);    // for alertable wait state
		hRecvSock = accept(hLisnSock, (SOCKADDR*)&recvAdr, &recvAdrSz);
		if (hRecvSock == INVALID_SOCKET)
		{
			if (WSAGetLastError() == WSAEWOULDBLOCK)
				continue;
			else
				ErrorHandling("accept() error");
		}
		puts("Client connected.....");

		// hRecvSock 소켓 생성 이후
		// overlapped 정보가 계속 유지되면서 주고 받고 하기 때문에 동적할당을 통하여 구조체 변수 생성
		lpOvLp = (LPWSAOVERLAPPED)malloc(sizeof(WSAOVERLAPPED));
		memset(lpOvLp, 0, sizeof(WSAOVERLAPPED));
		hbInfo = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
		hbInfo->hClntSock = (DWORD)hRecvSock; // 소켓 정보
		(hbInfo->wsaBuf).buf = hbInfo->buf; // 버퍼 정보
		(hbInfo->wsaBuf).len = BUF_SIZE;

		// ReadCompRoutine 함수 호출 -> 구조체를 인자(lpOvLp)로 전달
		// lpOvLp의 정보를 참조 가능
		lpOvLp->hEvent = (HANDLE)hbInfo; // 해당 정보 추출 가능
		// ReadCompRoutine 함수에서 추출한 소켓 정보를 대상으로 Write를 진행 가능
		WSARecv(hRecvSock, &(hbInfo->wsaBuf), 1, &recvBytes, (LPDWORD)&flagInfo, lpOvLp, ReadCompRoutine);
	}
	closesocket(hRecvSock);
	closesocket(hLisnSock);
	WSACleanup();
	return 0;

}

void CALLBACK ReadCompRoutine(
	DWORD dwError, DWORD szRecvBytes, LPWSAOVERLAPPED lpOverlapped, DWORD flags)
{
	// Info 정보 추출
	LPPER_IO_DATA hbInfo = (LPPER_IO_DATA)(lpOverlapped->hEvent);
	// 소켓 정보 추출 -> 해당 정보를 이용하여 ReadCompRoutine 진행
	SOCKET hSock = hbInfo->hClntSock;
	LPWSABUF bufInfo = &(hbInfo->wsaBuf);
	DWORD sentBytes;

	// 수신 데이터가 0이면 연결 종료 요청
	if (szRecvBytes == 0)
	{
		closesocket(hSock);
		free(lpOverlapped->hEvent); free(lpOverlapped);
		puts("Client disconnected.....");
	}
	else    // echo!
	{
		bufInfo->len = szRecvBytes;
		WSASend(hSock, bufInfo, 1, &sentBytes, 0, lpOverlapped, WriteCompRoutine);
	}
}

void CALLBACK WriteCompRoutine(
	DWORD dwError, DWORD szSendBytes, LPWSAOVERLAPPED lpOverlapped, DWORD flags)
{
	LPPER_IO_DATA hbInfo = (LPPER_IO_DATA)(lpOverlapped->hEvent);
	SOCKET hSock = hbInfo->hClntSock;
	LPWSABUF bufInfo = &(hbInfo->wsaBuf);
	DWORD recvBytes;
	int flagInfo = 0;
	WSARecv(hSock, bufInfo, 1, &recvBytes, (LPDWORD)&flagInfo, lpOverlapped, ReadCompRoutine);
}

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}