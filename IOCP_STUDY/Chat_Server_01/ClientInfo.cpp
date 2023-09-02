#include "pch.h"
#include "ClientInfo.h"


ClientInfo::ClientInfo()
{
	ZeroMemory(&_recvOverlappedEx, sizeof(OverlappedEx));
	_socket = INVALID_SOCKET;
}

void ClientInfo::Init(const UINT32 index, HANDLE iocpHandle)
{
	_index = index;
	_iocpHandle = iocpHandle;
}

bool ClientInfo::OnConnect(HANDLE iocpHandle, SOCKET socket)
{
	_socket = socket;
	_isConnect = 1;

	Clear();

	//I/O Completion Port��ü�� ������ �����Ų��.
	if (BindIOCP(iocpHandle) == false)
	{
		return false;
	}

	return BindRecv();
}

void ClientInfo::Close(bool isForce)
{
	struct linger stLinger = { 0, 0 };	// SO_DONTLINGER�� ����

	// bIsForce�� true�̸� SO_LINGER, timeout = 0���� �����Ͽ� ���� ���� ��Ų��. ���� : ������ �ս��� ������ ���� 
	if (true == isForce)
	{
		stLinger.l_onoff = 1;
	}

	//socketClose������ ������ �ۼ����� ��� �ߴ� ��Ų��.
	shutdown(_socket, SD_BOTH);

	//���� �ɼ��� �����Ѵ�.
	setsockopt(_socket, SOL_SOCKET, SO_LINGER, (char*)&stLinger, sizeof(stLinger));

	_isConnect = 0;
	_lastestClosedTimeSec = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
	//���� ������ ���� ��Ų��.
	closesocket(_socket);
	_socket = INVALID_SOCKET;
}

void ClientInfo::Clear()
{
}

bool ClientInfo::PostAccept(SOCKET listenSock, const UINT64 curTimeSec)
{
	printf_s("PostAccept. client Index: %d\n", GetIndex());

	_lastestClosedTimeSec = UINT32_MAX;

	_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET == _socket)
	{
		printf_s("client Socket WSASocket Error : %d\n", GetLastError());
		return false;
	}

	ZeroMemory(&_acceptOverlappedEx, sizeof(OverlappedEx));

	DWORD bytes = 0;
	DWORD flags = 0;
	_acceptOverlappedEx._wsaBuf.len = 0;
	_acceptOverlappedEx._wsaBuf.buf = nullptr;
	_acceptOverlappedEx._operation = IOOperation::ACCEPT;
	_acceptOverlappedEx._sessionIndex = _index;

	if (FALSE == AcceptEx(listenSock, _socket, _acceptBuf, 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &bytes, (LPWSAOVERLAPPED) & (_acceptOverlappedEx)))
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			printf_s("AcceptEx Error : %d\n", GetLastError());
			return false;
		}
	}

	return true;
}

bool ClientInfo::AcceptCompletion()
{
	printf_s("AcceptCompletion : SessionIndex(%d)\n", _index);

	if (OnConnect(_iocpHandle, _socket) == false)
	{
		return false;
	}

	SOCKADDR_IN		stClientAddr;
	int nAddrLen = sizeof(SOCKADDR_IN);
	char clientIP[32] = { 0, };
	inet_ntop(AF_INET, &(stClientAddr.sin_addr), clientIP, 32 - 1);
	printf("Ŭ���̾�Ʈ ���� : IP(%s) SOCKET(%d)\n", clientIP, (int)_socket);

	return true;
}

bool ClientInfo::BindIOCP(HANDLE iocpHandle)
{
	auto hIOCP = CreateIoCompletionPort((HANDLE)GetSock()
		, iocpHandle
		, (ULONG_PTR)(this), 0);

	if (hIOCP == INVALID_HANDLE_VALUE)
	{
		printf("[����] CreateIoCompletionPort()�Լ� ����: %d\n", GetLastError());
		return false;
	}

	return true;
}

bool ClientInfo::BindRecv()
{
	DWORD dwFlag = 0;
	DWORD dwRecvNumBytes = 0;

	//Overlapped I/O�� ���� �� ������ ������ �ش�.
	_recvOverlappedEx._wsaBuf.len = MAX_SOCK_RECVBUF;
	_recvOverlappedEx._wsaBuf.buf = _recvBuf;
	_recvOverlappedEx._operation = IOOperation::RECV;

	int nRet = WSARecv(_socket,
		&(_recvOverlappedEx._wsaBuf),
		1,
		&dwRecvNumBytes,
		&dwFlag,
		(LPWSAOVERLAPPED) & (_recvOverlappedEx),
		NULL);

	//socket_error�̸� client socket�� �������ɷ� ó���Ѵ�.
	if (nRet == SOCKET_ERROR && (WSAGetLastError() != ERROR_IO_PENDING))
	{
		printf("[����] WSARecv()�Լ� ���� : %d\n", WSAGetLastError());
		return false;
	}

	return true;
}

bool ClientInfo::SendMsg(const UINT32 dataSize, char* pMsg)
{
	auto sendOverlappedEx = new OverlappedEx;
	ZeroMemory(sendOverlappedEx, sizeof(OverlappedEx));
	sendOverlappedEx->_wsaBuf.len = dataSize;
	sendOverlappedEx->_wsaBuf.buf = new char[dataSize];
	CopyMemory(sendOverlappedEx->_wsaBuf.buf, pMsg, dataSize);
	sendOverlappedEx->_operation = IOOperation::SEND;

	std::lock_guard<std::mutex> guard(_sendLock);

	_sendDataqueue.push(sendOverlappedEx);

	if (_sendDataqueue.size() == 1)
	{
		SendIO();
	}

	return true;
}

void ClientInfo::SendCompleted(const UINT32 dataSize)
{
	printf("[�۽� �Ϸ�] bytes : %d\n", dataSize);

	std::lock_guard<std::mutex> guard(_sendLock);

	delete[] _sendDataqueue.front()->_wsaBuf.buf;
	delete _sendDataqueue.front();

	_sendDataqueue.pop();

	if (_sendDataqueue.empty() == false)
	{
		SendIO();
	}
}

bool ClientInfo::SendIO()
{
	auto sendOverlappedEx = _sendDataqueue.front();

	DWORD dwRecvNumBytes = 0;
	int nRet = WSASend(_socket,
		&(sendOverlappedEx->_wsaBuf),
		1,
		&dwRecvNumBytes,
		0,
		(LPWSAOVERLAPPED)sendOverlappedEx,
		NULL);

	//socket_error�̸� client socket�� �������ɷ� ó���Ѵ�.
	if (nRet == SOCKET_ERROR && (WSAGetLastError() != ERROR_IO_PENDING))
	{
		printf("[����] WSASend()�Լ� ���� : %d\n", WSAGetLastError());
		return false;
	}

	return true;
}

bool ClientInfo::SetSocketOption()
{
	int opt = 1;
	if (SOCKET_ERROR == setsockopt(_socket, IPPROTO_TCP, TCP_NODELAY, (const char*)&opt, sizeof(int)))
	{
		printf_s("[DEBUG] TCP_NODELAY error: %d\n", GetLastError());
		return false;
	}

	opt = 0;
	if (SOCKET_ERROR == setsockopt(_socket, SOL_SOCKET, SO_RCVBUF, (const char*)&opt, sizeof(int)))
	{
		printf_s("[DEBUG] SO_RCVBUF change error: %d\n", GetLastError());
		return false;
	}

	return true;
}
