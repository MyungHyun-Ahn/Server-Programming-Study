#include "pch.h"
#include "IOCPServer.h"

bool IOCPServer::Init(const UINT32 maxIOWorkerThreadCount)
{
	WSAData wsaData;

	int nRet = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (0 != nRet)
	{
		printf("[����] WSAStartup()�Լ� ���� : %d\n", WSAGetLastError());
		return false;
	}

	//���������� TCP , Overlapped I/O ������ ����
	_listenSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED);

	if (INVALID_SOCKET == _listenSocket)
	{
		printf("[����] socket()�Լ� ���� : %d\n", WSAGetLastError());
		return false;
	}

	_maxIOWorkerThreadCount = maxIOWorkerThreadCount;

	printf("���� �ʱ�ȭ ����\n");
	return true;
}

bool IOCPServer::BindandListen(int bindPort)
{
	// ���� �ּ� ����
	SOCKADDR_IN serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(bindPort);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	// bind
	int nRet = bind(_listenSocket, (SOCKADDR*)&serverAddr, sizeof(SOCKADDR_IN));
	if (0 != nRet)
	{
		printf("[����] bind()�Լ� ���� : %d\n", WSAGetLastError());
		return false;
	}

	//���� ��û�� �޾Ƶ��̱�
	nRet = listen(_listenSocket, 5);
	if (0 != nRet)
	{
		printf("[����] listen()�Լ� ���� : %d\n", WSAGetLastError());
		return false;
	}

	//CompletionPort ��ü ����
	_IOCPHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, _maxIOWorkerThreadCount);
	if (NULL == _IOCPHandle)
	{
		printf("[����] CreateIoCompletionPort()�Լ� ����: %d\n", GetLastError());
		return false;
	}

	// listen ������ IOCP �ڵ鿡 ���
	auto hIOCPHandle = CreateIoCompletionPort((HANDLE)_listenSocket, _IOCPHandle, (UINT32)0, 0);
	if (nullptr == hIOCPHandle)
	{
		printf("[����] listen socket IOCP bind ���� : %d\n", WSAGetLastError());
		return false;
	}

	printf("���� ��� ����..\n");
	return true;
}

bool IOCPServer::ServerStart(const UINT32 maxClientCount)
{
	CreateClient(maxClientCount);

	//���ӵ� Ŭ���̾�Ʈ �ּ� ������ ������ ����ü
	bool bRet = CreateWorkerThread();
	if (false == bRet) {
		return false;
	}

	bRet = CreateAccepterThread();
	if (false == bRet) {
		return false;
	}

	printf("���� ����\n");
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

	printf("WokerThread ����..\n");
	return true;
}

bool IOCPServer::CreateAccepterThread()
{
	_accepterThread = std::thread([this]() { AccepterThread(); });

	printf("AccepterThread ����..\n");
	return true;
}

void IOCPServer::WorkerThread()
{
	//CompletionKey�� ���� ������ ����
	ClientInfo* pClientInfo = nullptr;
	//�Լ� ȣ�� ���� ����
	BOOL bSuccess = TRUE;
	//Overlapped I/O�۾����� ���۵� ������ ũ��
	DWORD dwIoSize = 0;
	//I/O �۾��� ���� ��û�� Overlapped ����ü�� ���� ������
	LPOVERLAPPED lpOverlapped = NULL;

	while (_isWorkerRun)
	{
		bSuccess = GetQueuedCompletionStatus(_IOCPHandle,
			&dwIoSize,					// ������ ���۵� ����Ʈ
			(PULONG_PTR)&pClientInfo,		// CompletionKey
			&lpOverlapped,				// Overlapped IO ��ü
			INFINITE);					// ����� �ð�

		//����� ������ ���� �޼��� ó��..
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

		//client�� ������ ��������..			
		if (FALSE == bSuccess || (0 == dwIoSize && IOOperation::ACCEPT != pOverlappedEx->_operation))
		{
			//printf("socket(%d) ���� ����\n", (int)pClientInfo->m_socketClient);
			CloseSocket(pClientInfo); //Caller WokerThread()
			continue;
		}


		if (IOOperation::ACCEPT == pOverlappedEx->_operation)
		{
			pClientInfo = GetClientInfo(pOverlappedEx->_sessionIndex);
			if (pClientInfo->AcceptCompletion())
			{
				//Ŭ���̾�Ʈ ���� ����
				++_clientCount;

				OnConnect(pClientInfo->GetIndex());
			}
			else
			{
				CloseSocket(pClientInfo, true);  //Caller WokerThread()
			}
		}
		//Overlapped I/O Recv�۾� ��� �� ó��
		else if (IOOperation::RECV == pOverlappedEx->_operation)
		{
			OnReceive(pClientInfo->GetIndex(), dwIoSize, pClientInfo->GetRecvBuffer());
			pClientInfo->BindRecv();
		}
		//Overlapped I/O Send�۾� ��� �� ó��
		else if (IOOperation::SEND == pOverlappedEx->_operation)
		{
			pClientInfo->SendCompleted(dwIoSize);
		}
		//���� ��Ȳ
		else
		{
			printf("Client Index(%d)���� ���ܻ�Ȳ\n", pClientInfo->GetIndex());
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
