#pragma once

// 라이브러리
#pragma comment(lib, "ws2_32")
#pragma comment(lib, "mswsock.lib")

// 윈도우
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <MSWSock.h>
#include <windows.h>

// C++
#include <iostream>
#include <functional>
#include <string>

// 쓰레드 관련
#include <mutex>
#include <thread>
#include <chrono>

// std 자료구조
#include <vector>
#include <deque>
#include <queue>
#include <unordered_map>

// 커스텀
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