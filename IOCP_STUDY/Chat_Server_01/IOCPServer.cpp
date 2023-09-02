#include "IOCPServer.h"

bool IOCPServer::Init(const UINT32 maxIOWorkerThreadCount)
{
	return false;
}

bool IOCPServer::BindandListen(int bindPort)
{
	return false;
}

bool IOCPServer::ServerStart(const UINT32 maxClientCount)
{
	return false;
}

void IOCPServer::DestroyThread()
{
}

bool IOCPServer::SendMsg(const UINT32 clientIndex, const UINT32 dataSize, char* pData)
{
	return false;
}

void IOCPServer::OnConnect(const UINT32 clientIndex)
{
}

void IOCPServer::OnClose(const UINT32 clientIndex)
{
}

void IOCPServer::OnReceive(const UINT32 clientIndex, const UINT32 dataSize, char* pData)
{
}

void IOCPServer::CreateClient(const UINT32 maxClientCount)
{
}

ClientInfo* IOCPServer::GetEmptyClientInfo()
{
	return nullptr;
}

ClientInfo* IOCPServer::GetClientInfo(const UINT32 clientIndex)
{
	return nullptr;
}

bool IOCPServer::CreateWorkerThread()
{
	return false;
}

bool IOCPServer::CreateAccepterThread()
{
	return false;
}

void IOCPServer::WorkerThread()
{
}

void IOCPServer::AccepterThread()
{
}

void IOCPServer::CloseSocket()
{
}
