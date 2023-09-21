#include "pch.h"
#include "CCore.h"
#include "CObject.h"
#include "CTimeManager.h"
#include "CKeyManager.h"

// CCore* CCore::pCore = nullptr;
// 윈도우 함수에서 반환 값 대부분 HRESULT hr
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

	// 삭제하는 방법이 다르다
	DeleteDC(m_memDC);
	DeleteObject(m_hBit);
}



// 여기서 넣어준 해상도가 실제 해상도라는 보장이 없으므로 이걸 맞춰주어야 함
int CCore::init(HWND hWnd_, POINT ptResolution_)
{
	m_hWnd = hWnd_;
	m_ptResolution = ptResolution_;
	
	RECT rt = {0, 0, m_ptResolution.x, m_ptResolution.y};


	// 해상도에 맞게 윈도우 크기 조정
	// 순수하게 물체가 그려질 수 있는 해상도의 영역

	// 윈도우의 위치와 크기를 변경해주는 함수
	// 핸들? 요구? - C 스타일 멤버로 들고있으면 요구할 필요가 없음 -> C++

	// 윈도우 창의 모든 크기를 설정 - 상태바 등 포함
	// 비트연산으로 넘김 -> 메뉴바가 있을 경우에 rt에 값을 채워줌
	// 이것저것 메뉴 등 더 있는 창의 크기를 추가해서 반환
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, true);
	// 계산한 윈도우의 좌우 크기로 세팅
	SetWindowPos(m_hWnd, nullptr, 100, 100, rt.right - rt.left, rt.bottom - rt.top, 0);

	m_hDC = GetDC(m_hWnd);

	// 이중 버퍼링 용도의 비트맵과 DC를 만든다.
	// 윈본과 똑같은 크기
	m_hBit = CreateCompatibleBitmap(m_hDC, m_ptResolution.x, m_ptResolution.y);
	m_memDC = CreateCompatibleDC(m_hDC);

	// 화면 픽셀을 묶어서 비트맵이라고 한다.
	HBITMAP hOldBit = (HBITMAP)SelectObject(m_memDC, m_hBit);
	DeleteObject(hOldBit);

	// Manager 초기화
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
	// 갱신 속도가 너무 빠르다 - 초당 40000번
	// 포지션이 정수 단위로 움직이면 안된다.

	// 컴퓨터 사양에 따라 다른 이동속도가 나오면 안됨
	// 시간 동기화가 필요함
	// 한 프레임 당 움직이는 크기가 고정값이면 안됨
	// 컴퓨터 속도에 따라서 달라져야 함

	// Manager Update
	CTimeManager::GetInstance()->update();

	update();

	render();

	// 메시지 기능을 사용하지 않을 것임
	// 그리기 - 여기다 넣으면 매 프레임마다 그리기를 호출할 것임

}

void CCore::update()
{
	// 키 입력 확인 - 메시지 안씀 - 이 때 쓰는 것이 비동기 키 입출력 함수

	Vec2 vPos = g_obj.GetPos();

	// 제일 상위 비트를 체크해서 눌렸는지 검증
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
	// 화면 Clear
	// 흰색을 채우기 - 근데 느리다.
	Rectangle(m_memDC, -1, -1, m_ptResolution.x + 1, m_ptResolution.y + 1);

	Vec2 vPos = g_obj.GetPos();
	Vec2 vScale = g_obj.GetScale();

	Rectangle(m_memDC
		, int(vPos.x - vScale.x / 2.f)
		, int(vPos.y - vScale.y / 2.f)
		, int(vPos.x + vScale.x / 2.f)
		, int(vPos.y + vScale.y / 2.f));


	// 화면을 바꿔치기 - 복사 비용은 항상 동일 - 앞으로 더 드랍되기는 쉽지 않다.
	BitBlt(m_hDC, 0, 0, m_ptResolution.x, m_ptResolution.y, m_memDC, 0, 0, SRCCOPY);

	// CPU 만을 처리해서 동작하기 때문에 느려진다.
}
