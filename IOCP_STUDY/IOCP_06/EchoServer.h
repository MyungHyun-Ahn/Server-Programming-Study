#pragma once
#include "IOCPServer.h"

class EchoServer : public IOCPServer
{
public:
	EchoServer() {}
	virtual ~EchoServer() = default;

	virtual void OnConnect(const UINT32 clientIdx) override
	{
		printf("[OnConnect] 클라이언트 접속 : Index(%d) \n", clientIdx);
	}

	virtual void OnClose(const UINT32 clientIdx) override
	{
		printf("[OnClose] 클라이언트 종료 : Index(%d) \n", clientIdx);
	}

	virtual void OnReceive(const UINT32 clientIdx, const UINT32 dataSize, char* data) override
	{
		data[dataSize] = '\0';
		printf("[OnReceive] 클라이언트 : Index(%d) : 데이터 수신 %d bytes, 수신된 데이터 : %s \n", clientIdx, dataSize, (data + 5));
		
		PacketData packetData(clientIdx, dataSize, data);

		// 데이터 push 할 때 lock
		lock_guard<mutex> guard(mLock);
		mPacketDataQueue.push_back(packetData);
	}

	void Run(UINT32 maxClient)
	{
		mIsRunProcessThread = true;
		mProcessThread = thread([this]() { ProcessPacket(); });

		StartServer(maxClient);
	}

	void End()
	{
		mIsRunProcessThread = false;

		if (mProcessThread.joinable())
			mProcessThread.join();

		DestroyThread();
	}

private:
	void ProcessPacket()
	{
		while (mIsRunProcessThread)
		{
			PacketData packetData = DequePacketData();
			if (packetData.mDataSize != 0)
			{
				// SEND
				SendMsg(packetData.mClientIndex, packetData.mDataSize, packetData.pPacketData);
			}
			else
			{
				this_thread::sleep_for(chrono::microseconds(1));
			}
		}
	}

	PacketData DequePacketData()
	{
		// 데이터 꺼내서 사용할 때 lock
		lock_guard<mutex> guard(mLock);
		PacketData packetData;

		if (mPacketDataQueue.empty() == false)
		{
			packetData = mPacketDataQueue.front();
			mPacketDataQueue.front().Release();
			mPacketDataQueue.pop_front();

			return packetData;
		}
		return PacketData();
	}




private:
	bool mIsRunProcessThread = false;

	thread mProcessThread;

	mutex mLock;
	deque<PacketData> mPacketDataQueue;
};