#pragma once

#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>

#include <thread>
#include <vector>
#include <mutex>
#include <chrono>
#include <deque>

#include "Define.h"
#include "ClientInfo.h"
#include "Packet.h"

using namespace std;