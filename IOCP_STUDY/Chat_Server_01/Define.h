#pragma once
#include "pch.h"

const UINT32 MAX_SOCK_RECVBUF = 256;	// 소켓 버퍼의 크기
const UINT32 MAX_SOCK_SENDBUF = 4096;	// 소켓 버퍼의 크기
const UINT64 RE_USE_SESSION_WAIT_TIMESEC = 3;


enum class IOOperation
{
	ACCEPT,
	RECV,
	SEND
};

//WSAOVERLAPPED구조체를 확장 시켜서 필요한 정보를 더 넣었다.
struct OverlappedEx
{
	WSAOVERLAPPED	_wsaOverlapped;		//Overlapped I/O구조체
	WSABUF			_wsaBuf;				//Overlapped I/O작업 버퍼
	IOOperation		_eOperation;			//작업 동작 종류
	UINT32			_sessionIndex = 0;
};
