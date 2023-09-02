#pragma once

enum class DOMAIN_STATE
{
	NONE = 0,
	LOGIN = 1,
	ROOM = 2,
};

class User
{
public:
	const UINT32 PACKET_DATA_BUFFER_SIZE = 8096;

	User() = default;
	~User() = default;

	void Init(const INT32 index);
	void Clear();

	int SetLogin(char* userID);
	void EnterRoom(INT32 roomIndex);
	void SetDomainState(DOMAIN_STATE value) { }

	INT32 GetCurrentRoom();
	INT32 GetNetConnIdx();
	INT32 GetUserId() const;
	DOMAIN_STATE GetDomainState();
	PacketInfo GetPacket();

	void SetPacketData(const UINT32 dataSize, char* pData);

private:
	INT32 _index = -1;
	INT32 _roomIndex = -1;

	std::string _userID;
	bool _isConfirm = false;
	std::string _authToken;

	DOMAIN_STATE _curDomainState = DOMAIN_STATE::NONE;

	UINT32 _packetDataBufferWPos = 0;
	UINT32 _packetDataBufferRPos = 0;
	char* _packetDataBuffer = nullptr;
};

