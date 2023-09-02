#pragma once
#include "pch.h"

class ClientInfo
{
	ClientInfo();

	void Init(const UINT32 index, HANDLE iocpHandle);

	UINT32 GetIndex() { return _index; }
	bool IsConnected() { return _isConnect; }
	SOCKET GetSock() { return _socket; }
	UINT64 GetLastestClosedTimeSec() { return _lastestClosedTimeSec; }
	char* GetRecvBuffer() { return _recvBuf; }

	bool OnConnect(HANDLE iocpHandle, SOCKET sock);
	void Close(bool isForce = false);
	void Clear();
	
	bool PostAccept(SOCKET listenSock, const UINT64 curTimeSec);
	bool AcceptCompletion();
	bool BindIOCP(HANDLE iocpHandle);

	bool BindRecv();

	bool SendMsg();
	void SendCompleted(const UINT32 dataSize);

private:
	bool SendIO();
	bool SetSocketOption();


private:
	INT32 _index = 0;
	HANDLE _iocpHande = INVALID_HANDLE_VALUE;

	INT64 _isConnect = 0;
	UINT64 _lastestClosedTimeSec = 0;

	SOCKET _socket;
	OverlappedEx _acceptOverlappedEx;
	char _acceptBuf[64];

	OverlappedEx _recvOverlappedEx;
	char _recvBuf[MAX_SOCK_RECVBUF];

	mutex _sendLock;
	queue<OverlappedEx*> _sendDataqueue;
};