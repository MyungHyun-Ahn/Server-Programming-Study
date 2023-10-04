#pragma once

// IOCP 큐에 등록 가능한 모든 것은 이것을 상속 받아 구현
class IocpObject
{
public:
	virtual ~IocpObject() {}

	virtual HANDLE GetHandle() abstract;
	virtual void Dispatch(struct IocpEvent* iocpEvent, int32 numOfBytes = 0) abstract;
};

class IocpCore
{
public:
	IocpCore();
	~IocpCore();

	HANDLE GetHandle() { return mIocpHandle; }

	bool Register(class IocpObject* iocpObject);
	bool Dispatch(uint32 timeoutMs = INFINITE);

private:
	HANDLE mIocpHandle;
};

extern IocpCore GIocpCore;
