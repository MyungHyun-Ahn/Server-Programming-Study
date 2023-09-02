#pragma once
#include "ClientInfo.h"

void ClientInfo::Init(const UINT32 index, HANDLE iocpHandle)
{
}

bool ClientInfo::OnConnect(HANDLE iocpHandle, SOCKET sock)
{
	return false;
}

void ClientInfo::Close(bool isForce)
{
}

void ClientInfo::Clear()
{
}

bool ClientInfo::PostAccept(SOCKET listenSock, const UINT64 curTimeSec)
{
	return false;
}

bool ClientInfo::AcceptCompletion()
{
	return false;
}

bool ClientInfo::BindIOCP(HANDLE iocpHandle)
{
	return false;
}

bool ClientInfo::BindRecv()
{
	return false;
}

bool ClientInfo::SendMsg()
{
	return false;
}

void ClientInfo::SendCompleted(const UINT32 dataSize)
{
}

bool ClientInfo::SendIO()
{
	return false;
}

bool ClientInfo::SetSocketOption()
{
	return false;
}
