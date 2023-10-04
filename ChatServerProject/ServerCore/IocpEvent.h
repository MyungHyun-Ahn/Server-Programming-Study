#pragma once

class Session;

// Disconnect
enum class EventType : uint8
{
	Connect,
	Accept,
	Recv,
	Send
};


struct IocpEvent : public OVERLAPPED
{
	IocpEvent(EventType type);

	void		Init();

	EventType	mType;
	Session*	mSession = nullptr; // Accept Only
};