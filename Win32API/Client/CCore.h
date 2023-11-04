#pragma once


// �̱��� ����
// ��ü�� ������ 1���� ����
// ��𼭵� ���� ���� ����

// static ���� ��� (������ ������ ����) -> �ݵ�� �ʱ�ȭ ���־�� ��
// Ŭ���� ���ο� �����ص� Ŭ������ ũ�⿡ ������ ���� ����
//class CCore
//{
//public:
//
//	// static ���� ��� �Լ�, ��ü ���� ȣ�� ����, this�� ���� (��� ���� �Ұ�), ���� ����� ���� ����
//	static CCore* GetInstance()
//	{
//		// ���� ȣ�� �� ���
//		if (pCore == nullptr)
//		{
//			pCore = new CCore;
//		}
//		// 2�� �̻� ȣ��� ��� - �ƹ��͵� �� �ʿ� ����
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
	SINGLE(CCore)
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
	HWND	m_hWnd;				// ���� ������ �ڵ�
	POINT	m_ptResolution;		// ���� ������ �ػ�
	HDC		m_hDC;				// DC�� �ڵ鰪

	HBITMAP m_hBit;
	HDC		m_memDC;

	// ���� ����ϴ� GDI Object
	HBRUSH	m_arrBrush[(UINT)BRUSH_TYPE::END];
	HPEN	m_arrPen[(UINT)PEN_TYPE::END];

};