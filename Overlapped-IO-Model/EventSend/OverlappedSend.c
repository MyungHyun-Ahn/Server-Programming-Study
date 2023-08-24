/*
Overlapped IO의 소켓 생성
	SOCKET WSASocket(int af, int type, int protocol, LPWSAPROTOCOL_INFO lpProtocolInfo, GROUP g, DWORD dwFlags);
		- lpProtocolInfo	: 생성되는 소켓의 특성 정보를 담는 구조체, 필요 없는 경우 NULL 전달
		- g					: 예약되어 있는 매개변수, 0 전달
		- ** dwFlags		: WSA_FLAG_OVERLAPPED를 전달하여 Overlapped IO가 가능한 소켓의 생성

Event 방식 - manual-reset
Overlapped IO의 전송방법
	int WSASend(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent, DWORD dwFlags,
					LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoution);
		- WSA가 붙은 함수가 따로 있는 이유 : 입출력 완료를 확인하는 방법을 함수에서 지정가능

		- dwFlags : 함수의 데이터 전송특성을 변경하는 경우 사용
		- lpOverlapped : Event 오브젝트를 사용하는 경우 사용
		- lpCompletionRoutine : 데이터의 전송 완료를 확인할 Completion Routine 이라는 함수의 주소 값 전달

		- WSAEVENT 핸들 생성 -> WSAOVERLAPPED 구조체의 hEvent에 이벤트 핸들 등록 -> WSASend 함수에 구조체 정보 전달
				 -> 데이터의 전송 완료가 되면 event가 signaled 상태로 전환

데이터 송수신 결과의 확인방법
	BOOL WSAGetOverlappedResult(SOCKET s, LPWSAOVERLAPPED lpOverlapped, LPDWORD lpcbTransfer, BOOL fWait, LPDWORD lpdwFlags);
		- lpOverlapped : WSAOVERLAPPED 구조체의 주소 값 전달
		- lpcbTransfer : 실제 송수신된 바이트 크기를 저장할 변수의 주소 값 전달
		- fWait : IO가 진행중인 경우, TRUE 전달 시 완료까지 대기, FALSE 전달시 FALSE 반환하며 빠져나옴

		- WSASend 함수를 호출하고 바로 WRITE 된 상황에는 호출할 필요 없음
		- SOCKET_ERROR를 반환했을 때 -> 완료가 진행 중일 때도 반환할 수도 있음
		- WSAGetLastError 함수호출을 통하여 WSA_IO_PENDING이 등록된 경우 IO가 현재 진행 중이라는 의미
		- 즉, 입출력이 완료되지 않았음 -> 이때 WSAGetOverlappedResult 함수를 호출하여 실제 전송된 데이터의 크기를 확인

Overlapped IO의 수신방법
	int WSARecv(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesRecvd, LPWORD lpFlags,
					LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoution);
		- WSASend 함수와 사용법과 매우 유사
*/

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

void ErrorHandling(char* msg);

int main()
{
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN sendAdr;

	WSABUF dataBuf;
	char msg[] = "Network is Computer!";
	int sendBytes = 0;

	WSAEVENT evObj;
	WSAOVERLAPPED overlapped;

	/*
	if (argc != 3)
	{
		printf("Usage: %s <IP> <port>\n", argc[0]);
		exit(1);
	}
	*/

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	hSocket = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	memset(&sendAdr, 0, sizeof(sendAdr));
	sendAdr.sin_family = AF_INET;
	sendAdr.sin_addr.s_addr = inet_addr("127.0.0.1");
	sendAdr.sin_port = htons(atoi("8080"));

	if (connect(hSocket, (SOCKADDR*)&sendAdr, sizeof(sendAdr)) == SOCKET_ERROR)
		ErrorHandling("connect() error!");

	// event object 생성하고 WSAOVERLAPPED 구조체에 이벤트 핸들 등록
	evObj = WSACreateEvent();
	memset(&overlapped, 0, sizeof(overlapped));
	overlapped.hEvent = evObj;

	// WSABUF에 전송 데이터 정보 등록ㄴ
	dataBuf.len = strlen(msg) + 1;
	dataBuf.buf = msg;

	// OVERLAPPED 구조체를 등록하여 전송
	if (WSASend(hSocket, &dataBuf, 1, &sendBytes, 0, &overlapped, NULL) == SOCKET_ERROR)
	{
		// PENDING 상태면 전송 중이라는 의미 - 아닐 시 WSASend 에러
		if (WSAGetLastError() == WSA_IO_PENDING)
		{
			puts("Background data send");
			// 전송이 끝날 때까지 대기
			WSAWaitForMultipleEvents(1, &evObj, TRUE, WSA_INFINITE, FALSE);
			// 전송 결과를 확인
			WSAGetOverlappedResult(hSocket, &overlapped, &sendBytes, FALSE, NULL);
		}
		else
		{
			ErrorHandling("WSASend() error!");
		}
	}

	printf("Send data size : %d \n", sendBytes);
	WSACloseEvent(evObj);
	closesocket(hSocket);
	WSACleanup();
	return 0;
}

void ErrorHandling(char* msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}