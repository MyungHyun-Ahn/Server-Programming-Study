#include "pch.h"
#include "CPathManager.h"
#include "CCore.h"

CPathManager::CPathManager()
	: m_szContentPath{}
{

}

CPathManager::~CPathManager()
{

}


void CPathManager::init()
{
	// ���α׷��� ����� ��ġ : ���� ���丮
	// �迭�� ���� ��ΰ� ä����
	// ��Ʈ�� + F5 : ������Ʈ ���
	// ���� ���� ���� : ���� ���� ���
	GetCurrentDirectory(255, m_szContentPath);

	int iLen = wcslen(m_szContentPath);
	for (int i = iLen - 1; i >= 0; --i)
	{
		if ('\\' == m_szContentPath[i])
		{
			m_szContentPath[i] = '\0';
			break;
		}
	}

	wcscat_s(m_szContentPath, 255, L"\\bin\\content\\");

	// ���� ������
	// m_szContentPath + bin\\content\\ 

	SetWindowText(CCore::GetInstance()->GetMainHandle(), m_szContentPath);
}