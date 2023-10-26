#pragma once
// �̹��� Image ������
// Texture ��� ǥ��
// 3D �𵨸��� ������ ���� -> ������ �̹��� : Texture
// ��ü�� ǥ�� ����

// Texture / Sound ��� �������� ���� �κ�

class CResource
{
public:
	CResource();
	~CResource();

public:
	void SetKey(const wstring& strKey_) { m_strKey = strKey_; }
	void SetRelativePath(const wstring& strPath_) { m_strRelativePath = strPath_; }

	const wstring& GetKey() { return m_strKey; }
	const wstring& GetRelativePath() { return m_strRelativePath; }


private:
	wstring m_strKey; // Ű�� : Ư�� ���ҽ��� ã�ƿ��� ���� ����
	wstring m_strRelativePath; // ���ҽ� �����
};

