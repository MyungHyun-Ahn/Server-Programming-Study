#pragma once
#include "pch.h"

class IOCPServer
{
public:
	IOCPServer() {}
	virtual ~IOCPServer() { WSACleanup(); }

	bool InitSocket()
	{
		WSADATA wsaData;

		int nRet = WSAStartup(MAKEWORD(2, 2), &wsaData);

		if (0 != nRet)
		{
			printf("[에러] WSAStartup()함수 실패 : %d\n", WSAGetLastError());
			return false;
		}

		// TCP, Overlapped IO 소켓 생성
		mListenSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED);

		if (INVALID_SOCKET == mListenSocket)
		{
			printf("[에러] socket()함수 실패 : %d\n", WSAGetLastError());
			return false;
		}

		printf("소켓 초기화 성공\n");
		return true;
	}

	// 서버용 함수
	// 서버의 주소정보를 소켓과 연결시키고 접속 요청을 받기 위해 소켓을 등록하는 함수
	bool BindandListen(int nBindPort)
	{
		SOCKADDR_IN stServerAddr;
		stServerAddr.sin_family = AF_INET;
		stServerAddr.sin_port = htons(nBindPort);
		stServerAddr.sin_addr.s_addr = htonl(INADDR_ANY); // 어떤 주소에서 들어오는 접속이라도 받아들이겠다


		int nRet = bind(mListenSocket, (SOCKADDR*)&stServerAddr, sizeof(SOCKADDR_IN));

		if (0 != nRet)
		{
			printf("[에러] bind()함수 실패 : %d\n", WSAGetLastError());
			return false;
		}

		//접속 요청을 받아들이기 위해 cIOCompletionPort소켓을 등록하고 
		//접속대기큐를 5개로 설정 한다.
		nRet = listen(mListenSocket, 5);
		if (0 != nRet)
		{
			printf("[에러] listen()함수 실패 : %d\n", WSAGetLastError());
			return false;
		}

		printf("서버 등록 성공..\n");
		return true;
	}

	//접속 요청을 수락하고 메세지를 받아서 처리하는 함수
	bool StartServer(const UINT32 maxClientCount)
	{
		CreateClient(maxClientCount);


		mIOCPHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, MAX_WORKERTHREAD);
		if (NULL == mIOCPHandle)
		{
			printf("[에러] CreateIoCompletionPort()함수 실패: %d\n", GetLastError());
			return false;
		}

		auto hIOCPHandle = CreateIoCompletionPort((HANDLE)mListenSocket, mIOCPHandle, (UINT32)0, 0);
		if (nullptr == hIOCPHandle)
		{
			printf("[에러] listen socket IOCP bind 실패 : %d\n", WSAGetLastError());
			return false;
		}

		bool bRet = CreateWokerThread();
		if (false == bRet) {
			return false;
		}

		bRet = CreateAccepterThread();
		if (false == bRet) {
			return false;
		}

		bRet = CreateSenderThread();
		if (false == bRet) {
			return false;
		}

		printf("서버 시작\n");
		return true;
	}

	// 생성되어있는 쓰레드를 파괴
	void DestroyThread()
	{
		mIsWorkerRun = false;
		CloseHandle(mIOCPHandle);

		for (auto& th : mIOWorkerThreads)
		{
			if (th.joinable())
				th.join();
		}

		mIsAccepterRun = false;
		CloseHandle(reinterpret_cast<HANDLE>(mListenSocket));

		if (mAccepterThread.joinable())
			mAccepterThread.join();
	}

	void SendMsg(UINT32 clientIndex, UINT32 dataSize, char* msgData)
	{
		auto client = mClientInfos[clientIndex];
		client->PushSendQueue(dataSize, msgData);
	}

	// 가상함수
	virtual void OnConnect(const UINT32 clientIdx) {}
	virtual void OnClose(const UINT32 clientIdx) {}
	virtual void OnReceive(const UINT32 clientIdx, const UINT32 dataSize, char* data) {}

