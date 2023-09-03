#pragma once
#include "IOCPServer.h"
#include "PacketManager.h"

class ChatServer : public IOCPServer
{
public:
	ChatServer() = default;
	virtual ~ChatServer() = default;

	virtual void OnConnect(const UINT32 clientIndex) override;
	virtual void OnClose(const UINT32 clientIndex) override;
	virtual void OnReceive(const UINT32 clientIndex, const UINT32 size, char* pData) override;

	void Run(const UINT32 maxClient);
	void End();

private:
	std::unique_ptr<PacketManager> _pPacketManager;
};

