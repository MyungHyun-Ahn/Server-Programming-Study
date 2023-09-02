#pragma once
#include "pch.h"

const UINT32 MAX_SOCK_RECVBUF = 256;	// ���� ������ ũ��
const UINT32 MAX_SOCK_SENDBUF = 4096;	// ���� ������ ũ��
const UINT64 RE_USE_SESSION_WAIT_TIMESEC = 3;


enum class IOOperation
{
	ACCEPT,
	RECV,
	SEND
};

//WSAOVERLAPPED����ü�� Ȯ�� ���Ѽ� �ʿ��� ������ �� �־���.
struct OverlappedEx
{
	WSAOVERLAPPED	_wsaOverlapped;		//Overlapped I/O����ü
	WSABUF			_wsaBuf;				//Overlapped I/O�۾� ����
	IOOperation		_operation;			//�۾� ���� ����
	UINT32			_sessionIndex = 0;
};
