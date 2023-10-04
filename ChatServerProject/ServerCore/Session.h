#pragma once
#include "IocpCore.h"
#include "IocpEvent.h"
#include "NetAddress.h"

// ������ ���
class Session : public IocpObject
{
public:
	Session();
	virtual ~Session();

public:
	/* ���� ���� */
	void		SetNetAddress(NetAddress address) { mNetAddress = address; }
	NetAddress	GetAddress() { return mNetAddress; }
	SOCKET		GetSocket() { return mSocket; }

public:
	/* �������̽� ���� */
	virtual HANDLE		GetHandle() override;
	virtual void		Dispatch(struct IocpEvent* iocpEvent, int32 numOfBytes = 0) override;

public:
	// TEMP
	char mRecvBuffer[1000];

private:
	SOCKET			mSocket = INVALID_SOCKET;
	NetAddress		mNetAddress = {};
	Atomic<bool>	mConnected = false;
};


