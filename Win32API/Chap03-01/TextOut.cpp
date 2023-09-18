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
DC(Device Context)��?
��������� 3���� ���� ���� ���̺귯���� ����
 - Kernel - �޸� ����, ���α׷� ����
 - User - ���� �������̽��� �����츦 ����
 - GDI(Graphic Device Interface) - ȭ�鿡 ��µǴ� ��� ���ڿ� �׸��� �̰��� ��ģ��!

DC�� ��¿� �ʿ��� ��� ������ ������ ������ ����ü GDI ����� ����
�׸��� ���� �Լ����� ���� �μ��� �ʿ� - ��ǥ ���� ��� �� ���...
������ �μ��� �ѱ�⺸�ٴ� DC�� ��� ��Ƶΰ� DC�� �ڵ��� �޾� ���ڷ� �ѱ�� �׸��� �Լ��� �ξ� �������� �� �ִ�.

�׸���(hDC, X, Y)

ȭ�� ���� ���������� �ʱ� ���� ������ ���� ������ ������ �ִ�.
�ٸ� ���α׷��� â�� ������� �� �׸��� ���ƾ� �ϴ� ������ �����Ѵ�.
DC�� ��, �׸��⿡ �ʿ��� ���� ������ ���� ��ġ ������ �Ѵ�.

*/

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	// �簢 ����
	RECT rt = { 100, 100, 400, 300 };
	LPCTSTR str = TEXT("���ع��� ��λ���"
	"������ �⵵��"
	"�ϴ����� �����ϻ�"
	"�츮���� ����");

	switch (iMessage)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;


	// WM_PAINT 
	case WM_PAINT:
		// hdc = GetDC(hWnd); // DC�� ȹ��
		// BOOL TextOut(HDC hdc, int nXStart, int nYStart, LPCTSTR lpString, int cbString);
		// - hdc : DC �ڵ�
		// - x, y ��ǥ
		// - ���ڿ�
		// - ���ڿ� ����

		// ���� ����
		// UINT SetTextAlign(HDC hdc, UINT fMode);
		// � �������� ��ġ���� ������ �� �ִ�.
		//SetTextAlign(hdc, TA_CENTER);
		//TextOut(hdc, 200, 60, TEXT("Beautiful Korea"), lstrlen(TEXT("Beautiful Korea"))); // ���
		//TextOut(hdc, 200, 80, TEXT("is My"), lstrlen(TEXT("is My")));
		//TextOut(hdc, 200, 100, TEXT("Lovely Home Country"), lstrlen(TEXT("Lovely Home Country")));
		//ReleaseDC(hWnd, hdc); // �Ҵ� ���� - DC�� �޸𸮸� �����ϹǷ� �׻� �Ҵ� ���� ���־�� �Ѵ�.

		/*
		end - begin ������ε� �����Ҵ� ������ �� �� ����
		begin���� ������ dc�� end�θ� ���� ����
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, 100, 300, TEXT("End-Begin Test"), lstrlen(TEXT("End-Begin Test")));
		EndPaint(hWnd, &ps);
		*/

		// �簢 ���� �����Ͽ� ���� �� ���
		// int DrawText(HDC hDC, LPCTSTR lpString, int nCount, LPRECT lpRect, UINT uFormat);

		hdc = BeginPaint(hWnd, &ps);
		DrawText(hdc, str, -1, &rt, DT_CENTER | DT_WORDBREAK);
		EndPaint(hWnd, &ps);

		return 0;
	}

	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}