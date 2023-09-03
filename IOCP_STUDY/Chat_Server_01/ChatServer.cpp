#include "pch.h"
#include "ChatServer.h"

void ChatServer::OnConnect(const UINT32 clientIndex)
{
	printf("[OnConnect] 클라이언트: Index(%d)\n", clientIndex);

	PacketInfo packet{ clientIndex, (UINT16)PACKET_ID::SYS_USER_CONNECT, 0 };
	_pPacketManager->PushSystemPacket(packet);
}

void ChatServer::OnClose(const UINT32 clientIndex)
{
	printf("[OnClose] 클라이언트: Index(%d)\n", clientIndex);

	PacketInfo packet{ clientIndex, (UINT16)PACKET_ID::SYS_USER_DISCONNECT, 0 };
	_pPacketManager->PushSystemPacket(packet);
}

void ChatServer::OnReceive(const UINT32 clientIndex, const UINT32 size, char* pData)
{
	printf("[OnReceive] 클라이언트: Index(%d), dataSize(%d)\n", clientIndex, size);

	_pPacketManager->ReceivePacketData(clientIndex, size, pData);
}

void ChatServer::Run(const UINT32 maxClient)
{
	auto sendPacketFunc = [&](UINT32 clientIndex, UINT16 packetSize, char* pSendPacket)
	{
		SendMsg(clientIndex, packetSize, pSendPacket);
	};

	_pPacketManager = std::make_unique<PacketManager>();
	_pPacketManager->SendPacketFunc = sendPacketFunc;
	_pPacketManager->Init(maxClient);
	_pPacketManager->Run();

	ServerStart(maxClient);
}

void ChatServer::End()
{
	_pPacketManager->End();
	DestroyThread();
}
