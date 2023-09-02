#pragma once

// ���̺귯��
#pragma comment(lib, "ws2_32")
#pragma comment(lib, "mswsock.lib")

// ������
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <MSWSock.h>
#include <windows.h>

// C++
#include <iostream>
#include <functional>
#include <string>

// ������ ����
#include <mutex>
#include <thread>
#include <chrono>

// std �ڷᱸ��
#include <vector>
#include <deque>
#include <queue>
#include <unordered_map>

// Ŀ����
#include "IOCPServer.h"
#include "ChatServer.h"
#include "ClientInfo.h"
#include "Define.h"
#include "ErrorCode.h"
#include "PacketManager.h"
#include "Packet.h"
#include "User.h"
#include "UserManager.h"

using namespace std;