#include "pch.h"
#include "RoomManager.h"
#include "Room.h"

void RoomManager::Init(const INT32 beginRoomNumber, const INT32 maxRoomCount, const INT32 maxRoomUserCount)
{
	_beginRoomNumber = beginRoomNumber;
	_maxRoomCount = maxRoomCount;
	_endRoomNumber = beginRoomNumber + maxRoomCount;

	_roomList = std::vector<Room*>(maxRoomCount);

	for (auto i = 0; i < maxRoomCount; i++)
	{
		_roomList[i] = new Room();
		_roomList[i]->SendPacketFunc = SendPacketFunc;
		_roomList[i]->Init((i + beginRoomNumber), maxRoomUserCount);
	}
}

UINT32 RoomManager::EnterUser(INT32 roomNumber, User* user)
{
	auto pRoom = GetRoomByNumber(roomNumber);
	if (pRoom == nullptr)
	{
		return (UINT16)ERROR_CODE::ROOM_INVALID_INDEX;
	}


	return pRoom->EnterUser(user);
}

INT16 RoomManager::LeaveUser(INT32 roomNumber, User* user)
{
	auto pRoom = GetRoomByNumber(roomNumber);
	if (pRoom == nullptr)
	{
		return (INT16)ERROR_CODE::ROOM_INVALID_INDEX;
	}

	user->SetDomainState(DOMAIN_STATE::LOGIN);
	pRoom->LeaveUser(user);
	return (INT16)ERROR_CODE::NONE;
}

Room* RoomManager::GetRoomByNumber(INT32 number)
{
	if (number < _beginRoomNumber || number >= _endRoomNumber)
	{
		return nullptr;
	}

	auto index = (number - _beginRoomNumber);
	return _roomList[index];
}
