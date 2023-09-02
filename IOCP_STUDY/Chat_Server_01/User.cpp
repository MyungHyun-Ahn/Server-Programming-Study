#include "pch.h"
#include "User.h"

void User::Init(const INT32 index)
{
}

void User::Clear()
{
}

int User::SetLogin(char* userID)
{
    return 0;
}

void User::EnterRoom(INT32 roomIndex)
{
}

INT32 User::GetCurrentRoom()
{
    return INT32();
}

INT32 User::GetNetConnIdx()
{
    return INT32();
}

INT32 User::GetUserId() const
{
    return INT32();
}

DOMAIN_STATE User::GetDomainState()
{
    return DOMAIN_STATE();
}

PacketInfo User::GetPacket()
{
    return PacketInfo();
}

void User::SetPacketData(const UINT32 dataSize, char* pData)
{
}
