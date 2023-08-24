# Overlapped IO 모델
## IO의 중첩
 * IO(입출력)의 중첩 : 데이터의 전송/수신을 현 시간을 기준으로 둘 이상을 전송/수신하고 있는 상태
 * Blocking mode에서는 IO를 중첩 시킬 수 없음
 * Non-Blocking mode에서는 IO 함수를 바로 반환시키므로 바로 IO 함수를 호출할 수 있음
 * 즉, 동시에 진행시키는 상황을 IO의 중첩이라고 함
 * IO 송신의 성능에 가장 큰 영향을 주는 것은 대역폭(인터넷 환경)에 
 관련있음
 * 즉, IO의 중첩은 성능에 큰 영향을 주지 않음, 큰 용량의 전송에서 효율적 
 * 단점 : Non-Blocking mode는 별도의 확인 과정이 필요함
----
## 비동기 IO
 - Non-Blocking mode의 IO를 뜻함
 - IO가 비동기 방식으로 동작해야 IO를 중첩시킬 수 있음
 - 윈도우 Overlapped IO는 IO를 중첩시키는 입출력 모델임
----
## 윈도우 Overlapped IO
 - Overlapped IO가 아니더라도 IO를 중첩시킬 수 있음
 - Overlapped IO의 포커스는 IO가 아닌, "입출력의 완료 확인방법"에 있음
 - 비동기 IO를 진행시킬 때는 IO를 중첩시킬 수 있기 때문에 시간이 지나고 꼭 확인하여야 한다.
 - Window의 Overlapped IO를 활용하면 이 작업을 체계화해서 편하게 할 수 있음
----

