#include "pch.h"
#include "Room.h"

void Room::Init(const INT32 roomNum, const INT32 maxUserCount)
{
}

UINT32 Room::EnterUser(User* user)
{
    return UINT32();
}

void Room::LeaveUser(User* leaveUser)
{
}

void Room::NotifyChat(INT32 clinetIndex, const char* userId, const char* msg)
{
}

void Room::SendToAllUser(const UINT16 dataSize, char* data, const INT32 passUserIndex, bool exceptMe)
{
}
