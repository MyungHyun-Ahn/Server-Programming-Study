#include "pch.h"
#include "CResourceManager.h"
#include "CPathManager.h"
#include "CTexture.h"

CResourceManager::CResourceManager()
{

}

CResourceManager::~CResourceManager()
{
	auto it = m_mapTex.begin();
	for (auto it = m_mapTex.begin(); it != m_mapTex.end(); ++it)
	{
		delete it->second;
		it->second = nullptr;
	}
}

// ���ҽ� ���� ���� Key��, �����
CTexture* CResourceManager::LoadTexture(const wstring& strKey_, const wstring& strRelativePath_)
{
	CTexture* pTex = FindTexture(strKey_);
	if (pTex != nullptr)
	{
		return pTex;
	}

	wstring strFilePath = CPathManager::GetInstance()->GetContentPath();
	strFilePath += strRelativePath_;

	pTex = new CTexture;
	pTex->Load(strFilePath);
	pTex->SetKey(strKey_);
	pTex->SetRelativePath(strRelativePath_);

	m_mapTex.insert(make_pair(strKey_, pTex));

	// �̹� ������ Ű�� ������ �ε� X : �� ���ҽ��� �ǳ���

	return pTex;
}

CTexture* CResourceManager::FindTexture(const wstring& strKey_)
{
	auto it = m_mapTex.find(strKey_);

	if (it == m_mapTex.end())
	{
		return nullptr;
	}

	return it->second;
}
