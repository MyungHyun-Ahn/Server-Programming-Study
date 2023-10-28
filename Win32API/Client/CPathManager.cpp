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
	// 프로그램이 실행된 위치 : 현재 디렉토리
	// 배열에 현재 경로가 채워짐
	// 컨트롤 + F5 : 프로젝트 경로
	// 실행 파일 실행 : 실행 파일 경로
	GetCurrentDirectory(255, m_szContentPath);

	int iLen = (int)wcslen(m_szContentPath);
	for (int i = iLen - 1; i >= 0; --i)
	{
		if ('\\' == m_szContentPath[i])
		{
			m_szContentPath[i] = '\0';
			break;
		}
	}

	wcscat_s(m_szContentPath, 255, L"\\bin\\content\\");

	// 상위 폴더로
	// m_szContentPath + bin\\content\\ 

	SetWindowText(CCore::GetInstance()->GetMainHandle(), m_szContentPath);
}