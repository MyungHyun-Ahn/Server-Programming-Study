#include "pch.h"
#include "Room.h"

void Room::Init(const INT32 roomNum, const INT32 maxUserCount)
{
    _roomNum = roomNum;
    _maxUserCount = maxUserCount;
}

UINT32 Room::EnterUser(User* user)
{
	if (_currentUserCount >= _maxUserCount)
	{
		return (UINT16)ERROR_CODE::ENTER_ROOM_FULL_USER;
	}

	_userList.push_back(user);
	++_currentUserCount;

	user->EnterRoom(_roomNum);
	return (UINT16)ERROR_CODE::NONE;
}

void Room::LeaveUser(User* leaveUser)
{
	// ���� ���� ���� ID�� ã�Ƽ� �װ� �´ٸ� leave
	_userList.remove_if([leaveUserId = leaveUser->GetUserId()](User* pUser) {
		return leaveUserId == pUser->GetUserId();
	});
}

void Room::NotifyChat(INT32 clinetIndex, const char* userId, const char* msg)
{
	ROOM_CHAT_NOTIFY_PACKET roomChatNtfyPkt;
	roomChatNtfyPkt.PacketId = (UINT16)PACKET_ID::ROOM_CHAT_NOTIFY;
	roomChatNtfyPkt.PacketLength = sizeof(roomChatNtfyPkt);

	CopyMemory(roomChatNtfyPkt.Msg, msg, sizeof(roomChatNtfyPkt.Msg));
	CopyMemory(roomChatNtfyPkt.UserID, userId, sizeof(roomChatNtfyPkt.UserID));
	SendToAllUser(sizeof(roomChatNtfyPkt), (char*)&roomChatNtfyPkt, clinetIndex, false);
}

void Room::SendToAllUser(const UINT16 dataSize, char* data, const INT32 passUserIndex, bool exceptMe)
{
	// exceptMe �ɼ� ���������� �ڽ��� �����ϰ� ��ε�ĳ��Ʈ ����
	// ������ �ڽ� ����
	for (auto pUser : _userList)
	{
		if (pUser == nullptr) {
			continue;
		}

		if (exceptMe && pUser->GetNetConnIdx() == passUserIndex) {
			continue;
		}

		SendPacketFunc((UINT32)pUser->GetNetConnIdx(), (UINT32)dataSize, data);
	}
}
