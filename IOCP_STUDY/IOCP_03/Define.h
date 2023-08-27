#pragma once
#include "pch.h"

#define MAX_SOCKBUF 256
#define MAX_WORKERTHREAD 4

enum class IOOperation
{
	RECV,
	SEND,
};

// WSAOVERLAPPED ����ü Ȯ��
struct stOverlappedEx
{
	WSAOVERLAPPED	m_wsaOverlapped;
	SOCKET			m_socketClient;
	WSABUF			m_wsaBuf;
	IOOperation		m_eOperation;
};

// Ŭ���̾�Ʈ ������ ��� ����ü
struct stClientInfo
{
	SOCKET			m_socketClient;
	stOverlappedEx	m_stRecvOverlappedEx;
	stOverlappedEx	m_stSendOverlappedEx;

	char			m_recvBuf[MAX_SOCKBUF]; // ������ ����
	char			m_sendBuf[MAX_SOCKBUF]; // ������ ����

	stClientInfo()
	{
		// �޸� ������ 0X00���� ä��� ��ũ�� : memset�� ����
		ZeroMemory(&m_stRecvOverlappedEx, sizeof(stOverlappedEx));
		ZeroMemory(&m_stSendOverlappedEx, sizeof(stOverlappedEx));
		m_socketClient = INVALID_SOCKET;
	}
};