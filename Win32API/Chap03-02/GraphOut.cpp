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

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	switch (iMessage)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;


		// WM_PAINT 
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		
		// 10, 10 ��ǥ�� ������ �� ���
		SetPixel(hdc, 10, 10, RGB(255, 0, 0));

		// MoveToEx �Լ��� LineTo �Լ��� ��Ʈ

		// CP�� MoveToEx �Լ��� (A, B) ��ǥ�� �̵���Ų ��
		// LineTo �Լ��� ������ ��ǥ(C, D)�� ���� ����
		MoveToEx(hdc, 50, 50, NULL);
		LineTo(hdc, 300, 90);

		// �簢���� �׸� (top, left, bottom, right)
		Rectangle(hdc, 50, 100, 200, 180);

		// �簢���� �����ϴ� Ÿ���� �׸�
		// (x-r, y-r, x+r, x+r)
		Ellipse(hdc, 220, 100, 400, 200);
		EndPaint(hWnd, &ps);
		return 0;
	}

	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}