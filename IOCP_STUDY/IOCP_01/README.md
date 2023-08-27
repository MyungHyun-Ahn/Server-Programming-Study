# IOCP 1단계 실습 IOCP Echo 서버 만들기
## Echo 서버 프로그램 분석
### IOOperation enum class
 - RECV
 - SEND
 - IOCP 오브젝트에서 완료 결과를 통지받고 어떤 작업을 수행할지 구분하는 역할

### WSAOVERLAPPED 구조체 확장
 - WSAOVERLAPPED 구조체를 확장하여 필요한 정보를 더 받아줌
 - 클라이언트 소켓
 - Overlapped IO 작업 버퍼
 - 데이터 버퍼
 - IOOperation : 작업 종류

### stClientInfo 구조체
 - 클라이언트 정보를 담기 위함
 - 클라이언트와 연결되는 소켓
 - Recv, Send Overlapped 구조체
 - 생성자에서 Overlapped 구조체를 0으로 밀어주고, 클라이언트 소켓을 INVALID_SOCKET 설정

### IOCompletionPort 동작 분석
1. InitSocket : 소켓을 초기화
    * WSAStartup
    * WSASocket : TCP, Listen 소켓 생성, 마지막 인자 WSA_FLAG_OVERLAPPED
2. BindandListen : 서버용 함수로, 서버 주소 정보 연결, 접속 요청 받기위한 등록
    * SOCKADDR_IN 구조체
        - 서버 소켓 주소정보 등록
    * bind
    * listen
3. StartServer : 접속 요청 수락, 메시지 처리
    * CreateClient
    * IOCP 오브젝트 생성
        - CreateIoCompletionPort
    * WokerThread 활성화
        - IOCP 오브젝트에서 완료 정보를 받아 RECV, SEND 작업 수행
    * AccepterThread 활성화 
        - 클라이언트 접속을 수락
        - IOCP 오브젝트에 클라이언트 소켓 등록
        - 초기 작업으로 RECV 등록
