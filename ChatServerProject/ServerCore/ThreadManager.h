#pragma once
#include <thread>
#include <functional>

class ThreadManager
{
public:
	ThreadManager();
	~ThreadManager();

	void			Launch(function<void(void)> callback);
	void			Join();

private:
	static void		InitTLS();
	static void		DestroyTLS();

private:
	Mutex			mLock;
	vector<thread>	mThreads;
};

