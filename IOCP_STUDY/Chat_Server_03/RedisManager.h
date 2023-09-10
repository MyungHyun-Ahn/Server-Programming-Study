#pragma once
#include "../Redis/RedisConn.h"

class RedisManager
{
public:
	RedisManager() = default;
	~RedisManager() = default;

	bool		Run(std::string ip, UINT16 port, const UINT32 threadCount);
	void		End();

	void		PushTask(RedisTask task);

	RedisTask	TakeResponseTask();

private:
	bool		Connect(std::string ip, UINT16 port);
	void		TaskProcessThread();
	RedisTask	TakeRequestTask();

	void		TakeLoginRequestTask(RedisTask task);
	void		PushResponse(RedisTask task);

	void		Test();

private:
	RedisCpp::CRedisConn		_Conn;
	bool						_isTaskRun = false;
	std::vector<std::thread>	_taskThreads;

	std::mutex					_reqLock;
	std::deque<RedisTask>		_requestTask;

	std::mutex					_resLock;
	std::deque<RedisTask>		_responseTask;
};

