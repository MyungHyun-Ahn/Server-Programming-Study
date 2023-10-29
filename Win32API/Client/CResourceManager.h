#pragma once


class CTexture;

class CResourceManager
{
	SINGLE(CResourceManager)

public:
	CTexture* LoadTexture(const wstring& strKey_, const wstring& strRelativePath_);
	CTexture* FindTexture(const wstring& strKey_);
private:
	map<wstring, CTexture*> m_mapTex;
};

