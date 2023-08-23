/*
select() : ������ ������ �迭�� �����ؼ� �ǳ���

���Ͽ� �̺�Ʈ ���
	int WSAEventSelect(Socket s, WSAEVENT hEventObject, long lNetworkEvents);
		- s					: ������ ��� ������ �ڵ��� ����
		- hEventObject		: �ش� ������ ��ȭ�� �����Ǿ��� �� Ȯ���ϱ� ���� Ŀ�ο�����Ʈ�� �ڵ� ����
		- lNetworkEvents	: ������ �̺�Ʈ�� ���� ����
	
		- Socket�� hEventObject�� �����Ѵٰ� ����
		- Socket���� lNetworkEvents�� �ش��ϴ� �̺�Ʈ�� �����Ǹ� hEventObject�� �ش��ϴ� Ŀ�ο�����Ʈ�� signaled ���·� �ٲ�
	
		- INetworkEvents�� ������ �� �ִ� ����
			* FD_READ		: ������ ������ �����ϴ°�?
			* FD_WRITE		: ���ŷ ���� ������ ������ �����Ѱ�?
			* FD_OOB		: Out of band �����Ͱ� ���ŵǾ��°�?
			* FD_ACCEPT		: �����û�� �־��°�?
			* FD_CLOSE		: ������ ���ᰡ ��û�Ǿ��°�?
	
�̺�Ʈ�� ������ ����
	WSAEVENT WSACreateEvent(void);
		- Event ������Ʈ �ڵ�(hEventObject) ������Ʈ�� ���� (manual-reset mode)
		- non-signaled ����

	BOOL WSACloseEvent(WSAEVENT hEvent);
		- Event ������Ʈ�� ���Ḧ ���� �Լ�

�̺�Ʈ �߻������� Ȯ��
	DWORD WSAWaitForMultipleEvents(DWORD cEvents, const WSAEVENT* lphEvents, BOOL fWaitAll, DWORD dwTimeout, BOOL fAlertable);
		- Ŀ�� ������Ʈ�� Ȯ�� - signaled ���·� �ٲ������
		
		- cEvents		: signaled ���·� �ٲ������ üũ�� Event ������Ʈ�� ���� ���� ����
		- lphEvents		: Event ������Ʈ�� �ڵ��� �����ϰ� �ִ� �迭�� �ּ� �� ����
		- fWaitAll		: TRUE ���� - ��� Event ������Ʈ�� signaled ������ �� ��ȯ
						  False ���� - �ϳ��� signaled ���¿��� ��ȯ
		- dwTimeout		: Timeout ����, WSA_INFINTE ���� - signaled ���°� �� ������ ��ȯ���� ����
		- fAlertable	: TRUE ���� - alertable wait ���·��� ����
			* WaitForMultipleObjects �Լ����� ������ ������

		- ��ȭ�� �߻��� ������ �ִ��� ���ϴ� Ÿ�ֿ̹� Ȯ���� �� ����
		- ��ȯ���� WSA_WAIT_EVENT_0�� ���� ���� �̺�Ʈ �ڵ��� �迭���� ��ȭ�� �߻��� �ڵ��� ���� �ε��� ��
		- ���� ��ġ���� �ݺ����� ���Ͽ� ���� ������ �ϳ� �� �˻�

		- manual-reset ��带 ����ϴ� ����
		- auto-reset ��忡���� WSAWaitForMultipleEvents �Լ��� ȣ���ϸ� ��� non-signaled ���·� ��ȯ�ǹǷ� �޼����� ��ȭ�� ������ �� ���� ��

	- ���� ���� -> �̺�Ʈ ������Ʈ ���� -> ���ϰ� �̺�Ʈ ������Ʈ�� ����
	- ���� 1�� �� �̺�Ʈ ������Ʈ 1��
	- ���ϴ� Ÿ�ֿ̹� WSAWaitForMultipleEvents �Լ��� ȣ���Ͽ� ����

�̺�Ʈ ������ ����
	int WSAEnumNetworkEvents(SOCKET s, WSAEVENT hEventObject, LPWSANETWORKEVENTS lpNetworkEvents);
		- lpNetworkEvents : �߻��� �̺�Ʈ�� ���������� ���������� ä���� WSANETWORKEVENTS ����ü ������ �ּ� �� ����

		WSANETWORKEVENTS netEvents;
		WSAEnumNetworkEvents(hSock, hEvent, &netEvents);
		if (netEvents.lNetworkEvents & FD_ACCEPT) // FD_ACCEPT �̺�Ʈ�� ���� ó��


*/


#include <stdio.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define BUF_SIZE 100

void CompressSockets(SOCKET hSockArr[], int idx, int total);
void CompressEvents(WSAEVENT hEventArr[], int idx, int total);
void ErrorHandling(char* msg);

