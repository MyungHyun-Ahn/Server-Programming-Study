#include "pch.h"
#include "RedisManager.h"

bool RedisManager::Run(std::string ip, UINT16 port, const UINT32 threadCount)
{
	if (Connect(ip, port) == false)
	{
		printf("Redis 접속 실패\n");
		return false;
	}

#if _DEBUG
	Test();
#endif

	_isTaskRun = true;

	for (UINT32 i = 0; i < threadCount; i++)
	{
		_taskThreads.emplace_back([this]() { TaskProcessThread(); });
	}

	printf("Redis 동작 중...\n");
	return true;
}

void RedisManager::End()
{
	_isTaskRun = false;

	for (auto& thread : _taskThreads)
	{
		if (thread.joinable())
			thread.join();
	}
}

void RedisManager::PushTask(RedisTask task)
{
	std::lock_guard<std::mutex> guard(_reqLock);
	_requestTask.push_back(task);
}

RedisTask RedisManager::TakeResponseTask()
{
	std::lock_guard<std::mutex> guard(_resLock);
	if (_responseTask.empty())
		return RedisTask();

	auto task = _responseTask.front();
	_responseTask.pop_front();
	return task;
}

bool RedisManager::Connect(std::string ip, UINT16 port)
{
	if (_Conn.connect(ip, port) == false)
	{
		std::cout << "connect error " << _Conn.getErrorStr() << std::endl;
		return false;
	}

	if (_Conn.auth("redispass") == false)
	{
		std::cout << "auth error " << _Conn.getErrorStr() << std::endl;
		return false;
	}
	
	std::cout << "connect success !!!" << std::endl;

	return true;
}

void RedisManager::TaskProcessThread()
{
	printf("Redis 스레드 시작...\n");

	while (_isTaskRun)
	{
		bool isIdle = true;

		auto task = TakeRequestTask();
		if (task.TaskID != RedisTaskID::INVALID)
		{
			isIdle = false;

			if (task.TaskID == RedisTaskID::REQUEST_LOGIN)
			{
				TakeLoginRequestTask(task);
			}

			task.Release();
		}
	}
}

RedisTask RedisManager::TakeRequestTask()
{
	std::lock_guard<std::mutex> guard(_reqLock);

	if (_requestTask.empty())
	{
		return RedisTask();
	}

	auto task = _requestTask.front();
	_requestTask.pop_front();

	return task;
}

void RedisManager::TakeLoginRequestTask(RedisTask task)
{
	auto pRequest = (RedisLoginReq*)task.pData;

	RedisLoginRes bodyData;
	bodyData.Result = (UINT16)ERROR_CODE::LOGIN_USER_INVALID_PW;

	std::string value;
	if (_Conn.get(pRequest->UserID, value))
	{
		if (value.compare(pRequest->UserPW) == 0)
		{
			bodyData.Result = (UINT16)ERROR_CODE::NONE;
		}
	}

	RedisTask resTask;
	resTask.UserIndex = task.UserIndex;
	resTask.TaskID = RedisTaskID::RESPONSE_LOGIN;
	resTask.DataSize = sizeof(RedisLoginRes);
	resTask.pData = new char[resTask.DataSize];
	CopyMemory(resTask.pData, (char*)&bodyData, resTask.DataSize);

	PushResponse(resTask);
}

void RedisManager::PushResponse(RedisTask task)
{
	std::lock_guard<std::mutex> guard(_resLock);
	_responseTask.push_back(task);
}

void RedisManager::Test()
{
	{
		uint32_t ret;
		if (!_Conn.set("test777", "test1", ret))
		{
			std::cout << "set error: " << _Conn.getErrorStr() << std::endl;
		}
		else
		{
			std::cout << "auth success" << std::endl;
		}
	}

	{
		uint32_t ret;
		if (!_Conn.set("test2", "test2", ret))
		{
			std::cout << "set error: " << _Conn.getErrorStr() << std::endl;
		}
		else
		{
			std::cout << "auth success" << std::endl;
		}
	}

	{
		uint32_t ret;
		if (!_Conn.set("test2", "test2", ret))
		{
			std::cout << "set error: " << _Conn.getErrorStr() << std::endl;
		}
		else
		{
			std::cout << "auth success" << std::endl;
		}
	}
}
