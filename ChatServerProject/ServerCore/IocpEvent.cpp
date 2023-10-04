#include "pch.h"
#include "IocpEvent.h"

IocpEvent::IocpEvent(EventType type) : mType(type)
{
	Init();
}

void IocpEvent::Init()
{
	hEvent = 0;
	Internal = 0;
	InternalHigh = 0;
	Offset = 0;
	OffsetHigh = 0;
}