int main(void)
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAdr, clntAdr;

	// ���� �ڵ�� �̺�Ʈ ������Ʈ�� 1:1 ����
	// ������ �ڵ� ������ �����ϴ� �迭
	SOCKET hSockArr[WSA_MAXIMUM_WAIT_EVENTS];
	// �̺�Ʈ ������Ʈ �ڵ��� �����ϴ� �迭
	WSAEVENT hEventArr[WSA_MAXIMUM_WAIT_EVENTS];

	WSAEVENT newEvent;
	WSANETWORKEVENTS netEvents;

	int numOfClntSock = 0;
	int strLen, i;
	int posInfo, startIdx;
	int clntAdrLen;
	char msg[BUF_SIZE];

	/*if (argc != 2) {
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}*/

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(atoi("8080"));

	if (bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");

	if (listen(hServSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error");

	// listen �ܰ迡�� �̺�Ʈ �ڵ� ����
	newEvent = WSACreateEvent();
	// FD_ACCEPT �̺�Ʈ�� �߻��ϸ� signaled ���·� �ٲ�
	if (WSAEventSelect(hServSock, newEvent, FD_ACCEPT) == SOCKET_ERROR)
		ErrorHandling("WSAEventSelect() error");

	hSockArr[numOfClntSock] = hServSock;
	hEventArr[numOfClntSock] = newEvent;
	numOfClntSock++;

	while (1)
	{
		posInfo = WSAWaitForMultipleEvents(numOfClntSock, hEventArr, FALSE, WSA_INFINITE, FALSE);

		// �̺�Ʈ�� �߻��� ù��° �ε����� ��ȯ
		startIdx = posInfo - WSA_WAIT_EVENT_0;

		for (i = startIdx; i < numOfClntSock; i++)
		{
			int sigEventIdx =
				WSAWaitForMultipleEvents(1, &hEventArr[i], TRUE, 0, FALSE);
			// ��ȯ ����� ������ Ÿ�Ӿƿ��̸� continue
			if ((sigEventIdx == WSA_WAIT_FAILED || sigEventIdx == WSA_WAIT_TIMEOUT))
			{
				continue;
			}
			else
			{
				sigEventIdx = i;
				WSAEnumNetworkEvents(hSockArr[sigEventIdx], hEventArr[sigEventIdx], &netEvents);

				// ���� ��û�̸�
				if (netEvents.lNetworkEvents & FD_ACCEPT)
				{
					if (netEvents.iErrorCode[FD_ACCEPT_BIT] != 0)
					{
						puts("Accept Error");
						break;
					}
					clntAdrLen = sizeof(clntAdr);

					// ������ ����
					hClntSock = accept(hSockArr[sigEventIdx], (SOCKADDR*)&clntAdr, &clntAdrLen);
					// Ŭ���̾�Ʈ ���� ����, READ �̺�Ʈ�� CLOSE �̺�Ʈ ���� ���
					newEvent = WSACreateEvent();
					WSAEventSelect(hClntSock, newEvent, FD_READ | FD_CLOSE);

					hEventArr[numOfClntSock] = newEvent;
					hSockArr[numOfClntSock] = hClntSock;
					numOfClntSock++;
					puts("connected new client...");
				}

				// �̺�Ʈ ������ READ���
				if (netEvents.lNetworkEvents & FD_READ)
				{
					if (netEvents.iErrorCode[FD_READ_BIT] != 0)
					{
						puts("Read Error");
						break;
					}
					// recv�� ���Ͽ� �����͸� �ް�
					strLen = recv(hSockArr[sigEventIdx], msg, sizeof(msg), 0);
					// echo server - ���� �����͸� �״�� �ٽ� ����
					send(hSockArr[sigEventIdx], msg, strLen, 0);
				}

				// CLOSE �̺�Ʈ�� �߻�
				if (netEvents.lNetworkEvents & FD_CLOSE)
				{
					if (netEvents.iErrorCode[FD_CLOSE_BIT] != 0)
					{
						puts("Close Error");
						break;
					}
					// Ŭ���̾�Ʈ ������ �ݾ���
					WSACloseEvent(hEventArr[sigEventIdx]);
					closesocket(hSockArr[sigEventIdx]);

					numOfClntSock--;

					// ������ �ε����� �޺κ��� ������
					CompressSockets(hSockArr, sigEventIdx, numOfClntSock);
					CompressEvents(hEventArr, sigEventIdx, numOfClntSock);
				}
			}
		}
	}
	WSACleanup();
	return 0;
}

void CompressSockets(SOCKET hSockArr[], int idx, int total)
{
	int i;
	for (i = idx; i < total; i++)
		hSockArr[i] = hSockArr[i + 1];
}
void CompressEvents(WSAEVENT hEventArr[], int idx, int total)
{
	int i;
	for (i = idx; i < total; i++)
		hEventArr[i] = hEventArr[i + 1];
}
void ErrorHandling(char* msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}