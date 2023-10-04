#include "pch.h"
#include "Listener.h"
#include "SocketUtils.h"
#include "IocpEvent.h"
#include "Session.h"

/*--------------
	Listener
---------------*/

Listener::~Listener()
{
	SocketUtils::Close(mSocket);

	for (IocpEvent* acceptEvent : mAcceptEvents)
	{
		// TODO
		delete acceptEvent;
	}
}

bool Listener::StartAccept(NetAddress netAddress)
{
	mSocket = SocketUtils::CreateSocket();
	if (mSocket == INVALID_SOCKET)
		return false;

	// Iocp 큐 에 등록
	if (GIocpCore.Register(this) == false)
		return false;

	if (SocketUtils::SetReuseAddress(mSocket, true) == false)
		return false;

	if (SocketUtils::SetLinger(mSocket, 0, 0) == false)
		return false;

	if (SocketUtils::Bind(mSocket, netAddress) == false)
		return false;

	if (SocketUtils::Listen(mSocket) == false)
		return false;

	const int32 acceptCount = 1;
	for (int32 i = 0; i < acceptCount; i++)
	{
		IocpEvent* acceptEvent = new IocpEvent(EventType::Accept);
		mAcceptEvents.push_back(acceptEvent);
		RegisterAccept(acceptEvent);
	}

	return false;
}

void Listener::CloseSocket()
{
	SocketUtils::Close(mSocket);
}

HANDLE Listener::GetHandle()
{
	return reinterpret_cast<HANDLE>(mSocket);
}

void Listener::Dispatch(IocpEvent* acceptEvent, int32 numOfBytes)
{
	assert(acceptEvent->mType == EventType::Accept);
	ProcessAccept(acceptEvent);
}

void Listener::RegisterAccept(IocpEvent* acceptEvent)
{
	Session* session = new Session();

	acceptEvent->Init();
	acceptEvent->mSession = session;

	DWORD bytesReceived = 0;
	// Accept를 예약
	if (false == SocketUtils::AcceptEx(mSocket, session->GetSocket(), session->mRecvBuffer, 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, OUT & bytesReceived, static_cast<LPOVERLAPPED>(acceptEvent)))
	{
		const int32 errorCode = ::WSAGetLastError();
		// Pending 일 때 기다리는 중인 것
		if (errorCode != WSA_IO_PENDING)
		{
			// 일단 다시 Accept 걸어준다
			RegisterAccept(acceptEvent);
		}
	}
}

void Listener::ProcessAccept(IocpEvent* acceptEvent)
{
	Session* session = acceptEvent->mSession;

	if (false == SocketUtils::SetUpdateAcceptSocket(session->GetSocket(), mSocket))
	{
		RegisterAccept(acceptEvent);
		return;
	}

	SOCKADDR_IN sockAddress;
	int32 sizeOfSockAddr = sizeof(sockAddress);
	if (SOCKET_ERROR == ::getpeername(session->GetSocket(), OUT reinterpret_cast<SOCKADDR*>(&sockAddress), &sizeOfSockAddr))
	{
		RegisterAccept(acceptEvent);
		return;
	}

	session->SetNetAddress(NetAddress(sockAddress));

	cout << "Client Connected!" << endl;

	// TODO

	RegisterAccept(acceptEvent);
}