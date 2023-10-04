#include "pch.h"
#include "ThreadManager.h"

ThreadManager::ThreadManager()
{
	// Main Thread
	InitTLS();
}

ThreadManager::~ThreadManager()
{
	Join();
}

void ThreadManager::Launch(function<void(void)> callback)
{
	LockGuard guard(mLock);

	mThreads.push_back(thread([=]()
		{
			InitTLS();
			callback();
			DestroyTLS();
		}));


}

void ThreadManager::Join()
{
	for (thread& th : mThreads)
	{
		if (th.joinable())
			th.join();
	}

	mThreads.clear();
}

void ThreadManager::InitTLS()
{
	static Atomic<uint32> SThreadId = 1;
	LThreadId = SThreadId.fetch_add(1);
}

void ThreadManager::DestroyTLS()
{
}
