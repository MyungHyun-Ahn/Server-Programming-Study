#pragma once
#include "pch.h"

// Ŭ���̾�Ʈ ������ ��� ����ü

// �������� ��
// Init v
// OnConnect v
// Close v
// BindIOCompletionPort v
// BindRecv() v
// SendMsg v
// SendCompleted v

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
	
	std::mutex				mLock;
	bool					m_isSend = false;
	char					m_recvBuf[MAX_SOCKBUF]; // ������ ����
	std::queue<SendData*>	m_sendQueue;
};