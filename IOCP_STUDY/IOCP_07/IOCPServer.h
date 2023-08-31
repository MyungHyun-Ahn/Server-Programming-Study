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
			printf("[����] WSAStartup()�Լ� ���� : %d\n", WSAGetLastError());
			return false;
		}

		// TCP, Overlapped IO ���� ����
		mListenSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED);

		if (INVALID_SOCKET == mListenSocket)
		{
			printf("[����] socket()�Լ� ���� : %d\n", WSAGetLastError());
			return false;
		}

		printf("���� �ʱ�ȭ ����\n");
		return true;
	}

	// ������ �Լ�
	// ������ �ּ������� ���ϰ� �����Ű�� ���� ��û�� �ޱ� ���� ������ ����ϴ� �Լ�
	bool BindandListen(int nBindPort)
	{
		SOCKADDR_IN stServerAddr;
		stServerAddr.sin_family = AF_INET;
		stServerAddr.sin_port = htons(nBindPort);
		stServerAddr.sin_addr.s_addr = htonl(INADDR_ANY); // � �ּҿ��� ������ �����̶� �޾Ƶ��̰ڴ�


		int nRet = bind(mListenSocket, (SOCKADDR*)&stServerAddr, sizeof(SOCKADDR_IN));

		if (0 != nRet)
		{
			printf("[����] bind()�Լ� ���� : %d\n", WSAGetLastError());
			return false;
		}

		//���� ��û�� �޾Ƶ��̱� ���� cIOCompletionPort������ ����ϰ� 
		//���Ӵ��ť�� 5���� ���� �Ѵ�.
		nRet = listen(mListenSocket, 5);
		if (0 != nRet)
		{
			printf("[����] listen()�Լ� ���� : %d\n", WSAGetLastError());
			return false;
		}

		printf("���� ��� ����..\n");
		return true;
	}

	//���� ��û�� �����ϰ� �޼����� �޾Ƽ� ó���ϴ� �Լ�
	bool StartServer(const UINT32 maxClientCount)
	{
		CreateClient(maxClientCount);


		mIOCPHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, MAX_WORKERTHREAD);
		if (NULL == mIOCPHandle)
		{
			printf("[����] CreateIoCompletionPort()�Լ� ����: %d\n", GetLastError());
			return false;
		}

		auto hIOCPHandle = CreateIoCompletionPort((HANDLE)mListenSocket, mIOCPHandle, (UINT32)0, 0);
		if (nullptr == hIOCPHandle)
		{
			printf("[����] listen socket IOCP bind ���� : %d\n", WSAGetLastError());
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

		printf("���� ����\n");
		return true;
	}

	// �����Ǿ��ִ� �����带 �ı�
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

	// �����Լ�
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

	// WaitingThread Queue���� ����� ��������� ����
	bool CreateWokerThread()
	{
		unsigned int uiThreadId = 0;
		//WaingThread Queue�� ��� ���·� ���� ������� ���� ����Ǵ� ���� : (cpu���� * 2) + 1 
		for (int i = 0; i < MAX_WORKERTHREAD; i++)
			mIOWorkerThreads.emplace_back([this]() { WokerThread(); });

		printf("WokerThread ����..\n");
		return true;
	}

	//accept��û�� ó���ϴ� ������ ����
	bool CreateAccepterThread()
	{
		mAccepterThread = std::thread([this]() { AccepterThread(); });

		printf("AccepterThread ����..\n");
		return true;
	}

	bool CreateSenderThread()
	{
		mSenderThread = thread([this]() { SendThread(); });
		printf("SenderThread ����..\n");
		return true;
	}

	//������� �ʴ� Ŭ���̾�Ʈ ���� ����ü�� ��ȯ�Ѵ�.
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

	// Overlapped I/O �۾��� ���� �Ϸ� �뺸�� �޾� 
	// �׿� �ش��ϴ� ó���� �ϴ� �Լ�
	void WokerThread()
	{
		// CompletionKey�� ���� ������ ����
		stClientInfo* pClientInfo = NULL;
		// �Լ� ȣ�� ���� ����
		BOOL bSuccess = TRUE;
		// Overlapped I/O �۾����� ���۵� ������ ũ��
		DWORD dwIoSize = 0;
		// I/O �۾��� ���� ��û�� Overlapped ����ü�� ���� ������
		LPOVERLAPPED lpOverlapped = NULL;

		while (mIsWorkerRun)
		{
			//////////////////////////////////////////////////////
			// �� �Լ��� ���� ��������� WaitingThread Queue��
			// ��� ���·� ���� �ȴ�.
			// �Ϸ�� Overlapped I/O �۾��� �߻��ϸ� IOCP Queue����
			// �Ϸ�� �۾��� ������ �� ó���� �Ѵ�.
			// �׸��� PostQueuedCompletionStatus()�Լ������� �����
			// �޼����� �����Ǹ� �����带 �����Ѵ�.
			//////////////////////////////////////////////////////
			bSuccess = GetQueuedCompletionStatus(mIOCPHandle,
				&dwIoSize,						// ������ ���۵� ����Ʈ
				(PULONG_PTR)&pClientInfo,		// CompletionKey
				&lpOverlapped,					// Overlapped IO ��ü
				INFINITE);						// ����� �ð�

			// ����� ������ ���� �޼��� ó��..
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

			// client�� ������ ��������..			
			if (FALSE == bSuccess || (0 == dwIoSize && IOOperation::ACCEPT != pOverlappedEx->m_eOperation))
			{
				// printf("socket(%d) ���� ����\n", (int)pClientInfo->m_socketClient);

				// ���� ���� ���� ���ø����̼ǿ� ����
				OnClose(static_cast<int>(pClientInfo->getClientSocket()));

				pClientInfo->CloseSocket();
				continue;
			}

			// Overlapped I/O Recv �۾� ��� �� ó��
			if (IOOperation::RECV == pOverlappedEx->m_eOperation)
			{
				// ������ ���� ���� ���ø����̼ǿ� ����
				OnReceive(pClientInfo->getIndex(), dwIoSize, (pClientInfo->getRecvBuffer()));
				pClientInfo->BindRecv();
			}
			//Overlapped I/O Send �۾� ��� �� ó��
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
			//���� ��Ȳ
			else
			{
				printf("socket(%d)���� ���ܻ�Ȳ\n", (int)pClientInfo->getIndex());
			}
		}
	}

	// ������� ������ �޴� ������
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
	vector<stClientInfo*>	mClientInfos;						// Ŭ���̾�Ʈ ���� ����
	SOCKET					mListenSocket = INVALID_SOCKET;		// ���� ����
	int						mClientCnt = 0;						// ���� Ŭ���̾�Ʈ ��
	vector<thread>			mIOWorkerThreads;					// IO Worker ������
	thread					mAccepterThread;					// Accept ������
	thread					mSenderThread;						// Sender ������
	HANDLE					mIOCPHandle = INVALID_HANDLE_VALUE; // Completion Port ������Ʈ
	bool					mIsWorkerRun = true;				// �۾� ������ ���� �÷���
	bool					mIsAccepterRun = true;				// ���� ������ ���� �÷���
	bool					mIsSenderRun = true;				// ���� ������ ���� �÷���
};