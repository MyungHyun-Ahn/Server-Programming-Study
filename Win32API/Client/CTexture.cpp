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

// �ε��� ��δ� ���� ���

void CTexture::Load(const wstring& strFilePath_)
{
	// �����쿡�� ���� - hinst, path, option, size
	// size �� 0dlaus
	m_hBit = (HBITMAP)LoadImage(nullptr, strFilePath_.c_str(), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);

	// �ε� ����
	assert(m_hBit);

	
}
