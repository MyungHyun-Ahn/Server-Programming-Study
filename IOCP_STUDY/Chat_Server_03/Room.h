#pragma once

class UserManager;
class Packet;

class Room
{
public:
	Room() = default;
	~Room() = default;

	INT32		GetMaxUserCount() { return _maxUserCount; }
	INT32		GetCurrentUserCount() { return _currentUserCount; }
	INT32		GetRoomNumber() { return _roomNum; }

	void		Init(const INT32 roomNum, const INT32 maxUserCount);
	UINT32		EnterUser(User* user);
	void		LeaveUser(User* leaveUser);
	void		NotifyChat(INT32 clinetIndex, const char* userId, const char* msg);

	std::function<void(UINT32, UINT32, char*)> SendPacketFunc;

private:
	void SendToAllUser(const UINT16 dataSize, char* data, const INT32 passUserIndex, bool exceptMe);

private:
	INT32					_roomNum = -1;
	std::list<User*>		_userList;
	INT32					_maxUserCount = 0;
	UINT16					_currentUserCount = 0;

};

