/*
select() : 관찰할 정보를 배열에 저장해서 건내줌

소켓에 이벤트 등록
	int WSAEventSelect(Socket s, WSAEVENT hEventObject, long lNetworkEvents);
		- s					: 관리할 대상 소켓의 핸들을 전달
		- hEventObject		: 해당 소켓의 변화가 감지되었을 때 확인하기 위한 커널오브젝트의 핸들 전달
		- lNetworkEvents	: 감시할 이벤트의 유형 전달
	
		- Socket과 hEventObject를 연결한다고 이해
		- Socket에서 lNetworkEvents에 해당하는 이벤트가 감지되면 hEventObject에 해당하는 커널오브젝트를 signaled 상태로 바꿈
	
		- INetworkEvents에 전달할 수 있는 정보
			* FD_READ		: 수신할 정보가 존재하는가?
			* FD_WRITE		: 블로킹 없이 데이터 전송이 가능한가?
			* FD_OOB		: Out of band 데이터가 수신되었는가?
			* FD_ACCEPT		: 연결요청이 있었는가?
			* FD_CLOSE		: 연결의 종료가 요청되었는가?
	
이벤트의 생성과 종료
	WSAEVENT WSACreateEvent(void);
		- Event 오브젝트 핸들(hEventObject) 오브젝트를 생성 (manual-reset mode)
		- non-signaled 상태

	BOOL WSACloseEvent(WSAEVENT hEvent);
		- Event 오브젝트의 종료를 위한 함수

이벤트 발생유무의 확인
	DWORD WSAWaitForMultipleEvents(DWORD cEvents, const WSAEVENT* lphEvents, BOOL fWaitAll, DWORD dwTimeout, BOOL fAlertable);
		- 커널 오브젝트를 확인 - signaled 상태로 바뀌었는지
		
		- cEvents		: signaled 상태로 바뀌었는지 체크할 Event 오브젝트의 개수 정보 전달
		- lphEvents		: Event 오브젝트의 핸들을 저장하고 있는 배열의 주소 값 전달
		- fWaitAll		: TRUE 전달 - 모든 Event 오브젝트가 signaled 상태일 때 반환
						  False 전달 - 하나만 signaled 상태여도 반환
		- dwTimeout		: Timeout 지정, WSA_INFINTE 전달 - signaled 상태가 될 때까지 반환하지 않음
		- fAlertable	: TRUE 전달 - alertable wait 상태로의 진입
			* WaitForMultipleObjects 함수와의 유일한 차이점

		- 변화가 발생한 소켓이 있는지 원하는 타이밍에 확인할 수 있음
		- 반환값에 WSA_WAIT_EVENT_0를 빼준 값은 이벤트 핸들의 배열에서 변화가 발생한 핸들의 시작 인덱스 값
		- 시작 위치에서 반복문을 통하여 뒤의 값까지 하나 씩 검사

		- manual-reset 모드를 사용하는 이유
		- auto-reset 모드에서는 WSAWaitForMultipleEvents 함수를 호출하면 모두 non-signaled 상태로 전환되므로 뒷순서의 변화는 감지할 수 없게 됨

	- 소켓 생성 -> 이벤트 오브젝트 생성 -> 소켓과 이벤트 오브젝트를 연결
	- 소켓 1개 당 이벤트 오브젝트 1개
	- 원하는 타이밍에 WSAWaitForMultipleEvents 함수를 호출하여 관찰

이벤트 종류의 구분
	int WSAEnumNetworkEvents(SOCKET s, WSAEVENT hEventObject, LPWSANETWORKEVENTS lpNetworkEvents);
		- lpNetworkEvents : 발생한 이벤트의 유형정보와 오류정보로 채워질 WSANETWORKEVENTS 구조체 변수의 주소 값 전달

		WSANETWORKEVENTS netEvents;
		WSAEnumNetworkEvents(hSock, hEvent, &netEvents);
		if (netEvents.lNetworkEvents & FD_ACCEPT) // FD_ACCEPT 이벤트에 대한 처리


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

	// 소켓 핸들과 이벤트 오브젝트는 1:1 대응
	// 소켓의 핸들 정보를 저장하는 배열
	SOCKET hSockArr[WSA_MAXIMUM_WAIT_EVENTS];
	// 이벤트 오브젝트 핸들을 저장하는 배열
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

	// listen 단계에서 이벤트 핸들 생성
	newEvent = WSACreateEvent();
	// FD_ACCEPT 이벤트가 발생하면 signaled 상태로 바꿈
	if (WSAEventSelect(hServSock, newEvent, FD_ACCEPT) == SOCKET_ERROR)
		ErrorHandling("WSAEventSelect() error");

	hSockArr[numOfClntSock] = hServSock;
	hEventArr[numOfClntSock] = newEvent;
	numOfClntSock++;

	while (1)
	{
		posInfo = WSAWaitForMultipleEvents(numOfClntSock, hEventArr, FALSE, WSA_INFINITE, FALSE);

		// 이벤트가 발생한 첫번째 인덱스를 반환
		startIdx = posInfo - WSA_WAIT_EVENT_0;

		for (i = startIdx; i < numOfClntSock; i++)
		{
			int sigEventIdx =
				WSAWaitForMultipleEvents(1, &hEventArr[i], TRUE, 0, FALSE);
			// 반환 결과가 오류나 타임아웃이면 continue
			if ((sigEventIdx == WSA_WAIT_FAILED || sigEventIdx == WSA_WAIT_TIMEOUT))
			{
				continue;
			}
			else
			{
				sigEventIdx = i;
				WSAEnumNetworkEvents(hSockArr[sigEventIdx], hEventArr[sigEventIdx], &netEvents);

				// 연결 요청이면
				if (netEvents.lNetworkEvents & FD_ACCEPT)
				{
					if (netEvents.iErrorCode[FD_ACCEPT_BIT] != 0)
					{
						puts("Accept Error");
						break;
					}
					clntAdrLen = sizeof(clntAdr);

					// 연결을 수락
					hClntSock = accept(hSockArr[sigEventIdx], (SOCKADDR*)&clntAdr, &clntAdrLen);
					// 클라이언트 소켓 생성, READ 이벤트와 CLOSE 이벤트 감지 등록
					newEvent = WSACreateEvent();
					WSAEventSelect(hClntSock, newEvent, FD_READ | FD_CLOSE);

					hEventArr[numOfClntSock] = newEvent;
					hSockArr[numOfClntSock] = hClntSock;
					numOfClntSock++;
					puts("connected new client...");
				}

				// 이벤트 유형이 READ라면
				if (netEvents.lNetworkEvents & FD_READ)
				{
					if (netEvents.iErrorCode[FD_READ_BIT] != 0)
					{
						puts("Read Error");
						break;
					}
					// recv를 통하여 데이터를 받고
					strLen = recv(hSockArr[sigEventIdx], msg, sizeof(msg), 0);
					// echo server - 받은 데이터를 그대로 다시 전송
					send(hSockArr[sigEventIdx], msg, strLen, 0);
				}

				// CLOSE 이벤트가 발생
				if (netEvents.lNetworkEvents & FD_CLOSE)
				{
					if (netEvents.iErrorCode[FD_CLOSE_BIT] != 0)
					{
						puts("Close Error");
						break;
					}
					// 클라이언트 소켓을 닫아줌
					WSACloseEvent(hEventArr[sigEventIdx]);
					closesocket(hSockArr[sigEventIdx]);

					numOfClntSock--;

					// 삭제된 인덱스의 뒷부분을 땡겨줌
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