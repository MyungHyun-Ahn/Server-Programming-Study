#pragma once
#include "IOCPServer.h"

class EchoServer : public IOCPServer
{
	virtual void OnConnect(const UINT32 clientIdx) override
	{
		printf("[OnConnect] 클라이언트 접속 : Index(%d) \n", clientIdx);
	}

	virtual void OnClose(const UINT32 clientIdx) override
	{
		printf("[OnClose] 클라이언트 종료 : Index(%d) \n", clientIdx);
	}

	virtual void OnReceive(const UINT32 clientIdx, const UINT32 dataSize, char* data) override
	{
		printf("[OnReceive] 클라이언트 : Index(%d) : 데이터 수신 %d bytes, 수신된 데이터 : %s \n", clientIdx, dataSize, data);
	}
};