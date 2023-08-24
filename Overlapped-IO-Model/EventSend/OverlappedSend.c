/*
Overlapped IO�� ���� ����
	SOCKET WSASocket(int af, int type, int protocol, LPWSAPROTOCOL_INFO lpProtocolInfo, GROUP g, DWORD dwFlags);
		- lpProtocolInfo	: �����Ǵ� ������ Ư�� ������ ��� ����ü, �ʿ� ���� ��� NULL ����
		- g					: ����Ǿ� �ִ� �Ű�����, 0 ����
		- ** dwFlags		: WSA_FLAG_OVERLAPPED�� �����Ͽ� Overlapped IO�� ������ ������ ����

Event ��� - manual-reset
Overlapped IO�� ���۹��
	int WSASend(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent, DWORD dwFlags,
					LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoution);
		- WSA�� ���� �Լ��� ���� �ִ� ���� : ����� �ϷḦ Ȯ���ϴ� ����� �Լ����� ��������

		- dwFlags : �Լ��� ������ ����Ư���� �����ϴ� ��� ���
		- lpOverlapped : Event ������Ʈ�� ����ϴ� ��� ���
		- lpCompletionRoutine : �������� ���� �ϷḦ Ȯ���� Completion Routine �̶�� �Լ��� �ּ� �� ����

		- WSAEVENT �ڵ� ���� -> WSAOVERLAPPED ����ü�� hEvent�� �̺�Ʈ �ڵ� ��� -> WSASend �Լ��� ����ü ���� ����
				 -> �������� ���� �Ϸᰡ �Ǹ� event�� signaled ���·� ��ȯ

������ �ۼ��� ����� Ȯ�ι��
	BOOL WSAGetOverlappedResult(SOCKET s, LPWSAOVERLAPPED lpOverlapped, LPDWORD lpcbTransfer, BOOL fWait, LPDWORD lpdwFlags);
		- lpOverlapped : WSAOVERLAPPED ����ü�� �ּ� �� ����
		- lpcbTransfer : ���� �ۼ��ŵ� ����Ʈ ũ�⸦ ������ ������ �ּ� �� ����
		- fWait : IO�� �������� ���, TRUE ���� �� �Ϸ���� ���, FALSE ���޽� FALSE ��ȯ�ϸ� ��������

		- WSASend �Լ��� ȣ���ϰ� �ٷ� WRITE �� ��Ȳ���� ȣ���� �ʿ� ����
		- SOCKET_ERROR�� ��ȯ���� �� -> �Ϸᰡ ���� ���� ���� ��ȯ�� ���� ����
		- WSAGetLastError �Լ�ȣ���� ���Ͽ� WSA_IO_PENDING�� ��ϵ� ��� IO�� ���� ���� ���̶�� �ǹ�
		- ��, ������� �Ϸ���� �ʾ��� -> �̶� WSAGetOverlappedResult �Լ��� ȣ���Ͽ� ���� ���۵� �������� ũ�⸦ Ȯ��

Overlapped IO�� ���Ź��
	int WSARecv(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesRecvd, LPWORD lpFlags,
					LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoution);
		- WSASend �Լ��� ������ �ſ� ����
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

	// event object �����ϰ� WSAOVERLAPPED ����ü�� �̺�Ʈ �ڵ� ���
	evObj = WSACreateEvent();
	memset(&overlapped, 0, sizeof(overlapped));
	overlapped.hEvent = evObj;

	// WSABUF�� ���� ������ ���� ��Ϥ�
	dataBuf.len = strlen(msg) + 1;
	dataBuf.buf = msg;

	// OVERLAPPED ����ü�� ����Ͽ� ����
	if (WSASend(hSocket, &dataBuf, 1, &sendBytes, 0, &overlapped, NULL) == SOCKET_ERROR)
	{
		// PENDING ���¸� ���� ���̶�� �ǹ� - �ƴ� �� WSASend ����
		if (WSAGetLastError() == WSA_IO_PENDING)
		{
			puts("Background data send");
			// ������ ���� ������ ���
			WSAWaitForMultipleEvents(1, &evObj, TRUE, WSA_INFINITE, FALSE);
			// ���� ����� Ȯ��
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