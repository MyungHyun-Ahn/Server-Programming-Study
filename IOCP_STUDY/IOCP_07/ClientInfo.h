#pragma once
#include "pch.h"

/*
�ǽ� 7�ܰ� �������� ��
AcceptEx �Լ��� �񵿱� accept ���� : PostAccept
�Ϸ�Ǹ� accept �Ϸ�ó�� : AcceptCompletion

AcceptEx() �Լ� ����
BOOL AcceptEx(
	SOCKET sListenSocket,
	SOCKET sAcceptSocket,
	PVOID lpOutPutBuffer,
	DWORD dwReceiveDataLength,
	DWORD dwRemoteAddressLength,
	LPWORD lpdwBytesReceived,
	LPOVERLAPPED lpOverlapped
);
*/

class stClientInfo
{
public:
	stClientInfo()
	{
		// �޸� ������ 0X00���� ä��� ��ũ�� : memset�� ����
		ZeroMemory(&m_stRecvOverlappedEx, sizeof(stOverlappedEx));
		m_socketClient = INVALID_SOCKET;
	}

	~stClientInfo() {}


	UINT32 getLastClosedTime() { return mLastClosedTimeSec; }
	bool IsConnected() { return mIsConnected == 1; }
	SOCKET getClientSocket() { return m_socketClient; }
	void setClientSocket(SOCKET clientSocket) { m_socketClient = clientSocket; }
	INT32 getIndex() { return mIndex; }
	char* getRecvBuffer() { return m_recvBuf; }

	bool IsSend() { return m_isSend; }

	void Init(UINT32 index)
	{
		mIndex = index;
	}

	bool OnConnect(HANDLE iocpHandle)
	{
		bool bRet = BindIOCompletionPort(iocpHandle);
		if (false == bRet)
		{
			return false;
		}

		return BindRecv();
	}

	bool PostAccept(SOCKET listenSocket)
	{
		mLastClosedTimeSec = UINT32_MAX;
		mIsConnected = 1;

		m_socketClient = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
		if (m_socketClient == SOCKET_ERROR)
			printf_s("[����] WSASocket() �Լ� ����: %d\n", GetLastError());

		ZeroMemory(&m_stAcceptOverlappedEx, sizeof(stOverlappedEx));

		/*
		BOOL AcceptEx(
			SOCKET sListenSocket,			// ���� ����
			SOCKET sAcceptSocket,			// ������ ���� ����
			PVOID lpOutPutBuffer,			// ���� �ּҿ� ����Ʈ �ּҸ� �ޱ� ���� ����, NULL�� �� �� ����
			DWORD dwReceiveDataLength,		// ���� �� ���۵Ǵ� ���� �����͸� �����ϱ� ���� ������ ũ��, �ʿ� ������ 0
			DWORD LocalAddressLength,		// ���� �ּ� ����ü ���� + 16
			DWORD dwRemoteAddressLength,	// ���� �ּ� ����ü ���� + 16
			LPWORD lpdwBytesReceived,		// lpOverlapped�� ����ϸ� ����, NULL �Է�
			LPOVERLAPPED lpOverlapped		// OVERLAPPED ����ü�� ������
		);
		*/

		DWORD bytes = 0;
		DWORD flags = 0;
		m_stAcceptOverlappedEx.m_wsaBuf.len = 0;
		m_stAcceptOverlappedEx.m_wsaBuf.buf = nullptr;
		m_stAcceptOverlappedEx.m_eOperation = IOOperation::ACCEPT;
		m_stAcceptOverlappedEx.m_clientIndex = mIndex;

		if (AcceptEx(listenSocket, m_socketClient, mAcceptBuf, 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, nullptr, (LPWSAOVERLAPPED)&m_stAcceptOverlappedEx) == false)
		{
			if (WSA_IO_PENDING != WSAGetLastError())
			{
				printf_s("AcceptEx Error : %d\n", GetLastError());
				return false;
			}
		}

		return true;
	}

	bool AcceptCompletion(HANDLE iocpHandle)
	{
		printf_s("AcceptCompletion : SessionIndex(%d)\n", mIndex);

		if (false == OnConnect(iocpHandle))
		{
			// CloseSocket(m_socketClient);
			return false;
		}

		SOCKADDR_IN clientAddr;

		int nAddrLen = sizeof(SOCKADDR_IN);
		char clientIP[32] = { 0, };
		inet_ntop(AF_INET, &(clientAddr.sin_addr), clientIP, 32 - 1);
		printf("Ŭ���̾�Ʈ ���� : IP(%s) SOCKET(%d)\n", clientIP, (int)m_socketClient);

		return true;
	}

