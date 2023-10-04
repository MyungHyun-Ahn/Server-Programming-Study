#include "pch.h"
#include "IocpCore.h"
#include "IocpEvent.h"

// TEMP
IocpCore GIocpCore;

/*--------------
	IocpCore
---------------*/

// 생성자에서 IOCP 큐를 생성
IocpCore::IocpCore()
{
	mIocpHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
	assert(mIocpHandle != INVALID_HANDLE_VALUE);
}

IocpCore::~IocpCore()
{
	::CloseHandle(mIocpHandle);
}

// 소켓을 IOCP 큐에 등록
bool IocpCore::Register(IocpObject* iocpObject)
{
	return ::CreateIoCompletionPort(iocpObject->GetHandle(), mIocpHandle, reinterpret_cast<ULONG_PTR>(iocpObject), 0);
}

bool IocpCore::Dispatch(uint32 timeoutMs)
{
	DWORD numOfBytes = 0;
	IocpObject* iocpObject = nullptr;
	IocpEvent* iocpEvent = nullptr;

	// 완료 정보를 가져와서
	if (::GetQueuedCompletionStatus(mIocpHandle, OUT & numOfBytes, OUT reinterpret_cast<PULONG_PTR>(&iocpObject), OUT reinterpret_cast<LPOVERLAPPED*>(&iocpEvent), timeoutMs))
	{
		// 일감을 꺼내서 처리 요청
		iocpObject->Dispatch(iocpEvent, numOfBytes);
	}
	else
	{
		int32 errCode = ::WSAGetLastError();
		switch (errCode)
		{
		case WAIT_TIMEOUT:
			return false;
		default:
			// TODO : 로그 찍기
			iocpObject->Dispatch(iocpEvent, numOfBytes);
			break;
		}
	}

	return true;
}