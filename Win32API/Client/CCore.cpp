#include "pch.h"
#include "CCore.h"
#include "CObject.h"
#include "CTimeManager.h"
#include "CKeyManager.h"
#include "CSceneManager.h"
#include "CPathManager.h"
#include "CCollisionManager.h"

// CCore* CCore::pCore = nullptr;
// ������ �Լ����� ��ȯ �� ��κ� HRESULT hr
// if (FAILED(E_FAIL))

CCore::CCore() 
	: m_hWnd(0)
	, m_ptResolution{}
	, m_hDC(0)
	, m_hBit(0)
	, m_memDC(0)
	, m_arrBrush{}
	, m_arrPen{}
{
}

CCore::~CCore()
{
	ReleaseDC(m_hWnd, m_hDC);

	// �����ϴ� ����� �ٸ���
	DeleteDC(m_memDC);
	DeleteObject(m_hBit);

	for (int i = 0; i < (UINT)PEN_TYPE::END; ++i)
	{
		DeleteObject(m_arrPen[i]);
	}
}



// ���⼭ �־��� �ػ󵵰� ���� �ػ󵵶�� ������ �����Ƿ� �̰� �����־�� ��
int CCore::init(HWND hWnd_, POINT ptResolution_)
{
	m_hWnd = hWnd_;
	m_ptResolution = ptResolution_;
	
	RECT rt = {0, 0, m_ptResolution.x, m_ptResolution.y};


	// �ػ󵵿� �°� ������ ũ�� ����
	// �����ϰ� ��ü�� �׷��� �� �ִ� �ػ��� ����

	// �������� ��ġ�� ũ�⸦ �������ִ� �Լ�
	// �ڵ�? �䱸? - C ��Ÿ�� ����� ��������� �䱸�� �ʿ䰡 ���� -> C++

	// ������ â�� ��� ũ�⸦ ���� - ���¹� �� ����
	// ��Ʈ�������� �ѱ� -> �޴��ٰ� ���� ��쿡 rt�� ���� ä����
	// �̰����� �޴� �� �� �ִ� â�� ũ�⸦ �߰��ؼ� ��ȯ
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, true);
	// ����� �������� �¿� ũ��� ����
	SetWindowPos(m_hWnd, nullptr, 100, 100, rt.right - rt.left, rt.bottom - rt.top, 0);

	m_hDC = GetDC(m_hWnd);

	// ���� ���۸� �뵵�� ��Ʈ�ʰ� DC�� �����.
	// ������ �Ȱ��� ũ��
	m_hBit = CreateCompatibleBitmap(m_hDC, m_ptResolution.x, m_ptResolution.y);
	m_memDC = CreateCompatibleDC(m_hDC);

	// ȭ�� �ȼ��� ��� ��Ʈ���̶�� �Ѵ�.
	HBITMAP hOldBit = (HBITMAP)SelectObject(m_memDC, m_hBit);
	DeleteObject(hOldBit);

	// ���� ����� �� �� �귯�� ����
	CreateBrushPen();

	// Manager �ʱ�ȭ
	CPathManager::GetInstance()->init();
	CTimeManager::GetInstance()->init();
	CKeyManager::GetInstance()->init();
	CSceneManager::GetInstance()->init();
	CCollisionManager::GetInstance()->init();

	return S_OK;
}

void CCore::progress()
{
	// ���� �ӵ��� �ʹ� ������ - �ʴ� 40000��
	// �������� ���� ������ �����̸� �ȵȴ�.

	// ��ǻ�� ��翡 ���� �ٸ� �̵��ӵ��� ������ �ȵ�
	// �ð� ����ȭ�� �ʿ���
	// �� ������ �� �����̴� ũ�Ⱑ �������̸� �ȵ�
	// ��ǻ�� �ӵ��� ���� �޶����� ��

	// Manager Update
	CTimeManager::GetInstance()->update();
	CKeyManager::GetInstance()->update();

	CSceneManager::GetInstance()->update();
	CCollisionManager::GetInstance()->update();


	// Rendering
	Rectangle(m_memDC, -1, -1, m_ptResolution.x + 1, m_ptResolution.y + 1);

	CSceneManager::GetInstance()->render(m_memDC);

	BitBlt(m_hDC, 0, 0, m_ptResolution.x, m_ptResolution.y, m_memDC, 0, 0, SRCCOPY);

	// CTimeManager::GetInstance()->render();
}

void CCore::CreateBrushPen()
{
	// hollow brush - ���� ���� X
	m_arrBrush[(UINT)BRUSH_TYPE::HOLLOW] = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
	 
	// red blue green pen - ���� ���� O
	m_arrPen[(UINT)PEN_TYPE::RED] = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	m_arrPen[(UINT)PEN_TYPE::GREEN] = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	m_arrPen[(UINT)PEN_TYPE::BLUE] = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
}
