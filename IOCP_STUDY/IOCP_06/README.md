# IOCP 6단계 실습 - 1-Send 구현하기
## Queue에 담아놓고 순차적으로 보내기
### 구현방법
 * Send를 호출하면 일단 Queue에 담는다.
 * Send를 할 때 Queue에 있는 데이터를 확인한다.
 * 1개가 있으면 바로 WSASend를 보냄
 * 2개 이상이면 앞의 Send가 아직 끝나지 않은 것이다.
 * 1보다 크면 일단 Queue에 냅둠
 * GQCS : GetQueueCompletionStatus : 만약 Send라면 1개를 Queue에서 꺼내주고 Send 진행

 - 좀 더 가벼운 Lock을 사용하도록 생각하기.