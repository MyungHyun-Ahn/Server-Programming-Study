#include "pch.h"
#include "EchoServer.h"

const UINT16 SERVER_PORT = 11021;
const UINT16 MAX_CLIENT = 100;		//�� �����Ҽ� �ִ� Ŭ���̾�Ʈ ��

int main()
{
	EchoServer server;
	server.InitSocket();
	server.BindandListen(SERVER_PORT);
	server.StartServer(MAX_CLIENT);

	printf("�ƹ� Ű�� ���� ������ ����մϴ�\n");
	getchar();

	server.DestroyThread();
	return 0;
}