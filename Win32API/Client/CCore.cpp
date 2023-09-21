#include "pch.h"
#include "CCore.h"
#include "CObject.h"
#include "CTimeManager.h"
#include "CKeyManager.h"

// CCore* CCore::pCore = nullptr;
// ������ �Լ����� ��ȯ �� ��κ� HRESULT hr
// if (FAILED(E_FAIL))

CObject g_obj;

CCore::CCore() : m_hWnd(0)
				, m_ptResolution{}
				, m_hDC(0)
				, m_hBit(0)
				, m_memDC(0)
{
}

CCore::~CCore()
{
	ReleaseDC(m_hWnd, m_hDC);

	// �����ϴ� ����� �ٸ���
	DeleteDC(m_memDC);
	DeleteObject(m_hBit);
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

	// Manager �ʱ�ȭ
	CTimeManager::GetInstance()->init();
	CKeyManager::GetInstance()->init();


	Vec2 vPos = Vec2((float)(m_ptResolution.x / 2), (float)(m_ptResolution.y / 2));
	g_obj.SetPos(vPos);

	Vec2 vScale = Vec2(100, 100);
	g_obj.SetScale(vScale);

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

	update();

	render();

	// �޽��� ����� ������� ���� ����
	// �׸��� - ����� ������ �� �����Ӹ��� �׸��⸦ ȣ���� ����

}

void CCore::update()
{
	// Ű �Է� Ȯ�� - �޽��� �Ⱦ� - �� �� ���� ���� �񵿱� Ű ����� �Լ�

	Vec2 vPos = g_obj.GetPos();

	// ���� ���� ��Ʈ�� üũ�ؼ� ���ȴ��� ����
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		vPos.x -= 200.f * fDT;
	}

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		vPos.x += 200.f * fDT;
	}

	g_obj.SetPos(vPos);
}

void CCore::render()
{
	// ȭ�� Clear
	// ����� ä��� - �ٵ� ������.
	Rectangle(m_memDC, -1, -1, m_ptResolution.x + 1, m_ptResolution.y + 1);

	Vec2 vPos = g_obj.GetPos();
	Vec2 vScale = g_obj.GetScale();

	Rectangle(m_memDC
		, int(vPos.x - vScale.x / 2.f)
		, int(vPos.y - vScale.y / 2.f)
		, int(vPos.x + vScale.x / 2.f)
		, int(vPos.y + vScale.y / 2.f));


	// ȭ���� �ٲ�ġ�� - ���� ����� �׻� ���� - ������ �� ����Ǳ�� ���� �ʴ�.
	BitBlt(m_hDC, 0, 0, m_ptResolution.x, m_ptResolution.y, m_memDC, 0, 0, SRCCOPY);

	// CPU ���� ó���ؼ� �����ϱ� ������ ��������.
}
