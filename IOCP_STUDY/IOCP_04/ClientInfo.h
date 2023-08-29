#pragma once
#include "pch.h"

// 클라이언트 정보를 담는 구조체

// 만들어야할 것
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
		// 메모리 영역을 0X00으로 채우는 매크로 : memset과 동일
		ZeroMemory(&m_stRecvOverlappedEx, sizeof(stOverlappedEx));
		m_socketClient = INVALID_SOCKET;
	}

	~stClientInfo() {}


	SOCKET getClientSocket() { return m_socketClient; }
	void setClientSocket(SOCKET clientSocket) { m_socketClient = clientSocket; }
	INT32 getIndex() { return mIndex; }

	char* getRecvBuffer() { return m_recvBuf; }

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
		//socket과 pClientInfo를 CompletionPort객체와 연결시킨다.
		auto hIOCP = CreateIoCompletionPort((HANDLE)m_socketClient, iocpHandle, (ULONG_PTR)(this), 0);

		if (NULL == hIOCP || iocpHandle != hIOCP)
		{
			printf("[에러] CreateIoCompletionPort()함수 실패: %d\n", GetLastError());
			return false;
		}

		return true;
	}

	bool BindRecv()
	{
		DWORD dwFlag = 0;
		DWORD dwRecvNumBytes = 0;

		// Overlapped I/O 위해 각 정보 세팅
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

		//socket_error이면 client socket이 끊어진걸로 처리한다.
		if (nRet == SOCKET_ERROR && (WSAGetLastError() != ERROR_IO_PENDING))
		{
			printf("[에러] WSARecv()함수 실패 : %d\n", WSAGetLastError());
			return false;
		}

		return true;
	}

	bool SendMsg(UINT32 nLen, char* pMsg)
	{
		auto sendOverlappedEx = new stOverlappedEx;
		ZeroMemory(sendOverlappedEx, sizeof(stOverlappedEx));

		DWORD dwRecvNumBytes = 0;

		////전송될 메세지를 복사
		//CopyMemory(pClientInfo->m_sendBuf, pMsg, nLen);
		//pClientInfo->m_sendBuf[nLen] = '\0';


		//Overlapped I/O 위해 각 정보 세팅
		sendOverlappedEx->m_wsaBuf.len = nLen;
		sendOverlappedEx->m_wsaBuf.buf = new char[nLen];
		CopyMemory(sendOverlappedEx->m_wsaBuf.buf, pMsg, nLen);
		sendOverlappedEx->m_eOperation = IOOperation::SEND;

		int nRet = WSASend(m_socketClient,
			&(sendOverlappedEx->m_wsaBuf),
			1,
			&dwRecvNumBytes,
			0,
			(LPWSAOVERLAPPED)sendOverlappedEx,
			NULL);

		//socket_error이면 client socket이 끊어진걸로 처리한다.
		if (nRet == SOCKET_ERROR && (WSAGetLastError() != ERROR_IO_PENDING))
		{
			printf("[에러] WSASend()함수 실패 : %d\n", WSAGetLastError());
			return false;
		}
		return true;
	}

	void CloseSocket(bool bIsForce = false)
	{
		struct linger stLinger = { 0, 0 };	// SO_DONTLINGER로 설정

	// bIsForce가 true이면 SO_LINGER, timeout = 0으로 설정하여 강제 종료 시킨다. 주의 : 데이터 손실이 있을수 있음 
		if (true == bIsForce)
		{
			stLinger.l_onoff = 1;
		}

		// socketClose소켓의 데이터 송수신을 모두 중단 시킨다.
		shutdown(m_socketClient, SD_BOTH);

		// 소켓 옵션을 설정한다.
		setsockopt(m_socketClient, SOL_SOCKET, SO_LINGER, (char*)&stLinger, sizeof(stLinger));

		// 소켓 연결을 종료 시킨다. 
		closesocket(m_socketClient);

		m_socketClient = INVALID_SOCKET;
	}

	void SendCompleted(const UINT32 dataSize)
	{
		printf("[송신 완료] bytes : %d \n", dataSize);
	}


private:
	INT32			mIndex = 0;
	SOCKET			m_socketClient;
	stOverlappedEx	m_stRecvOverlappedEx;

	char			m_recvBuf[MAX_SOCKBUF]; // 데이터 버퍼
};