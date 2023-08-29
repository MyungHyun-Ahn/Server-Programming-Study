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