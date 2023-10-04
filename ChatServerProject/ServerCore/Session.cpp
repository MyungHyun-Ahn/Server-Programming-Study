#include "pch.h"
#include "Session.h"
#include "SocketUtils.h"

Session::Session()
{
	// �̸� ������ �����ϰ� ���⿡ ���
	mSocket = SocketUtils::CreateSocket();
}

Session::~Session()
{
	SocketUtils::Close(mSocket);
}

HANDLE Session::GetHandle()
{
	return reinterpret_cast<HANDLE>(mSocket);
}

void Session::Dispatch(IocpEvent* iocpEvent, int32 numOfBytes)
{
	// TODO
}