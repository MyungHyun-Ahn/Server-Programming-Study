#pragma once

class Room;

class RoomManager
{
public:
	RoomManager() = default;
	~RoomManager() = default;

	void Init(const INT32 beginRoomNumber, const INT32 maxRoomCount, const INT32 maxRoomUserCount);
	UINT32 GetMaxRoomCount() { return _maxRoomCount; }

	UINT32 EnterUser(INT32 roomNumber, User* user);
	INT16 LeaveUser(INT32 roomNumber, User* user);
	Room* GetRoomByNumber(INT32 number);

	std::function<void(UINT32, UINT16, char*)> SendPacketFunc;

private:
	std::vector<Room*> _roomList;
	INT32 _beginRoomNumber = 0;
	INT32 _endRoomNumber = 0;
	INT32 _maxRoomCount = 0;
};

