/*
CreateIoCompletionPort 함수의 기능
1. CP 커널 오브젝트를 생성
	HANDLE hCpObject;
	hCpObject = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 2);
		* 4번째 인자가 중요 : CP 오브젝트에 IO 이후에 작업할 쓰레드의 개수를 전달
2. 커널 오브젝트에 대상 소켓을 등록
	CreateIoCompletionPort((HANDLE)hSock, hCpObject, (DWORD)ioInfo, 0);
		* hCpObject(CP 오브젝트)에 hSock(소켓)을 등록

Completion Port의 완료된 IO 확인
	BOOL GetQueuedCompletionStatus(HANDLE CompletionPort, LPDWORD lpNumberOfBytes, PULONG_PTR lpCompletionKey, LPOVERLAPPED* lpOverlapped, DWORD dwMiliseconds);
		* CompletionPort		: CP 오브젝트의 핸들 전달
		* lpNumberOfBytes		: 송수신 된 데이터의 크기정보를 저장할 변수의 주소 값 전달
		* lpCompletionKey		: 전달된 값의 저장을 위한 변수의 주소 값 전달
		* lpOverlapped			: OVERLAPPED 구조체 전달
		* dwMiliseconds			: 타임아웃 정보를 전달

CompletionPort에 할당되는 쓰레드의 의미
	* CP 오브젝트의 생성에서 자동으로 쓰레드가 생성되지는 않음
	* 쓰레드의 main 함수가 작업을 수행
	* CP 오브젝트에 할당가능 한 쓰레드의 수를 2개로 제한하는 것은 GetQueuedCompletionStatus 함수를 동시에 호출할 수 있는 쓰레드의 수를 2개로 제한한다는 것
	* 결국 2개의 쓰레드만 CP 오브젝트에서 일할 수 있다는 것
*/