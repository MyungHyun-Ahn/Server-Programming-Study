#pragma once


// 싱글톤 패턴
// 객체의 생성을 1개로 제한
// 어디서든 쉽게 접근 가능

// static 정적 멤버 (데이터 영역에 선언) -> 반드시 초기화 해주어야 함
// 클래스 내부에 선언해도 클래스의 크기에 영향을 주지 않음
//class CCore
//{
//public:
//
//	// static 정적 멤버 함수, 객체 없이 호출 가능, this가 없음 (멤버 접근 불가), 정적 멤버는 접근 가능
//	static CCore* GetInstance()
//	{
//		// 최초 호출 된 경우
//		if (pCore == nullptr)
//		{
//			pCore = new CCore;
//		}
//		// 2번 이상 호출된 경우 - 아무것도 할 필요 없음
//
//		return pCore;
//	}
//
//	static void Release()
//	{
//		if (pCore != nullptr)
//		{
//			delete pCore;
//			pCore = nullptr;
//		}
//	}
//
//private:
//	static CCore* pCore;
//
//	CCore() = default;
//	~CCore() = default;
//};

class CCore
{
	SINGLE(CCore);
public:
	int			init(HWND hWnd_, POINT ptResolution_);
	void		progress();

	HWND		GetMainHandle() { return m_hWnd; }
	POINT		GetResolution() { return m_ptResolution; }
	HDC			GetMainDC() { return m_hDC; }

	HBRUSH		GetBrush(BRUSH_TYPE eType_) { return m_arrBrush[(UINT)eType_]; }
	HPEN		GetPen(PEN_TYPE eType_) { return m_arrPen[(UINT)eType_]; }


private:
	void CreateBrushPen();

private:
	HWND	m_hWnd;				// 메인 윈도우 핸들
	POINT	m_ptResolution;		// 메인 윈도우 해상도
	HDC		m_hDC;				// DC의 핸들값

	HBITMAP m_hBit;
	HDC		m_memDC;

	// 자주 사용하는 GDI Object
	HBRUSH	m_arrBrush[(UINT)BRUSH_TYPE::END];
	HPEN	m_arrPen[(UINT)PEN_TYPE::END];

};