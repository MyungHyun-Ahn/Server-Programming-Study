# Asynchronous Notification IO 모델
## 비동기?
* 일치하지 않는다는 것을 뜻함
* 비동기 입출력이란 입출력 함수의 반환시점과 데이터 송수신의 완료시점이 일치하지 않는 경우를 뜻함
----
## Notification IO : 알림 입출력
 * 입력버퍼에서 데이터가 수신되어서 데이터의 수신이 필요하거나, 출력버퍼가 비어서 데이터의 전송이 가능한 상황의 알림
 * 즉, 특정상황이 발생했음을 알리는 것
 * 대표적인 Notification IO 모델은 select 방식
 * WSAEventSelect : select 함수의 비동기 버전에 해당
 