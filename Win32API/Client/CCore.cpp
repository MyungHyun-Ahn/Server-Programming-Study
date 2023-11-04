#include "pch.h"
#include "CCore.h"
#include "CObject.h"
#include "CTimeManager.h"
#include "CKeyManager.h"
#include "CSceneManager.h"
#include "CPathManager.h"
#include "CCollisionManager.h"

// CCore* CCore::pCore = nullptr;
// 윈도우 함수에서 반환 값 대부분 HRESULT hr
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

	// 삭제하는 방법이 다르다
	DeleteDC(m_memDC);
	DeleteObject(m_hBit);

	for (int i = 0; i < (UINT)PEN_TYPE::END; ++i)
	{
		DeleteObject(m_arrPen[i]);
	}
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

	// 자주 사용할 펜 및 브러쉬 설정
	CreateBrushPen();

	// Manager 초기화
	CPathManager::GetInstance()->init();
	CTimeManager::GetInstance()->init();
	CKeyManager::GetInstance()->init();
	CSceneManager::GetInstance()->init();
	CCollisionManager::GetInstance()->init();

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
	CKeyManager::GetInstance()->update();

	CSceneManager::GetInstance()->update();
	CCollisionManager::GetInstance()->update();


	// Rendering
	Rectangle(m_memDC, -1, -1, m_ptResolution.x + 1, m_ptResolution.y + 1);

	CSceneManager::GetInstance()->render(m_memDC);

	BitBlt(m_hDC, 0, 0, m_ptResolution.x, m_ptResolution.y, m_memDC, 0, 0, SRCCOPY);

	CTimeManager::GetInstance()->render();
}

void CCore::CreateBrushPen()
{
	// hollow brush - 직접 삭제 X
	m_arrBrush[(UINT)BRUSH_TYPE::HOLLOW] = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
	 
	// red blue green pen - 직접 삭제 O
	m_arrPen[(UINT)PEN_TYPE::RED] = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	m_arrPen[(UINT)PEN_TYPE::GREEN] = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	m_arrPen[(UINT)PEN_TYPE::BLUE] = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
}
