#include "pch.h"
#include "User.h"

void User::Init(const INT32 index)
{
    _index = index;
    _packetDataBuffer = new char[PACKET_DATA_BUFFER_SIZE];
}

void User::Clear()
{
    _roomIndex = -1;
    _userID = "";
    _isConfirm = false;
    _curDomainState = DOMAIN_STATE::NONE;

    _packetDataBufferWPos = 0;
    _packetDataBufferRPos = 0;
}

int User::SetLogin(char* userID)
{
    _curDomainState = DOMAIN_STATE::LOGIN;
    _userID = userID;

    return 0;
}

void User::EnterRoom(INT32 roomIndex)
{
    _roomIndex = roomIndex;
    _curDomainState = DOMAIN_STATE::ROOM;
}

INT32 User::GetCurrentRoom()
{
    return _roomIndex;
}

INT32 User::GetNetConnIdx()
{
    return _index;
}

std::string User::GetUserId() const
{
    return _userID;
}

DOMAIN_STATE User::GetDomainState()
{
    return _curDomainState;
}

PacketInfo User::GetPacket()
{
	const int PACKET_SIZE_LENGTH = 2;
	const int PACKET_TYPE_LENGTH = 2;
	short packetSize = 0;

	UINT32 remainByte = _packetDataBufferWPos - _packetDataBufferRPos;

	if (remainByte < PACKET_HEADER_LENGTH)
	{
		return PacketInfo();
	}

	auto pHeader = (PACKET_HEADER*)&_packetDataBuffer[_packetDataBufferRPos];

	if (pHeader->PacketLength > remainByte)
	{
		return PacketInfo();
	}

	PacketInfo packetInfo;
	packetInfo.PacketId = pHeader->PacketId;
	packetInfo.DataSize = pHeader->PacketLength;
	packetInfo.pDataPtr = &_packetDataBuffer[_packetDataBufferRPos];

	_packetDataBufferRPos += pHeader->PacketLength;

	return packetInfo;
}

void User::SetPacketData(const UINT32 dataSize, char* pData)
{
	if ((_packetDataBufferWPos + dataSize) >= PACKET_DATA_BUFFER_SIZE)
	{
		auto remainDataSize = _packetDataBufferWPos - _packetDataBufferRPos;

		if (remainDataSize > 0)
		{
			CopyMemory(&_packetDataBuffer[0], &_packetDataBuffer[_packetDataBufferRPos], remainDataSize);
			_packetDataBufferWPos = remainDataSize;
		}
		else
		{
			_packetDataBufferWPos = 0;
		}

		_packetDataBufferRPos = 0;
	}

	CopyMemory(&_packetDataBuffer[_packetDataBufferWPos], pData, dataSize);
	_packetDataBufferWPos += dataSize;
}
