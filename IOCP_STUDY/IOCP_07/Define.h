#pragma once
#include "pch.h"

const UINT32 MAX_SOCKBUF = 256;	// 소켓 버퍼의 크기
const UINT32 MAX_SOCK_SENDBUF = 4096;	// 소켓 버퍼의 크기
const UINT32 MAX_WORKERTHREAD = 4;  //쓰레드 풀에 넣을 쓰레드 수
const UINT64 RE_USE_SESSION_WAIT_TIMESEC = 3;

enum class IOOperation
{
	RECV,
	SEND,
	ACCEPT,
};

// WSAOVERLAPPED 구조체 확장
struct stOverlappedEx
{
	WSAOVERLAPPED	m_wsaOverlapped;
	WSABUF			m_wsaBuf;
	IOOperation		m_eOperation;
	UINT32			m_clientIndex;
};

class SendData
{
public:
	SendData() {}
	SendData(UINT32 len, char* msg) : m_len(len), m_msgData(msg) {}

	UINT32		m_len = 0;
	char*		m_msgData = nullptr;
};