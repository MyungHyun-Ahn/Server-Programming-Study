#pragma once
#include "pch.h"

class PacketData
{
public:
	PacketData() {}
	PacketData(UINT32 clientIndex, UINT32 dataSize, char* packetData) : mClientIndex(clientIndex), mDataSize(dataSize)
	{
		pPacketData = new char[dataSize];
		CopyMemory(pPacketData, packetData, dataSize);
	}

	PacketData& operator=(PacketData& packetData)
	{
		Set(packetData);
		return *this;
	}

	void Set(UINT32 clientIndex, UINT32 dataSize, char* packetData)
	{
		mClientIndex = clientIndex;
		mDataSize = dataSize;
		pPacketData = new char[dataSize];
		CopyMemory(pPacketData, packetData, dataSize);
	}

	void Set(PacketData& packet)
	{
		mClientIndex = packet.mClientIndex;
		mDataSize = packet.mDataSize;
		pPacketData = new char[mDataSize];
		CopyMemory(pPacketData, packet.pPacketData, mDataSize);
	}

	void Release()
	{
		delete pPacketData;
	}


public:
	UINT32 mClientIndex = 0; // 클라이언트 번호
	UINT32 mDataSize = 0; // 데이터 크기
	char* pPacketData = nullptr; // 데이터

};