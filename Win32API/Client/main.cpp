// Client.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#include "pch.h"
#include <vector>

#include "framework.h"
#include "Client.h"
#include "CCore.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
HWND g_hWnd; // 메인 윈도우 핸들
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


// 윈도우 프로그램의 진입점
// 인자 앞에 붙는 매크로 SAL 주석 - 소스 코드 주석 언어 - 해당 변수의 용도를 알려주는 것
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, // 프로세스가 실행되는 메모리 주소
                     _In_opt_ HINSTANCE hPrevInstance, // optional
                     _In_ LPWSTR    lpCmdLine, // 받아온 명령어 문자열 - 게임 프로그래밍에서 딱히 쓰이진 않음
                     _In_ int       nCmdShow) // 
{
    // 메모리 누수 체크
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    // _CrtBreakAlloc();

    // 의미가 없는 즉, 쓰이지 않는다는 것을 알려주는 매크로
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.

    // 문자열 자체가 자원 - 리소스 뷰 String Table에 등록하고 ID 값을 줌
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CLIENT, szWindowClass, MAX_LOADSTRING);

    // 윈도우 정보 등록 / 생성
    // 프로세스 != 윈도우 : 윈도우는 인터페이스, 프로세스는 윈도우를 가질수도 아닐수도 있다.
    // ex) 백그라운드 프로세스
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    // Core 초기화
    if (FAILED(CCore::GetInstance()->init(g_hWnd, POINT{1280, 768})))
    {
        MessageBox(nullptr, L"Core 객체 초기화 실패", L"Error", MB_OK);
        return FALSE;
    }

    // 단축키 테이블 정보 로딩
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));

    MSG msg;

    // 3번째 인자 지연시간
    // SetTimer(g_hWnd, 0, 0, nullptr);


    // 기본 메시지 루프입니다:
    // 메시지 큐에서 꺼내서 처리 -> WndProc으로 보내서 처리
    // GetMessage 함수는 메시지 큐에서 메시지를 확인할 때까지 대기
    // GetMessage가 리턴되면 무조건 메시지를 받은 것
    // 메시지 반응 형태로 게임을 만들기에는 부적합
    // 기본 구조를 고쳐서 사용할 것
    // 윈도우 메시지를 처리하지 않고 게임을 만들것임

    // 메시지 기반으로 밖에 작동하지 않는 이유 GetMessage 함수 때문
    // PeekMessage를 사용 -> 메시지를 옅보다

    // PeekMessage는 함수가 바로 반환 - 비동기
    // 반환값의 의미 - 메시지가 있을 때와 없을 때

    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) // PM_REMOVE 옵션을 주면 GetMessage와 거의 비슷해진다
        {
			if (msg.message == WM_QUIT)
				break;

			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) // 단축키 정보가 발생했는지 검사
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
        }
        // 메시지가 발생하지 않는 대부분의 시간
        else // 메시지가 없어도
        {

            // Game 코드 수행
            // 디자인 패턴(설계 유형)
            // 싱글톤 패턴

            // Core 클래스 작업

            CCore::GetInstance()->progress();
        }
    }

    // KillTimer(g_hWnd, 0);
    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    // 윈도우 클래스의 구조체 멤버들을 채우고 있음
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CLIENT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    // RegisterClassExW 윈도우 제공 함수
    // 윈도우 클래스를 등록 - 어떤 기능을 할지 운영체제에 등록
    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   g_hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!g_hWnd)
   {
      return FALSE;
   }

   ShowWindow(g_hWnd, nCmdShow);
   UpdateWindow(g_hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
        // switch - case 내부에서 지역변수를 선언하고 싶을 때는 스코프를 지정해주어야 한다.
    case WM_PAINT:  // MSDN 레퍼런스 - 언제 발생하는지? - 무효화 영역이 발생한 경우
        {           // 무효화 영역? invalidate Rect : 윈도우 창이 있을 때 다른 창에 의해서 가려진 경우 - ? 호출 안됨 - 예전에는 그랬다. 지금은 그렇지 않다.
                    // 창을 완전히 줄였다가 켜면 발생한다. - 화면 전체에 대한 무효화 영역이 발생했다.
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps); // Device Context 만들어서 ID를 반환

            // Rectangle(hdc, 1180, 0, 1280, 100);

            EndPaint(hWnd, &ps);
        }
        break;

    case WM_KEYDOWN:
        {
        }
        break;

    case WM_LBUTTONDOWN:
    {
    }
        break;

    case WM_MOUSEMOVE:
    {
    }
        break;

    case WM_LBUTTONUP:
    {

    }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
