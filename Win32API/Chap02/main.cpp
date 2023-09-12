#include <windows.h>

/*
주석일치하지않음 경고 - 빌드 실패 해결법
프로젝트 설정 -> 링커 -> 하위 시스템 -> 콘솔에서 창으로 변경
*/

/*
간단하게 윈도우 창 하나를 띄우는 예제
*/

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hlnst;

// 윈도우 클래스를 정의하는데 사용
LPCTSTR lpszClass = TEXT("First");

/*
모든 윈도우즈 프로그램은 main 함수가 아닌 WinMain이다.
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow);
APIENTRY 지정자 : 윈도우즈의 호출 규약인 __stdcall 을 사용하겠다는 의미
 - hInstance : 프로그램의 인스턴스 핸들 * 이것 이외에는 잘 사용하지 않음
 - hPrevInstance : 바로 앞에 실행된 현재 프로그램의 인스턴스 핸들, Win32에서는 항상 NULL, 16비트 시절의 잔재이다.
 - lpszCmdParam : 프로그램 인수 argv와 같은 의미
 - nCmdShow : 프로그램이 실행될 형태, 최소화, 보통 모양 등이 전달


*/
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;

	// 윈도우 클래스 정의
	// 이것을 초기화하고 창을 띄움
	WNDCLASS WndClass;

	// 전역 변수에 대입
	// hInstance는 지역변수이므로 WinMain 밖에서는 사용할 수 없기 때문
	g_hlnst = hInstance;

	// cbClsExtra, cbWndExtra : 윈도우가 내부적으로 사용하는 예약 영역 사용하지 않을 경우 0
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // 배경색 설정
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW); // 마우스 커서 지장
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION); // 마우스 아이콘 지정
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc; // 윈도우 메시지 처리 함수 지정
	WndClass.lpszClassName = lpszClass; // 윈도우 클래스의 이름을 문자열로 지정
	WndClass.lpszMenuName = NULL; // 프로그램이 사용할 메뉴를 지정
	WndClass.style = CS_HREDRAW | CS_VREDRAW; // 스타일 정의
	RegisterClass(&WndClass); // 사용할 윈도우 클래스를 등록 -> 운영체제는 윈도우 클래스의 특성을 기억하게 됨

	// 실제 윈도우 생성
	/*
	HWND CreateWindow(lpszClassName, lpszWindowName, dwStyle, x, y, nWidth, nHeight, hwndParent, hmenu, hinst, lpvParam);
	윈도우를 생성하고 윈도우 핸들 반환
	*/
	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);
	
	// 윈도우를 화면에 출력
	ShowWindow(hWnd, nCmdShow);

	/*
	윈도우를 출력하는 과정
	 1. WndClass 정의 : 윈도우 기반 클래스 정의 -> 윈도우 속성 세팅
	 2. RegisterClass : 윈도우 클래스 등록
	 3. CreateWindow : 메모리 상에 윈도우 만들기
	 4. ShowWindow : 윈도우를 화면에 표시
	 5. 메시지 루프 : 메시지 처리
	*/


	/*
	메시지 루프
	BOOL GetMessage(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMinm UINT wMsgFilterMax);
	 - 메시지 큐에서 메시지를 읽어들임
	 - 메시지 큐? : 시스템, 사용자로부터 발생된 메시지가 잠시 대기하는 메시지 임시 저장 영역
	 - lpMsg : 읽어들인 메시지 저장
	 - 읽어들인 메시지가 WM_QUIT일 경우 FALSE -> 즉 WM_QUIT가 읽혀지면 프로그램 종료
	 - 나머지 인수는 메시지의 크기를 지정함 -> 잘 사용하지 않으므로 PASS

	BOOL TranslateMessage(CONST MSG* lpmsg);
	 - 키보드 입력 메시지를 가공하여 프로그램에서 쉽게 쓸 수 있도록 함
	 - WM_KEYDOWN 키보드 눌림 이벤트 -> WM_CHAR 문자가 입력되었다는 메시지를 만드는 역할

	LONG DispatchMessage(CONST MSG* lpmsg);
	 - 메시지 큐에서 꺼낸 메시지를 WndProc 함수로 전달
	*/

	/*
	메시지 구조체

	typedef struct tagMSG
	{
		HWND hwnd; // 메시지를 받을 윈도우 핸들
		UINT message; // 메시지의 종류 - 가장 중요
		WPARAM wParam; // 메시지에 대한 부가적인 정보
		LPARAM lParam; // 메시지에 대한 부가적인 정보
		DWORD time; // 메시지가 발생한 시간
		POINT pt; // 메시지가 발생한 마우스 위치
	} MSG;



	*/

	while (GetMessage(&Message, NULL, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	return (int)Message.wParam;
}

// 사용자와 시스템이 보내오는 메시지를 처리하는 역할
// Windows 에서는 WndProc 함수와 WinMain 함수 2개가 있어야 프로그램이 동작 가능

// CALLBACK __stdcall 호출규약 (APIENTRY, WINAPI)

/*
윈도우 프로시저
메시지가 발생할 때 프로그램의 반응을 처리하는 일을 수행
WinMain 내의 메시지 루프는 메시지를 메시지 처리 함수로 보내기만 하고 WndProc은 메시지가 입력되면 운영체제에 의해 호출되어 메시지를 처리
-> CALLBACK 함수

switch case 문으로 메시지를 구분하여 처리

마지막의 DefWindowProc 함수는 WndProc에서 처리하지 않은 나머지 메시지의 처리를 수행한다.
ex) 윈도우 이동 / 크기 변경, 프로그램 종료
*/

/*
메시지 루프와 메시지 처리 과정
사용자 조작 / 시스템 변화 / 메시지 입력 -> 메시지 큐 -> MSG 구조체

GetMessage 메시지 꺼냄 -> WM_QUIT - No -> TranslateMessage 키보드 해석 -> DispatchMessage 메시지 보냄 -> WndProc 메시지 처리
위 과정의 반복
*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}