private:
	void CreateClient(const UINT32 maxClientCount)
	{
		for (UINT32 i = 0; i < maxClientCount; ++i)
		{
			stClientInfo* client = new stClientInfo();
			client->Init(i);

			mClientInfos.push_back(client);
		}
	}

	stClientInfo* GetClientInfo(const UINT32 clientIndex)
	{
		return mClientInfos[clientIndex];
	}

	// WaitingThread Queue에서 대기할 쓰레드들을 생성
	bool CreateWokerThread()
	{
		unsigned int uiThreadId = 0;
		//WaingThread Queue에 대기 상태로 넣을 쓰레드들 생성 권장되는 개수 : (cpu개수 * 2) + 1 
		for (int i = 0; i < MAX_WORKERTHREAD; i++)
			mIOWorkerThreads.emplace_back([this]() { WokerThread(); });

		printf("WokerThread 시작..\n");
		return true;
	}

	//accept요청을 처리하는 쓰레드 생성
	bool CreateAccepterThread()
	{
		mAccepterThread = std::thread([this]() { AccepterThread(); });

		printf("AccepterThread 시작..\n");
		return true;
	}

	bool CreateSenderThread()
	{
		mSenderThread = thread([this]() { SendThread(); });
		printf("SenderThread 시작..\n");
		return true;
	}

	//사용하지 않는 클라이언트 정보 구조체를 반환한다.
	stClientInfo* GetEmptyClientInfo()
	{
		for (auto& client : mClientInfos)
		{
			if (INVALID_SOCKET == client->getClientSocket())
			{
				return client;
			}
		}

		return nullptr;
	}

	// Overlapped I/O 작업에 대한 완료 통보를 받아 
	// 그에 해당하는 처리를 하는 함수
	void WokerThread()
	{
		// CompletionKey를 받을 포인터 변수
		stClientInfo* pClientInfo = NULL;
		// 함수 호출 성공 여부
		BOOL bSuccess = TRUE;
		// Overlapped I/O 작업에서 전송된 데이터 크기
		DWORD dwIoSize = 0;
		// I/O 작업을 위해 요청한 Overlapped 구조체를 받을 포인터
		LPOVERLAPPED lpOverlapped = NULL;

		while (mIsWorkerRun)
		{
			//////////////////////////////////////////////////////
			// 이 함수로 인해 쓰레드들은 WaitingThread Queue에
			// 대기 상태로 들어가게 된다.
			// 완료된 Overlapped I/O 작업이 발생하면 IOCP Queue에서
			// 완료된 작업을 가져와 뒤 처리를 한다.
			// 그리고 PostQueuedCompletionStatus()함수에의해 사용자
			// 메세지가 도착되면 쓰레드를 종료한다.
			//////////////////////////////////////////////////////
			bSuccess = GetQueuedCompletionStatus(mIOCPHandle,
				&dwIoSize,						// 실제로 전송된 바이트
				(PULONG_PTR)&pClientInfo,		// CompletionKey
				&lpOverlapped,					// Overlapped IO 객체
				INFINITE);						// 대기할 시간

			// 사용자 쓰레드 종료 메세지 처리..
			if (TRUE == bSuccess && 0 == dwIoSize && NULL == lpOverlapped)
			{
				mIsWorkerRun = false;
				continue;
			}

			if (NULL == lpOverlapped)
			{
				continue;
			}

			stOverlappedEx* pOverlappedEx = (stOverlappedEx*)lpOverlapped;

			// client가 접속을 끊었을때..			
			if (FALSE == bSuccess || (0 == dwIoSize && IOOperation::ACCEPT != pOverlappedEx->m_eOperation))
			{
				// printf("socket(%d) 접속 끊김\n", (int)pClientInfo->m_socketClient);

				// 접속 종료 정보 애플리케이션에 전달
				OnClose(static_cast<int>(pClientInfo->getClientSocket()));

				pClientInfo->CloseSocket();
				continue;
			}

			// Overlapped I/O Recv 작업 결과 뒤 처리
			if (IOOperation::RECV == pOverlappedEx->m_eOperation)
			{
				// 데이터 수신 정보 애플리케이션에 전달
				OnReceive(pClientInfo->getIndex(), dwIoSize, (pClientInfo->getRecvBuffer()));
				pClientInfo->BindRecv();
			}
			//Overlapped I/O Send 작업 결과 뒤 처리
			else if (IOOperation::SEND == pOverlappedEx->m_eOperation)
			{
				pClientInfo->SendCompleted(dwIoSize);
			}
			else if (IOOperation::ACCEPT == pOverlappedEx->m_eOperation)
			{
				pClientInfo = GetClientInfo(pOverlappedEx->m_clientIndex);
				if (pClientInfo->AcceptCompletion(mIOCPHandle))
				{
					++mClientCnt;

					OnConnect(pClientInfo->getIndex());
				}
				else
				{
					pClientInfo->CloseSocket();
				}
			}
			//예외 상황
			else
			{
				printf("socket(%d)에서 예외상황\n", (int)pClientInfo->getIndex());
			}
		}
	}

	// 사용자의 접속을 받는 쓰레드
	void AccepterThread()
	{
		while (mIsAccepterRun)
		{
			auto nowTimeSec = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
			for (auto client : mClientInfos)
			{
				if (client->IsConnected())
					continue;

				if (nowTimeSec < client->getLastClosedTime())
					continue;

				auto diff = nowTimeSec - client->getLastClosedTime();
				if (diff <= RE_USE_SESSION_WAIT_TIMESEC)
					continue;

				client->PostAccept(mListenSocket);
			}
		}
	}

	void SendThread()
	{
		while (mIsSenderRun)
		{
			for (auto client : mClientInfos)
			{
				if (client->IsSend())
					continue;

				client->SendIO();
			}
		}
	}

private:
	vector<stClientInfo*>	mClientInfos;						// 클라이언트 정보 저장
	SOCKET					mListenSocket = INVALID_SOCKET;		// 리슨 소켓
	int						mClientCnt = 0;						// 접속 클라이언트 수
	vector<thread>			mIOWorkerThreads;					// IO Worker 쓰레드
	thread					mAccepterThread;					// Accept 쓰레드
	thread					mSenderThread;						// Sender 쓰레드
	HANDLE					mIOCPHandle = INVALID_HANDLE_VALUE; // Completion Port 오브젝트
	bool					mIsWorkerRun = true;				// 작업 쓰레드 동작 플래그
	bool					mIsAccepterRun = true;				// 접속 쓰레드 동작 플래그
	bool					mIsSenderRun = true;				// 전송 쓰레드 동작 플래그
};