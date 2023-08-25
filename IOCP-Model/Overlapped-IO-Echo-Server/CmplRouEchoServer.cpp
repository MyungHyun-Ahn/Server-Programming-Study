/*
Completion Routine Echo Server

Non-Blocking mode�� ���� ����
	SOCKET hLisnSock;
	int mode = 1; // Non-Blocking ������ �ɼ������� ���

	hLisnSock = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	ioctlsocket(hLisnSock, FIONBIO, &mode);
		- ����� ���(FIONBIO)�� ���� mode�� ����� ������ �ٲ�
		- Ŭ���̾�Ʈ �����û�� ���� �� accpet ȣ�� -> INVALID_SOCKET ��ȯ
		- WSAGetLastError ȣ�� -> WSAEWOULDBLOCK ��ȯ
		- accept �Լ� ȣ��� ���� �����Ǵ� ���� ���� Non-Blocking �Ӽ��� ����

TCP�� ��谡 ����
	- TCP�� ��谡 �����Ƿ� �����Ͱ� �Ϻθ� ���� ���¿��� C-R�� ����� Ȯ���� �ſ� ����
	- Ŭ���̾�Ʈ�� �Ϻθ� �����͸� �а� �� - ���� �����Ͱ� �� �����ߴٰ� ������ �ϰ� ���α׷����� �ϸ� �ȵ�
	- Ŭ���̾�Ʈ�� �����͸� ī��Ʈ �ؾ���

Overlapped IO ���� ������
	- Non-Blocking ����� accept �Լ��� alertable wait ���·��� ������ ���� SleepEx �Լ��� ������ ���� �ݺ� ȣ�� �ǹǷ� ���ɿ� ����
�ذ�å
	- accept �����带 �ϳ��ΰ�
	- ������ �����忡�� Ŭ���̾�Ʈ�� ������� ����ϰ� ��
*/

#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

#define BUF_SIZE 1024

void CALLBACK ReadCompRoutine(DWORD, DWORD, LPWSAOVERLAPPED, DWORD);
void CALLBACK WriteCompRoutine(DWORD, DWORD, LPWSAOVERLAPPED, DWORD);
void ErrorHandling(const char* message);

// Ŭ���̾�Ʈ�� ������ ������� ����ü
typedef struct
{
	SOCKET hClntSock; // �����û�� ������ ������ �ڵ�
	char buf[BUF_SIZE]; // �����͸� �ְ���� �� ����� ����
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

		// hRecvSock ���� ���� ����
		// overlapped ������ ��� �����Ǹ鼭 �ְ� �ް� �ϱ� ������ �����Ҵ��� ���Ͽ� ����ü ���� ����
		lpOvLp = (LPWSAOVERLAPPED)malloc(sizeof(WSAOVERLAPPED));
		memset(lpOvLp, 0, sizeof(WSAOVERLAPPED));
		hbInfo = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
		hbInfo->hClntSock = (DWORD)hRecvSock; // ���� ����
		(hbInfo->wsaBuf).buf = hbInfo->buf; // ���� ����
		(hbInfo->wsaBuf).len = BUF_SIZE;

		// ReadCompRoutine �Լ� ȣ�� -> ����ü�� ����(lpOvLp)�� ����
		// lpOvLp�� ������ ���� ����
		lpOvLp->hEvent = (HANDLE)hbInfo; // �ش� ���� ���� ����
		// ReadCompRoutine �Լ����� ������ ���� ������ ������� Write�� ���� ����
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
	// Info ���� ����
	LPPER_IO_DATA hbInfo = (LPPER_IO_DATA)(lpOverlapped->hEvent);
	// ���� ���� ���� -> �ش� ������ �̿��Ͽ� ReadCompRoutine ����
	SOCKET hSock = hbInfo->hClntSock;
	LPWSABUF bufInfo = &(hbInfo->wsaBuf);
	DWORD sentBytes;

	// ���� �����Ͱ� 0�̸� ���� ���� ��û
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