#pragma once
#include "pch.h"

#define MAX_SOCKBUF 256
#define MAX_WORKERTHREAD 4

enum class IOOperation
{
	RECV,
	SEND,
};

// WSAOVERLAPPED 구조체 확장
struct stOverlappedEx
{
	WSAOVERLAPPED	m_wsaOverlapped;
	SOCKET			m_socketClient;
	WSABUF			m_wsaBuf;
	IOOperation		m_eOperation;
};

// 클라이언트 정보를 담는 구조체
struct stClientInfo
{
	SOCKET			m_socketClient;
	stOverlappedEx	m_stRecvOverlappedEx;
	stOverlappedEx	m_stSendOverlappedEx;

	char			m_recvBuf[MAX_SOCKBUF]; // 데이터 버퍼
	char			m_sendBuf[MAX_SOCKBUF]; // 데이터 버퍼

	stClientInfo()
	{
		// 메모리 영역을 0X00으로 채우는 매크로 : memset과 동일
		ZeroMemory(&m_stRecvOverlappedEx, sizeof(stOverlappedEx));
		ZeroMemory(&m_stSendOverlappedEx, sizeof(stOverlappedEx));
		m_socketClient = INVALID_SOCKET;
	}
};