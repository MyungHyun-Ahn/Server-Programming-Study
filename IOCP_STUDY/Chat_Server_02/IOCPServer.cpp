#include "pch.h"
#include "IOCPServer.h"

bool IOCPServer::Init(const UINT32 maxIOWorkerThreadCount)
{
	WSAData wsaData;

	int nRet = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (0 != nRet)
	{
		printf("[에러] WSAStartup()함수 실패 : %d\n", WSAGetLastError());
		return false;
	}

	//연결지향형 TCP , Overlapped I/O 소켓을 생성
	_listenSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED);

	if (INVALID_SOCKET == _listenSocket)
	{
		printf("[에러] socket()함수 실패 : %d\n", WSAGetLastError());
		return false;
	}

	_maxIOWorkerThreadCount = maxIOWorkerThreadCount;

	printf("소켓 초기화 성공\n");
	return true;
}

bool IOCPServer::BindandListen(int bindPort)
{
	// 서버 주소 설정
	SOCKADDR_IN serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(bindPort);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	// bind
	int nRet = bind(_listenSocket, (SOCKADDR*)&serverAddr, sizeof(SOCKADDR_IN));
	if (0 != nRet)
	{
		printf("[에러] bind()함수 실패 : %d\n", WSAGetLastError());
		return false;
	}

	//접속 요청을 받아들이기
	nRet = listen(_listenSocket, 5);
	if (0 != nRet)
	{
		printf("[에러] listen()함수 실패 : %d\n", WSAGetLastError());
		return false;
	}

	//CompletionPort 객체 생성
	_IOCPHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, _maxIOWorkerThreadCount);
	if (NULL == _IOCPHandle)
	{
		printf("[에러] CreateIoCompletionPort()함수 실패: %d\n", GetLastError());
		return false;
	}

	// listen 소켓을 IOCP 핸들에 등록
	auto hIOCPHandle = CreateIoCompletionPort((HANDLE)_listenSocket, _IOCPHandle, (UINT32)0, 0);
	if (nullptr == hIOCPHandle)
	{
		printf("[에러] listen socket IOCP bind 실패 : %d\n", WSAGetLastError());
		return false;
	}

	printf("서버 등록 성공..\n");
	return true;
}

bool IOCPServer::ServerStart(const UINT32 maxClientCount)
{
	CreateClient(maxClientCount);

	//접속된 클라이언트 주소 정보를 저장할 구조체
	bool bRet = CreateWorkerThread();
	if (false == bRet) {
		return false;
	}

	bRet = CreateAccepterThread();
	if (false == bRet) {
		return false;
	}

	printf("서버 시작\n");
	return true;
}

void IOCPServer::DestroyThread()
{
	_isWorkerRun = false;
	CloseHandle(_IOCPHandle);

	for (auto& th : _IOWorkerThreads)
	{
		if (th.joinable())
		{
			th.join();
		}
	}

	_isAccepterRun = false;
	closesocket(_listenSocket);

	if (_accepterThread.joinable())
	{
		_accepterThread.join();
	}
}

bool IOCPServer::SendMsg(const UINT32 clientIndex, const UINT32 dataSize, char* pData)
{
	auto pClient = GetClientInfo(clientIndex);
	return pClient->SendMsg(dataSize, pData);
}

void IOCPServer::CreateClient(const UINT32 maxClientCount)
{
	for (UINT32 i = 0; i < maxClientCount; ++i)
	{
		auto client = new ClientInfo;
		client->Init(i, _IOCPHandle);

		_clientInfos.push_back(client);
	}
}

ClientInfo* IOCPServer::GetEmptyClientInfo()
{
	for (auto& client : _clientInfos)
	{
		if (client->IsConnected() == false)
		{
			return client;
		}
	}

	return nullptr;
}

ClientInfo* IOCPServer::GetClientInfo(const UINT32 clientIndex)
{
	return _clientInfos[clientIndex];
}

