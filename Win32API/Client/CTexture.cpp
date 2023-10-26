#include "pch.h"
#include "CTexture.h"

CTexture::CTexture()
	: m_hBit(0)
	, m_dc(0)
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
	m_hBit = (HBITMAP)LoadImage(nullptr, strFilePath_.c_str(), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);

	// 로딩 실패
	assert(m_hBit);

	
}
