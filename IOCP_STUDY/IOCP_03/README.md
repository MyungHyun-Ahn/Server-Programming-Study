# IOCP 3단계 실습 네트워크 부분, 서버 로직 코드 분리
## 네트워크 부분
### 파일 구조
 * ServerNetwork 필터 생성
 * 하위 파일로 Define.h, IOCPServer.h (네트워크 부분)

## 서버 로직 부분
### 파일 구조
 * EchoServer.h

## 연결, 끊어짐, 데이터 받음을 애플리케이션에 전달
### IOCPServer.h에서 virtual 함수 만들기
 * EchoServer.h에서 이를 상속받아 콘솔에 출력하도록 구현
 * IOCPServer.h에서 각각 필요한 부분에서 함수 호출
