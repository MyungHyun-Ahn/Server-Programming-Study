#include "pch.h"
#include "PacketManager.h"
#include "UserManager.h"

void PacketManager::Init(const UINT32 maxClient)
{
	_recvFuntionDictionary = std::unordered_map<int, PROCESS_RECV_PACKET_FUNCTION>();

	_recvFuntionDictionary[(int)PACKET_ID::SYS_USER_CONNECT] = &PacketManager::ProcessUserConnect;
	_recvFuntionDictionary[(int)PACKET_ID::SYS_USER_DISCONNECT] = &PacketManager::ProcessUserDisConnect;
	_recvFuntionDictionary[(int)PACKET_ID::LOGIN_REQUEST] = &PacketManager::ProcessLogin;

	CreateComponent(maxClient);
}

bool PacketManager::Run()
{
	_isRunProcessThread = true;
	_processThread = std::thread([this]() { ProcessPacket(); });

	return true;
}

void PacketManager::End()
{
	_isRunProcessThread = false;

	if (_processThread.joinable())
	{
		_processThread.join();
	}
}

void PacketManager::ReceivePacketData(const UINT32 clientIndex, const UINT32 size, char* pData)
{
	auto pUser = _userManager->GetUserByConnIdx(clientIndex);
	pUser->SetPacketData(size, pData);

	EnqueuePacketData(clientIndex);
}

void PacketManager::PushSystemPacket(PacketInfo packet)
{
	std::lock_guard<std::mutex> guard(_lock);
	_systemPacketQueue.push_back(packet);
}

void PacketManager::CreateComponent(const UINT32 maxClient)
{
	_userManager = new UserManager;
	_userManager->Init(maxClient);
}

void PacketManager::ClearConnectionInfo(INT32 clientIndex)
{
	auto pReqUser = _userManager->GetUserByConnIdx(clientIndex);

	if (pReqUser->GetDomainState() != DOMAIN_STATE::NONE)
	{
		_userManager->DeleteUserInfo(pReqUser);
	}
}

void PacketManager::EnqueuePacketData(const UINT32 clientIndex)
{
	std::lock_guard<std::mutex> guard(_lock);
	_inComingPacketUserIndex.push_back(clientIndex);
}

PacketInfo PacketManager::DequePacketData()
{
	UINT32 userIndex = 0;

	{
		std::lock_guard<std::mutex> guard(_lock);
		if (_inComingPacketUserIndex.empty())
		{
			return PacketInfo();
		}

		userIndex = _inComingPacketUserIndex.front();
		_inComingPacketUserIndex.pop_front();
	}

	auto pUser = _userManager->GetUserByConnIdx(userIndex);
	auto packetData = pUser->GetPacket();
	packetData.ClientIndex = userIndex;
	return packetData;
}

PacketInfo PacketManager::DequeSystemPacketData()
{
	std::lock_guard<std::mutex> guard(_lock);
	if (_systemPacketQueue.empty())
	{
		return PacketInfo();
	}

	auto packetData = _systemPacketQueue.front();
	_systemPacketQueue.pop_front();

	return packetData;
}

void PacketManager::ProcessPacket()
{
	while (_isRunProcessThread)
	{
		bool isIdle = true;

		auto packetData = DequePacketData();
		if (packetData.PacketId > (UINT16)PACKET_ID::SYS_END)
		{
			isIdle = false;
			ProcessRecvPacket(packetData.ClientIndex, packetData.PacketId, packetData.DataSize, packetData.pDataPtr);
		}

		packetData = DequeSystemPacketData();
		if (packetData.PacketId != 0)
		{
			isIdle = false;
			ProcessRecvPacket(packetData.ClientIndex, packetData.PacketId, packetData.DataSize, packetData.pDataPtr);
		}

		if (isIdle)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
}

void PacketManager::ProcessRecvPacket(const UINT32 clientIndex, const UINT16 packetId, const UINT16 packetSize, char* pPacket)
{
	auto iter = _recvFuntionDictionary.find(packetId);
	if (iter != _recvFuntionDictionary.end())
	{
		(this->*(iter->second))(clientIndex, packetSize, pPacket);
	}
}

void PacketManager::ProcessUserConnect(UINT32 clientIndex, UINT16 packetSize, char* pPacket)
{
	printf("[ProcessUserConnect] clientIndex: %d\n", clientIndex);
	auto pUser = _userManager->GetUserByConnIdx(clientIndex);
	pUser->Clear();
}

void PacketManager::ProcessUserDisConnect(UINT32 clientIndex, UINT16 packetSize, char* pPacket)
{
	printf("[ProcessUserDisConnect] clientIndex: %d\n", clientIndex);
	ClearConnectionInfo(clientIndex);
}

void PacketManager::ProcessLogin(UINT32 clientIndex, UINT16 packetSize, char* pPacket)
{
	if (LOGIN_REQUEST_PACKET_SZIE != packetSize)
	{
		return;
	}

	auto pLoginReqPacket = reinterpret_cast<LOGIN_REQUEST_PACKET*>(pPacket);

	auto pUserID = pLoginReqPacket->UserID;
	printf("requested user id = %s\n", pUserID);

	LOGIN_RESPONSE_PACKET loginResPacket;
	loginResPacket.PacketId = (UINT16)PACKET_ID::LOGIN_RESPONSE;
	loginResPacket.PacketLength = sizeof(LOGIN_RESPONSE_PACKET);

	if (_userManager->GetCurrentUserCnt() >= _userManager->GetMaxUserCnt())
	{
		//접속자수가 최대수를 차지해서 접속불가
		loginResPacket.Result = (UINT16)ERROR_CODE::LOGIN_USER_USED_ALL_OBJ;
		SendPacketFunc(clientIndex, sizeof(LOGIN_RESPONSE_PACKET), (char*)&loginResPacket);
		return;
	}

	//여기에서 이미 접속된 유저인지 확인하고, 접속된 유저라면 실패한다.
	if (_userManager->FindUserIndexByID(pUserID) == -1)
	{
		loginResPacket.Result = (UINT16)ERROR_CODE::NONE;
		SendPacketFunc(clientIndex, sizeof(LOGIN_RESPONSE_PACKET), (char*)&loginResPacket);
	}
	else
	{
		//접속중인 유저여서 실패를 반환한다.
		loginResPacket.Result = (UINT16)ERROR_CODE::LOGIN_USER_ALREADY;
		SendPacketFunc(clientIndex, sizeof(LOGIN_RESPONSE_PACKET), (char*)&loginResPacket);
		return;
	}
}
