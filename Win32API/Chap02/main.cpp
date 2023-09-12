#include <windows.h>

/*
�ּ���ġ�������� ��� - ���� ���� �ذ��
������Ʈ ���� -> ��Ŀ -> ���� �ý��� -> �ֿܼ��� â���� ����
*/

/*
�����ϰ� ������ â �ϳ��� ���� ����
*/

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hlnst;

// ������ Ŭ������ �����ϴµ� ���
LPCTSTR lpszClass = TEXT("First");

/*
��� �������� ���α׷��� main �Լ��� �ƴ� WinMain�̴�.
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow);
APIENTRY ������ : ���������� ȣ�� �Ծ��� __stdcall �� ����ϰڴٴ� �ǹ�
 - hInstance : ���α׷��� �ν��Ͻ� �ڵ� * �̰� �̿ܿ��� �� ������� ����
 - hPrevInstance : �ٷ� �տ� ����� ���� ���α׷��� �ν��Ͻ� �ڵ�, Win32������ �׻� NULL, 16��Ʈ ������ �����̴�.
 - lpszCmdParam : ���α׷� �μ� argv�� ���� �ǹ�
 - nCmdShow : ���α׷��� ����� ����, �ּ�ȭ, ���� ��� ���� ����


*/
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;

	// ������ Ŭ���� ����
	// �̰��� �ʱ�ȭ�ϰ� â�� ���
	WNDCLASS WndClass;

	// ���� ������ ����
	// hInstance�� ���������̹Ƿ� WinMain �ۿ����� ����� �� ���� ����
	g_hlnst = hInstance;

	// cbClsExtra, cbWndExtra : �����찡 ���������� ����ϴ� ���� ���� ������� ���� ��� 0
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // ���� ����
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW); // ���콺 Ŀ�� ����
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION); // ���콺 ������ ����
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc; // ������ �޽��� ó�� �Լ� ����
	WndClass.lpszClassName = lpszClass; // ������ Ŭ������ �̸��� ���ڿ��� ����
	WndClass.lpszMenuName = NULL; // ���α׷��� ����� �޴��� ����
	WndClass.style = CS_HREDRAW | CS_VREDRAW; // ��Ÿ�� ����
	RegisterClass(&WndClass); // ����� ������ Ŭ������ ��� -> �ü���� ������ Ŭ������ Ư���� ����ϰ� ��

	// ���� ������ ����
	/*
	HWND CreateWindow(lpszClassName, lpszWindowName, dwStyle, x, y, nWidth, nHeight, hwndParent, hmenu, hinst, lpvParam);
	�����츦 �����ϰ� ������ �ڵ� ��ȯ
	*/
	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);
	
	// �����츦 ȭ�鿡 ���
	ShowWindow(hWnd, nCmdShow);

	/*
	�����츦 ����ϴ� ����
	 1. WndClass ���� : ������ ��� Ŭ���� ���� -> ������ �Ӽ� ����
	 2. RegisterClass : ������ Ŭ���� ���
	 3. CreateWindow : �޸� �� ������ �����
	 4. ShowWindow : �����츦 ȭ�鿡 ǥ��
	 5. �޽��� ���� : �޽��� ó��
	*/


	/*
	�޽��� ����
	BOOL GetMessage(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMinm UINT wMsgFilterMax);
	 - �޽��� ť���� �޽����� �о����
	 - �޽��� ť? : �ý���, ����ڷκ��� �߻��� �޽����� ��� ����ϴ� �޽��� �ӽ� ���� ����
	 - lpMsg : �о���� �޽��� ����
	 - �о���� �޽����� WM_QUIT�� ��� FALSE -> �� WM_QUIT�� �������� ���α׷� ����
	 - ������ �μ��� �޽����� ũ�⸦ ������ -> �� ������� �����Ƿ� PASS

	BOOL TranslateMessage(CONST MSG* lpmsg);
	 - Ű���� �Է� �޽����� �����Ͽ� ���α׷����� ���� �� �� �ֵ��� ��
	 - WM_KEYDOWN Ű���� ���� �̺�Ʈ -> WM_CHAR ���ڰ� �ԷµǾ��ٴ� �޽����� ����� ����

	LONG DispatchMessage(CONST MSG* lpmsg);
	 - �޽��� ť���� ���� �޽����� WndProc �Լ��� ����
	*/

	/*
	�޽��� ����ü

	typedef struct tagMSG
	{
		HWND hwnd; // �޽����� ���� ������ �ڵ�
		UINT message; // �޽����� ���� - ���� �߿�
		WPARAM wParam; // �޽����� ���� �ΰ����� ����
		LPARAM lParam; // �޽����� ���� �ΰ����� ����
		DWORD time; // �޽����� �߻��� �ð�
		POINT pt; // �޽����� �߻��� ���콺 ��ġ
	} MSG;



	*/

	while (GetMessage(&Message, NULL, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	return (int)Message.wParam;
}

// ����ڿ� �ý����� �������� �޽����� ó���ϴ� ����
// Windows ������ WndProc �Լ��� WinMain �Լ� 2���� �־�� ���α׷��� ���� ����

// CALLBACK __stdcall ȣ��Ծ� (APIENTRY, WINAPI)

/*
������ ���ν���
�޽����� �߻��� �� ���α׷��� ������ ó���ϴ� ���� ����
WinMain ���� �޽��� ������ �޽����� �޽��� ó�� �Լ��� �����⸸ �ϰ� WndProc�� �޽����� �ԷµǸ� �ü���� ���� ȣ��Ǿ� �޽����� ó��
-> CALLBACK �Լ�

switch case ������ �޽����� �����Ͽ� ó��

�������� DefWindowProc �Լ��� WndProc���� ó������ ���� ������ �޽����� ó���� �����Ѵ�.
ex) ������ �̵� / ũ�� ����, ���α׷� ����
*/

/*
�޽��� ������ �޽��� ó�� ����
����� ���� / �ý��� ��ȭ / �޽��� �Է� -> �޽��� ť -> MSG ����ü

GetMessage �޽��� ���� -> WM_QUIT - No -> TranslateMessage Ű���� �ؼ� -> DispatchMessage �޽��� ���� -> WndProc �޽��� ó��
�� ������ �ݺ�
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



