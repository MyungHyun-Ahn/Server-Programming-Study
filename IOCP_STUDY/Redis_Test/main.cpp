#include "RedisConn.h"
#include <iostream>
#include <windows.h>

using  RedisCpp::CRedisConn;
using  RedisCpp::INSERT_POS;

const std::string  PASSWORD = "redispass";

void TestList()
{

	int64_t ret = 0;
	uint64_t ret2 = 0;
	CRedisConn con;
	std::string value;

	if (!con.connect("127.0.0.1", 6379))
	{
		std::cout << "connect error " << con.getErrorStr() << std::endl;
		return;
	}

	if (!con.auth(PASSWORD))
	{
		std::cout << "auth error: " << con.getErrorStr() << std::endl;
	}
	else
	{
		std::cout << "auth success" << std::endl;
	}

	if (!con.lindex("test", 1, value))
	{
		std::cout << "lindex error: " << con.getErrorStr() << std::endl;
	}
	else
	{
		std::cout << value.c_str() << std::endl;
	}

}

void TestHash(void)
{

	int64_t ret = 0;
	uint64_t ret2 = 0;
	uint32_t ret3;
	RedisCpp::ValueMap valueMap;
	std::string value;
	CRedisConn con;
	if (!con.connect("127.0.0.1", 6379))
	{
		std::cout << "connect error-- " << con.getErrorStr() << std::endl;
		return;
	}

	if (!con.auth(PASSWORD))
	{
		std::cout << "auth error: " << con.getErrorStr() << std::endl;
	}
	else
	{
		std::cout << "auth success" << std::endl;
	}

	RedisCpp::ValueMap::iterator it;
}

void Test(void)
{
	CRedisConn con;
	RedisCpp::ValueMap::iterator it;
	RedisCpp::ValueMap valueMap;
	RedisCpp::ValueList valueList;
	std::string value;

	int64_t ret = 0;
	uint64_t ret2 = 0;
	uint32_t ret3;
	//try
	//{
	while (1)
	{	//< host,port错误；redis-server未开启;
		if (!con.connect("127.0.0.1", 6379, "", 0))
		{
			std::cout << "connect error: " << con.getErrorStr() << std::endl;
			Sleep(1);
			continue;
		}
		break;
	}
	while (1)
	{
		std::cout << "...." << std::endl;
		//sleep(5);
		if (!con.auth(PASSWORD))
		{
			std::cout << "auth error: " << con.getErrorStr() << std::endl;
			//sleep(10);
			continue;
		}
		else
		{
			std::cout << "auth success" << std::endl;
			break;

		}
	}

	for (int j = 0; j < 1000; j++)
	{
		///////////////////////////hash////////////////////////////////////
		std::cout << "hgetll..............." << std::endl << std::endl;
		if (!con.hgetall("car", valueMap))
		{
			std::cout << "hgetall error: " << con.getErrorStr() << std::endl;
		}
		else
		{
			std::cout << "hgetall :" << std::endl;
			for (it = valueMap.begin(); it != valueMap.end(); ++it)
				std::cout << "field: " << it->first << " value: " << it->second << std::endl;
		}


		std::cout << "hget..............." << std::endl << std::endl;
		if (!con.hget("carC", "color", value))
		{
			std::cout << "hget error: " << con.getErrorStr() << std::endl;
		}
		else
		{
			std::cout << "hash value = " << value.c_str() << std::endl;
		}

		std::cout << "hset..............." << std::endl << std::endl;
		if (!con.hset("test", "colddddr", "bue", ret3))
		{
			std::cout << "hget error: " << con.getErrorStr() << std::endl;
		}
		else
		{
			if (ret3 == 1)
			{
				std::cout << " ret = 1; set a new value " << std::endl;
			}
			else if (ret3 == 0)
			{
				std::cout << "ret = 0; recovered value " << std::endl;
			}

		}

		std::cout << "hdel..............." << std::endl << std::endl;
		if (!con.hdel("car", "color", ret3))
		{
			std::cout << "hdel error: " << con.getErrorStr() << std::endl;
		}
		else
		{
			std::cout << "ret3 = " << ret3 << std::endl;
		}


		std::cout << "_errStr" << con.getErrorStr() << std::endl;


		///////////////////////////list////////////////////////////////////

		std::cout << "lpush..............." << std::endl << std::endl;
		if (!con.lpush("Ltest", "yuhaiyang", ret2))
		{
			std::cout << "lpush error: " << con.getErrorStr() << std::endl;
		}
		else
		{
			std::cout << "len = " << ret2 << std::endl;
		}


		std::cout << "lpop..............." << std::endl << std::endl;
		if (!con.lpop("Ltest", value))
		{
			std::cout << "lpop error: " << con.getErrorStr() << std::endl;
		}
		else
		{
			std::cout << "value: " << value << std::endl;
		}


		std::cout << "rpush..............." << std::endl << std::endl;
		if (!con.rpush("LtestM", "yuhaiyang", ret2))
		{
			std::cout << "Lrpush error: " << con.getErrorStr() << std::endl;
		}
		else
		{
			std::cout << "Llen = " << ret2 << std::endl;
		}


		std::cout << "rpop..............." << std::endl << std::endl;
		if (!con.rpop("LAtest", value))
		{
			std::cout << "rpop error: " << con.getErrorStr() << std::endl;
		}
		else
		{
			std::cout << "value: " << value << std::endl;
		}

		std::cout << "llen..............." << std::endl << std::endl;
		if (!con.llen("Ltest", ret2))
		{
			std::cout << "llen error: " << con.getErrorStr() << std::endl;

		}
		else
		{
			std::cout << "llen = " << ret2 << std::endl;
		}

		std::cout << "linsert..............." << std::endl << std::endl;
		if (!con.linsert("Ltest", RedisCpp::BEFORE, "value0", "chenjun", ret))
		{
			std::cout << "linsert error: " << con.getErrorStr() << std::endl;
		}
		else
		{
			std::cout << "revalue = " << ret << std::endl;
		}




		std::cout << "lrange..............." << std::endl << std::endl;
		if (!con.lrange("Ltest", 0, 2, valueList))
		{
			std::cout << "error " << con.getErrorStr() << std::endl;
		}
		else
		{
			RedisCpp::ValueList::const_iterator it = valueList.begin();

			for (; it != valueList.end(); it++)
			{
				std::cout << "value: " << *it << std::endl;
			}
		}


		std::cout << "lindex..............." << std::endl << std::endl;
		if (!con.lindex("Ltest", 1, value))
		{
			std::cout << "lindex error: " << con.getErrorStr() << std::endl;
		}
		else
		{
			std::cout << value.c_str() << std::endl;
		}

		//sleep(1);
	}
}