	// Bind
	bool BindIOCompletionPort(HANDLE iocpHandle)
	{
		//socket�� pClientInfo�� CompletionPort��ü�� �����Ų��.
		auto hIOCP = CreateIoCompletionPort((HANDLE)m_socketClient, iocpHandle, (ULONG_PTR)(this), 0);

		if (NULL == hIOCP || iocpHandle != hIOCP)
		{
			printf("[����] CreateIoCompletionPort()�Լ� ����: %d\n", GetLastError());
			return false;
		}

		return true;
	}

	bool BindRecv()
	{
		DWORD dwFlag = 0;
		DWORD dwRecvNumBytes = 0;

		// Overlapped I/O ���� �� ���� ����
		m_stRecvOverlappedEx.m_wsaBuf.len = MAX_SOCKBUF;
		m_stRecvOverlappedEx.m_wsaBuf.buf = m_recvBuf;
		m_stRecvOverlappedEx.m_eOperation = IOOperation::RECV;

		int nRet = WSARecv(m_socketClient,
			&m_stRecvOverlappedEx.m_wsaBuf,
			1,
			&dwRecvNumBytes,
			&dwFlag,
			(LPWSAOVERLAPPED) & (m_stRecvOverlappedEx),
			NULL);

		//socket_error�̸� client socket�� �������ɷ� ó���Ѵ�.
		if (nRet == SOCKET_ERROR && (WSAGetLastError() != ERROR_IO_PENDING))
		{
			printf("[����] WSARecv()�Լ� ���� : %d\n", WSAGetLastError());
			return false;
		}

		return true;
	}

	bool PushSendQueue(UINT32 nLen, char* pMsg)
	{
		SendData* sendData = new SendData(nLen, pMsg);

		std::lock_guard<std::mutex> guard(mLock);
		m_sendQueue.push(sendData);
		
		return true;
	}

	bool SendIO()
	{
		if (m_isSend || m_sendQueue.empty())
			return true;

		// ������ �����ϸ� ���� �Ǵ�.
		std::lock_guard<std::mutex> guard(mLock);
		m_isSend = true;

		SendData* sendData = m_sendQueue.front();

		auto sendOverlappedEx = new stOverlappedEx;
		ZeroMemory(sendOverlappedEx, sizeof(stOverlappedEx));

		DWORD dwRecvNumBytes = 0;

		////���۵� �޼����� ����
		//CopyMemory(pClientInfo->m_sendBuf, pMsg, nLen);
		//pClientInfo->m_sendBuf[nLen] = '\0';



		//Overlapped I/O ���� �� ���� ����
		sendOverlappedEx->m_wsaBuf.len = sendData->m_len;
		sendOverlappedEx->m_wsaBuf.buf = sendData->m_msgData;
		sendOverlappedEx->m_eOperation = IOOperation::SEND;

		int nRet = WSASend(m_socketClient,
			&(sendOverlappedEx->m_wsaBuf),
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

	void CloseSocket(bool bIsForce = false)
	{
		struct linger stLinger = { 0, 0 };	// SO_DONTLINGER�� ����

	// bIsForce�� true�̸� SO_LINGER, timeout = 0���� �����Ͽ� ���� ���� ��Ų��. ���� : ������ �ս��� ������ ���� 
		if (true == bIsForce)
		{
			stLinger.l_onoff = 1;
		}

		// socketClose������ ������ �ۼ����� ��� �ߴ� ��Ų��.
		shutdown(m_socketClient, SD_BOTH);

		// ���� �ɼ��� �����Ѵ�.
		setsockopt(m_socketClient, SOL_SOCKET, SO_LINGER, (char*)&stLinger, sizeof(stLinger));

		mIsConnected = 0;
		mLastClosedTimeSec = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now().time_since_epoch()).count();

		// ���� ������ ���� ��Ų��. 
		closesocket(m_socketClient);

		m_socketClient = INVALID_SOCKET;
	}

	void SendCompleted(const UINT32 dataSize)
	{
		printf("[�۽� �Ϸ�] bytes : %d \n", dataSize);
		std::lock_guard<std::mutex> guard(mLock);
		SendData* sendData = m_sendQueue.front();
		m_sendQueue.pop();
		delete sendData;

		m_isSend = false;
	}


private:
	INT32					mIndex = 0;
	SOCKET					m_socketClient;
	stOverlappedEx			m_stRecvOverlappedEx;

	INT32					mIsConnected = 0;
	char					mAcceptBuf[64];
	UINT32					mLastClosedTimeSec = 0;
	stOverlappedEx			m_stAcceptOverlappedEx;

	std::mutex				mLock;
	bool					m_isSend = false;
	char					m_recvBuf[MAX_SOCKBUF]; // ������ ����
	std::queue<SendData*>	m_sendQueue;
};