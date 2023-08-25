/*
CreateIoCompletionPort �Լ��� ���
1. CP Ŀ�� ������Ʈ�� ����
	HANDLE hCpObject;
	hCpObject = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 2);
		* 4��° ���ڰ� �߿� : CP ������Ʈ�� IO ���Ŀ� �۾��� �������� ������ ����
2. Ŀ�� ������Ʈ�� ��� ������ ���
	CreateIoCompletionPort((HANDLE)hSock, hCpObject, (DWORD)ioInfo, 0);
		* hCpObject(CP ������Ʈ)�� hSock(����)�� ���

Completion Port�� �Ϸ�� IO Ȯ��
	BOOL GetQueuedCompletionStatus(HANDLE CompletionPort, LPDWORD lpNumberOfBytes, PULONG_PTR lpCompletionKey, LPOVERLAPPED* lpOverlapped, DWORD dwMiliseconds);
		* CompletionPort		: CP ������Ʈ�� �ڵ� ����
		* lpNumberOfBytes		: �ۼ��� �� �������� ũ�������� ������ ������ �ּ� �� ����
		* lpCompletionKey		: ���޵� ���� ������ ���� ������ �ּ� �� ����
		* lpOverlapped			: OVERLAPPED ����ü ����
		* dwMiliseconds			: Ÿ�Ӿƿ� ������ ����

CompletionPort�� �Ҵ�Ǵ� �������� �ǹ�
	* CP ������Ʈ�� �������� �ڵ����� �����尡 ���������� ����
	* �������� main �Լ��� �۾��� ����
	* CP ������Ʈ�� �Ҵ簡�� �� �������� ���� 2���� �����ϴ� ���� GetQueuedCompletionStatus �Լ��� ���ÿ� ȣ���� �� �ִ� �������� ���� 2���� �����Ѵٴ� ��
	* �ᱹ 2���� �����常 CP ������Ʈ���� ���� �� �ִٴ� ��
*/