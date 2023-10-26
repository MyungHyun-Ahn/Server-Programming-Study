#pragma once

class CPathManager
{
	SINGLE(CPathManager);

public:
	void init();
	wchar_t* GetContentPath() { return m_szContentPath; }

private:
	// 윈도우에서도 경로가 255자 제한
	wchar_t m_szContentPath[255];
};

