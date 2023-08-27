#include "pch.h"
#include "EchoServer.h"

const UINT16 SERVER_PORT = 11021;
const UINT16 MAX_CLIENT = 100;		//총 접속할수 있는 클라이언트 수

int main()
{
	EchoServer server;
	server.InitSocket();
	server.BindandListen(SERVER_PORT);
	server.StartServer(MAX_CLIENT);

	printf("아무 키나 누를 때까지 대기합니다\n");
	getchar();

	server.DestroyThread();
	return 0;
}