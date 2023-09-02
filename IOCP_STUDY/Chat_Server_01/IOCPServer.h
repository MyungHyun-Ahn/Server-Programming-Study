#pragma once
#include "pch.h"

class IOCPServer
{
public:
	IOCPServer() { }
	virtual ~IOCPServer() {
		WSACleanup();
	}

	// ���� ����
	bool Init(const UINT32 maxIOWorkerThreadCount);
	bool BindandListen(int bindPort);
	bool ServerStart(const UINT32 maxClientCount);

	// ������ ����
	void DestroyThread();

	// ����
	bool SendMsg(const UINT32 clientIndex, const UINT32 dataSize, char* pData);

	// ���ø����̼ǿ��� ���
	virtual void OnConnect(const UINT32 clientIndex);
	virtual void OnClose(const UINT32 clientIndex);
	virtual void OnReceive(const UINT32 clientIndex, const UINT32 dataSize, char* pData);

private:
	// Ŭ���̾�Ʈ ����
	void CreateClient(const UINT32 maxClientCount);
	ClientInfo* GetEmptyClientInfo();
	ClientInfo* GetClientInfo(const UINT32 clientIndex);

	// ������ ����
	bool CreateWorkerThread();
	bool CreateAccepterThread();

	void WorkerThread();
	void AccepterThread();


	void CloseSocket();

private:
	// Ŭ���̾�Ʈ ����
	vector<ClientInfo*> _clientInfos;
	INT32 _clientCount;

	// ���� ����
	SOCKET _listenSocket = INVALID_SOCKET;

	// IO ������ ����
	UINT32 _maxIOWorkerThreadCount = 0;
	bool _isWorkerRun = true;
	thread _IOWorkerThread;

	// Accepter ������ ����
	bool _isAccepterRun = true;
	thread _accepterThread;

	// IOCP CP ������Ʈ
	HANDLE _IOCPHandle = INVALID_HANDLE_VALUE;
};