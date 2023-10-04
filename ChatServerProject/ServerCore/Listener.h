#pragma once
#include "IocpCore.h"
#include "NetAddress.h"

// 클라이언트 입장을 관리

class Listener : public IocpObject
{
public:
	Listener() = default;
	~Listener();

public:
	/* 외부에서 사용 */
	bool StartAccept(NetAddress netAddress);
	void CloseSocket();

public:
	/* 인터페이스 구현 */
	virtual HANDLE GetHandle() override;
	virtual void Dispatch(struct IocpEvent* iocpEvent, int32 numOfBytes = 0) override;

private:
	/* 수신 관련 */
	void RegisterAccept(IocpEvent* acceptEvent);
	void ProcessAccept(IocpEvent* acceptEvent);

protected:
	SOCKET mSocket = INVALID_SOCKET;
	vector<IocpEvent*> mAcceptEvents;
};

