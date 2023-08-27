#pragma once
#include "IOCPServer.h"

class EchoServer : public IOCPServer
{
	virtual void OnConnect(const UINT32 clientIdx) override
	{
		printf("[OnConnect] Ŭ���̾�Ʈ ���� : Index(%d) \n", clientIdx);
	}

	virtual void OnClose(const UINT32 clientIdx) override
	{
		printf("[OnClose] Ŭ���̾�Ʈ ���� : Index(%d) \n", clientIdx);
	}

	virtual void OnReceive(const UINT32 clientIdx, const UINT32 dataSize, char* data) override
	{
		printf("[OnReceive] Ŭ���̾�Ʈ : Index(%d) : ������ ���� %d bytes, ���ŵ� ������ : %s \n", clientIdx, dataSize, data);
	}
};