bool IOCPServer::CreateWorkerThread()
{
	for (UINT32 i = 0; i < _maxIOWorkerThreadCount; i++)
	{
		_IOWorkerThreads.emplace_back([this]() { WorkerThread(); });
	}

	printf("WokerThread 시작..\n");
	return true;
}

bool IOCPServer::CreateAccepterThread()
{
	_accepterThread = std::thread([this]() { AccepterThread(); });

	printf("AccepterThread 시작..\n");
	return true;
}

void IOCPServer::WorkerThread()
{
	//CompletionKey를 받을 포인터 변수
	ClientInfo* pClientInfo = nullptr;
	//함수 호출 성공 여부
	BOOL bSuccess = TRUE;
	//Overlapped I/O작업에서 전송된 데이터 크기
	DWORD dwIoSize = 0;
	//I/O 작업을 위해 요청한 Overlapped 구조체를 받을 포인터
	LPOVERLAPPED lpOverlapped = NULL;

	while (_isWorkerRun)
	{
		bSuccess = GetQueuedCompletionStatus(_IOCPHandle,
			&dwIoSize,					// 실제로 전송된 바이트
			(PULONG_PTR)&pClientInfo,		// CompletionKey
			&lpOverlapped,				// Overlapped IO 객체
			INFINITE);					// 대기할 시간

		//사용자 쓰레드 종료 메세지 처리..
		if (TRUE == bSuccess && 0 == dwIoSize && NULL == lpOverlapped)
		{
			_isWorkerRun = false;
			continue;
		}

		if (NULL == lpOverlapped)
		{
			continue;
		}

		auto pOverlappedEx = (OverlappedEx*)lpOverlapped;

		//client가 접속을 끊었을때..			
		if (FALSE == bSuccess || (0 == dwIoSize && IOOperation::ACCEPT != pOverlappedEx->_operation))
		{
			//printf("socket(%d) 접속 끊김\n", (int)pClientInfo->m_socketClient);
			CloseSocket(pClientInfo); //Caller WokerThread()
			continue;
		}


		if (IOOperation::ACCEPT == pOverlappedEx->_operation)
		{
			pClientInfo = GetClientInfo(pOverlappedEx->_sessionIndex);
			if (pClientInfo->AcceptCompletion())
			{
				//클라이언트 갯수 증가
				++_clientCount;

				OnConnect(pClientInfo->GetIndex());
			}
			else
			{
				CloseSocket(pClientInfo, true);  //Caller WokerThread()
			}
		}
		//Overlapped I/O Recv작업 결과 뒤 처리
		else if (IOOperation::RECV == pOverlappedEx->_operation)
		{
			OnReceive(pClientInfo->GetIndex(), dwIoSize, pClientInfo->GetRecvBuffer());
			pClientInfo->BindRecv();
		}
		//Overlapped I/O Send작업 결과 뒤 처리
		else if (IOOperation::SEND == pOverlappedEx->_operation)
		{
			pClientInfo->SendCompleted(dwIoSize);
		}
		//예외 상황
		else
		{
			printf("Client Index(%d)에서 예외상황\n", pClientInfo->GetIndex());
		}
	}
}

void IOCPServer::AccepterThread()
{
	while (_isAccepterRun)
	{
		auto curTimeSec = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now().time_since_epoch()).count();

		for (auto client : _clientInfos)
		{
			if (client->IsConnected())
			{
				continue;
			}

			if ((UINT64)curTimeSec < client->GetLastestClosedTimeSec())
			{
				continue;
			}

			auto diff = curTimeSec - client->GetLastestClosedTimeSec();
			if (diff <= RE_USE_SESSION_WAIT_TIMESEC)
			{
				continue;
			}

			client->PostAccept(_listenSocket, curTimeSec);
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(32));
	}
}

void IOCPServer::CloseSocket(ClientInfo* clientInfo, bool isForce)
{
	if (clientInfo->IsConnected() == false)
	{
		return;
	}

	auto clientIndex = clientInfo->GetIndex();

	clientInfo->Close(isForce);

	OnClose(clientIndex);
}
