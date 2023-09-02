#pragma once
class PacketManager
{
public:
	PacketManager() = default;
	~PacketManager() = default;

	void Init(const UINT32 maxClient);
	bool Run();
	void End();

	void ReceivePacketData(const UINT32 clientIndex, const UINT32 size, char* pData);
	void PushSystemPacket(PacketInfo packet);

	std::function<void(UINT32, UINT32, char*)> SendPacketFunc;

private:
	void CreateComponent(const UINT32 maxClient);
	void ClearConnectionInfo(INT32 clientIndex);

	void EnqueuePacketData(const UINT32 clientIndex);
	PacketInfo DequePacketData();
	PacketInfo DequeSystemPacketData();

	void ProcessPacket();
	void ProcessRecvPacket(const UINT32 clientIndex, const UINT16 packetId, const UINT16 packetSize, char* pPacket);
	void ProcessUserConnect(UINT32 clientIndex, UINT16 packetSize, char* pPacket);
	void ProcessUserDisConnect(UINT32 clientIndex, UINT16 packetSize, char* pPacket);
	void ProcessLogin(UINT32 clientIndex, UINT16 packetSize, char* pPacket);

private:
	typedef void(PacketManager::* PROCESS_RECV_PACKET_FUNCTION)(UINT32, UINT16, char*);
	std::unordered_map<int, PROCESS_RECV_PACKET_FUNCTION> _recvFuntionDictionary;

	std::function<void(int, char*)> _sendMQDataFunc;
	bool _isRunProcessThread = false;
	std::thread _processThread;
	std::mutex _lock;
	std::deque<UINT32> _inComingPacketUserIndex;
	std::deque<PacketInfo> _systemPacketQueue;
};

