#pragma once
#include "pch.h"

/*
실습 7단계 만들어야할 것
AcceptEx 함수로 비동기 accept 예약 : PostAccept
완료되면 accept 완료처리 : AcceptCompletion

AcceptEx() 함수 사용법
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
		// 메모리 영역을 0X00으로 채우는 매크로 : memset과 동일
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
			printf_s("[에러] WSASocket() 함수 실패: %d\n", GetLastError());

		ZeroMemory(&m_stAcceptOverlappedEx, sizeof(stOverlappedEx));

		/*
		BOOL AcceptEx(
			SOCKET sListenSocket,			// 리슨 소켓
			SOCKET sAcceptSocket,			// 접속을 받을 소켓
			PVOID lpOutPutBuffer,			// 로컬 주소와 리모트 주소를 받기 위한 버퍼, NULL이 될 수 없음
			DWORD dwReceiveDataLength,		// 접속 후 전송되는 최초 데이터를 수신하기 위한 버퍼의 크기, 필요 없으면 0
			DWORD LocalAddressLength,		// 소켓 주소 구조체 길이 + 16
			DWORD dwRemoteAddressLength,	// 소켓 주소 구조체 길이 + 16
			LPWORD lpdwBytesReceived,		// lpOverlapped를 사용하면 무시, NULL 입력
			LPOVERLAPPED lpOverlapped		// OVERLAPPED 구조체의 포인터
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
			CloseSocket(m_socketClient);
			return false;
		}

		SOCKADDR_IN clientAddr;

		int nAddrLen = sizeof(SOCKADDR_IN);
		char clientIP[32] = { 0, };
		inet_ntop(AF_INET, &(clientAddr.sin_addr), clientIP, 32 - 1);
		printf("클라이언트 접속 : IP(%s) SOCKET(%d)\n", clientIP, (int)m_socketClient);

		return true;
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

		// 전송을 시작하면 락을 건다.
		std::lock_guard<std::mutex> guard(mLock);
		m_isSend = true;

		SendData* sendData = m_sendQueue.front();

		auto sendOverlappedEx = new stOverlappedEx;
		ZeroMemory(sendOverlappedEx, sizeof(stOverlappedEx));

		DWORD dwRecvNumBytes = 0;

		////전송될 메세지를 복사
		//CopyMemory(pClientInfo->m_sendBuf, pMsg, nLen);
		//pClientInfo->m_sendBuf[nLen] = '\0';



		//Overlapped I/O 위해 각 정보 세팅
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

		mIsConnected = 0;
		mLastClosedTimeSec = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now().time_since_epoch()).count();

		// 소켓 연결을 종료 시킨다. 
		closesocket(m_socketClient);

		m_socketClient = INVALID_SOCKET;
	}

	void SendCompleted(const UINT32 dataSize)
	{
		printf("[송신 완료] bytes : %d \n", dataSize);
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
	char					m_recvBuf[MAX_SOCKBUF]; // 데이터 버퍼
	std::queue<SendData*>	m_sendQueue;
};