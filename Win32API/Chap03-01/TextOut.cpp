#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hlnst;

LPCTSTR lpszClass = TEXT("TextOut");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;

	g_hlnst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW); 
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	return (int)Message.wParam;
}

/*
DC(Device Context)란?
윈도우즈는 3가지 동적 연결 라이브러리로 구성
 - Kernel - 메모리 관리, 프로그램 실행
 - User - 유저 인터페이스와 윈도우를 관리
 - GDI(Graphic Device Interface) - 화면에 출력되는 모든 글자와 그림은 이것을 거친다!

DC는 출력에 필요한 모든 정보를 가지는 데이터 구조체 GDI 모듈이 관리
그리기 위한 함수에는 많은 인수가 필요 - 좌표 넓이 모양 색 등등...
일일이 인수를 넘기기보다는 DC에 모두 모아두고 DC의 핸들을 받아 인자로 넘기면 그리기 함수가 훨씬 간단해질 수 있다.

그리기(hDC, X, Y)

화면 밖을 빠져나가지 않기 위해 원점에 대한 정보도 가지고 있다.
다른 프로그램이 창을 덮어씌웠을 때 그리지 말아야 하는 영역을 관리한다.
DC는 즉, 그리기에 필요한 여러 정보와 안전 장치 역할을 한다.

*/

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	// 사각 영역
	RECT rt = { 100, 100, 400, 300 };
	LPCTSTR str = TEXT("동해물과 백두산이"
	"마르고 닳도록"
	"하느님이 보우하사"
	"우리나라 만세");

	switch (iMessage)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;


	// WM_PAINT 
	case WM_PAINT:
		// hdc = GetDC(hWnd); // DC를 획득
		// BOOL TextOut(HDC hdc, int nXStart, int nYStart, LPCTSTR lpString, int cbString);
		// - hdc : DC 핸들
		// - x, y 좌표
		// - 문자열
		// - 문자열 길이

		// 정렬 가능
		// UINT SetTextAlign(HDC hdc, UINT fMode);
		// 어떤 기준으로 배치할지 선택할 수 있다.
		//SetTextAlign(hdc, TA_CENTER);
		//TextOut(hdc, 200, 60, TEXT("Beautiful Korea"), lstrlen(TEXT("Beautiful Korea"))); // 출력
		//TextOut(hdc, 200, 80, TEXT("is My"), lstrlen(TEXT("is My")));
		//TextOut(hdc, 200, 100, TEXT("Lovely Home Country"), lstrlen(TEXT("Lovely Home Country")));
		//ReleaseDC(hWnd, hdc); // 할당 해제 - DC도 메모리를 차지하므로 항상 할당 해제 해주어야 한다.

		/*
		end - begin 방식으로도 동적할당 해제를 할 수 있음
		begin으로 선언한 dc는 end로만 해제 가능
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, 100, 300, TEXT("End-Begin Test"), lstrlen(TEXT("End-Begin Test")));
		EndPaint(hWnd, &ps);
		*/

		// 사각 영역 지정하여 여러 줄 출력
		// int DrawText(HDC hDC, LPCTSTR lpString, int nCount, LPRECT lpRect, UINT uFormat);

		hdc = BeginPaint(hWnd, &ps);
		DrawText(hdc, str, -1, &rt, DT_CENTER | DT_WORDBREAK);
		EndPaint(hWnd, &ps);

		return 0;
	}

	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}