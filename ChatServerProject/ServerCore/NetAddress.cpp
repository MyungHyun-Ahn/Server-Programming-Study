#include "pch.h"
#include "NetAddress.h"

NetAddress::NetAddress(SOCKADDR_IN sockAddr) : mSockAddr(sockAddr)
{
}

NetAddress::NetAddress(wstring ip, uint16 port)
{
	::memset(&mSockAddr, 0, sizeof(mSockAddr));
	mSockAddr.sin_family = AF_INET;
	mSockAddr.sin_addr = Ip2Address(ip.c_str());
	mSockAddr.sin_port = ::htons(port);
}

wstring NetAddress::GetIpAddress()
{
	WCHAR buffer[100];
	::InetNtopW(AF_INET, &mSockAddr.sin_addr, buffer, len32(buffer));
	return wstring(buffer);
}

IN_ADDR NetAddress::Ip2Address(const WCHAR* ip)
{
	IN_ADDR address;
	::InetPtonW(AF_INET, ip, &address);
	return address;
}