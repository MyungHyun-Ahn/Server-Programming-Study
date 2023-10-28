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

// �ε��� ��δ� ���� ���

void CTexture::Load(const wstring& strFilePath_)
{
	// �����쿡�� ���� - hinst, path, option, size
	// size �� 0dlaus
	// ���Ϸκ��� �ε��� �����͸� ��Ʈ������ ����
	m_hBit = (HBITMAP)LoadImage(nullptr, strFilePath_.c_str(), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);

	// �ε� ����
	assert(m_hBit);

	// ��Ʈ�ʰ� ������ DC
	m_dc = CreateCompatibleDC(CCore::GetInstance()->GetMainDC());

	// ��Ʈ�ʰ� DC ����
	HBITMAP hPrecBit = (HBITMAP)SelectObject(m_dc, m_hBit);

	// ���� ���� ����
	// Ư�� ������Ʈ�� ������ ������ �Լ�
	// BITMAP tInfo = {}; // ����ü�� ������ ä����
	GetObject(m_hBit, sizeof(BITMAP), &m_bitInfo);
	
}
