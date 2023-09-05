#pragma once

class IOCPServer
{
public:
	IOCPServer() { }
	virtual ~IOCPServer() {
		WSACleanup();
	}

	// 서버 관련
	bool			Init(const UINT32 maxIOWorkerThreadCount);
	bool			BindandListen(int bindPort);
	bool			ServerStart(const UINT32 maxClientCount);

	// 쓰레드 관련
	void			DestroyThread();

	// 전송
	bool			SendMsg(const UINT32 clientIndex, const UINT32 dataSize, char* pData);

	// 애플리케이션에서 사용
	virtual void	OnConnect(const UINT32 clientIndex) { }
	virtual void	OnClose(const UINT32 clientIndex) { }
	virtual void	OnReceive(const UINT32 clientIndex, const UINT32 dataSize, char* pData) { }

private:
	// 클라이언트 관련
	void			CreateClient(const UINT32 maxClientCount);
	ClientInfo*		GetEmptyClientInfo();
	ClientInfo*		GetClientInfo(const UINT32 clientIndex);

	// 쓰레드 관련
	bool			CreateWorkerThread();
	bool			CreateAccepterThread();

	void			WorkerThread();
	void			AccepterThread();


	void			CloseSocket(ClientInfo* clientInfo, bool isForce = false);

private:
	// 클라이언트 관련
	INT32						_clientCount = 0;
	std::vector<ClientInfo*>	_clientInfos;

	// 리슨 소켓
	SOCKET						_listenSocket = INVALID_SOCKET;

	// IO 쓰레드 관련
	UINT32						_maxIOWorkerThreadCount = 0;
	bool						_isWorkerRun = true;
	std::thread					_IOWorkerThread;
	std::vector<std::thread>	_IOWorkerThreads;

	// Accepter 쓰레드 관련
	bool						_isAccepterRun = true;
	std::thread					_accepterThread;

	// IOCP CP 오브젝트
	HANDLE						_IOCPHandle = INVALID_HANDLE_VALUE;
};