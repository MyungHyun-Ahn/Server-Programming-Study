#include "pch.h"
#include "RedisManager.h"

bool RedisManager::Run(std::string ip, UINT16 port, const UINT32 threadCount)
{
	return false;
}

void RedisManager::End()
{
}

void RedisManager::PushTask(RedisTask task)
{
}

RedisTask RedisManager::TakeResponseTask()
{
	return RedisTask();
}

bool RedisManager::Connect(std::string ip, UINT16 port)
{
	return false;
}

void RedisManager::TaskProcessThread()
{
}

RedisTask RedisManager::TakeRequestTask()
{
	return RedisTask();
}

void RedisManager::PushResponse(RedisTask task)
{
}
