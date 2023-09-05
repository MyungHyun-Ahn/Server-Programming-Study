# IOCP 8단계 실습 - 채팅서버 만들기 2
## Redis
 * 로그인할 때 Redis를 사용
 * 일반적으로는 RDBMS 사용
 * Redis는 메모리 DB
 * RDBMS와는 좀 다르지만 흐름은 거의 같다.
 * 연동할 때 사용하는 라이브러리는 당연히 다르다.
 * 웹서버와 소켓서버를 연계하는 경우에는 Redis를 많이 사용한다.

## hiredis
 - redis를 C++에서 사용할 수 있게 도와주는 라이브러리
 - hiredis를 설치하고 라이브러리를 프로젝트에 추가

## 설계
 - 기본적으로 이전 단계에서 진행한 패킷의 송수신과 비슷하게 동작
 - 로그인 요청이 들어오면 Redis Request Task를 만든다.
 - 이것을 Redis Manager 객체의 Task 큐에 전달
 - Redis 스레드에서 하나 씩 꺼내서 처리한 결과를 Redis Manager 객체의 완료 큐에 넣는다.
 - Packet Thread의 패킷 큐에서 반복문 돌며 패킷을 꺼내와서 처리하는 것과 마찬가지로
 - Redis Response 큐에서 꺼내서 로그인 완료 처리 후
 - 클라이언트에 로그인 완료를 전송한다.