#include "pch.h"
#include "CTexture.h"

#include "CCore.h"

CTexture::CTexture()
	: m_hBit(0)
	, m_dc(0)
	, m_bitInfo{}
{
}

CTexture::~CTexture()
{
	DeleteDC(m_dc);
	DeleteObject(m_hBit);
}

// 로딩된 경로는 절대 경로

void CTexture::Load(const wstring& strFilePath_)
{
	// 윈도우에서 제공 - hinst, path, option, size
	// size 가 0dlaus
	// 파일로부터 로딩한 데이터를 비트맵으로 생성
	m_hBit = (HBITMAP)LoadImage(nullptr, strFilePath_.c_str(), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);

	// 로딩 실패
	assert(m_hBit);

	// 비트맵과 연결할 DC
	m_dc = CreateCompatibleDC(CCore::GetInstance()->GetMainDC());

	// 비트맵과 DC 연결
	HBITMAP hPrecBit = (HBITMAP)SelectObject(m_dc, m_hBit);

	// 가로 세로 길이
	// 특정 오브젝트의 정보를 얻어오는 함수
	// BITMAP tInfo = {}; // 구조체에 정보를 채워줌
	GetObject(m_hBit, sizeof(BITMAP), &m_bitInfo);
	
}
