# IOCP 7단계 실습 - 비동기 Accept 사용하기
## 비동기 Accept
 - Accept를 비동기 I/O로 사용하면 모든 것이 비동기 I/O가 된다.
 - 지금까지는 동기 I/O로 Accept를 처리하고 있었음
 - 클라이언트 접속 요청이 들어오지 않으면 Blocking 상태가 됨

## 구현 방법
 - Accept 쓰레드에서 루프를 돌며 시간을 체크
 - 연결되어 있는 것은 continue
 - 연결이 끊어진 시간을 비교
 - 일정시간 이상 끊어진 객체에 Accept를 예약

 * 끊어진지 얼마 안된 클라이언트에 연결하면 문제가 발생할 수 있음

 - Accept가 완료되면 WokerThread에서 완료처리
