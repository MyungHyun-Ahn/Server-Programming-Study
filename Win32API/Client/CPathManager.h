#pragma once

class CPathManager
{
	SINGLE(CPathManager)

public:
	void init();
	wchar_t* GetContentPath() { return m_szContentPath; }

private:
	// �����쿡���� ��ΰ� 255�� ����
	wchar_t m_szContentPath[255];
};