void MyTest()
{
	CRedisConn con;
	RedisCpp::ValueMap::iterator it;
	RedisCpp::ValueMap valueMap;
	RedisCpp::ValueList valueList;
	std::string value;

	int64_t ret = 0;
	uint64_t ret2 = 0;
	uint32_t ret3;

	while (1)
	{
		if (!con.connect("127.0.0.1", 6379, "", 0))
		{
			std::cout << "connect error: " << con.getErrorStr() << std::endl;
			Sleep(1);
			continue;
		}
		break;
	}
	while (1)
	{
		std::cout << "...." << std::endl;
		//sleep(5);
		if (!con.auth(PASSWORD))
		{
			std::cout << "auth error: " << con.getErrorStr() << std::endl;
			//sleep(10);
			continue;
		}
		else
		{
			std::cout << "auth success" << std::endl;
			break;

		}
	}

	for (int i = 0; i < 100; i++)
	{
		if (!con.set("test", "result", ret3))
		{
			std::cout << "set error: " << con.getErrorStr() << std::endl;
		}
		else
		{
			if (ret3 == 1)
			{
				std::cout << " ret = 1; set a new value " << std::endl;
			}
			else if (ret3 == 0)
			{
				std::cout << "ret = 0; recovered value " << std::endl;
			}

		}

		std::string str;

		if (!con.get("test", str))
		{
			std::cout << "get error: " << con.getErrorStr() << std::endl;
		}
		else
		{
			std::cout << "get value: " << str << std::endl;
		}
	}
}

int main()
{
	// TestList();
	// TestHash();
	// Test();

	MyTest();
	std::cout << "done main" << std::endl;
	return 0;
}