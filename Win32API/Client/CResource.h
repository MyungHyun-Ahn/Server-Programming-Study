#pragma once
// 이미지 Image 데이터
// Texture 라고 표현
// 3D 모델링에 색상을 입힘 -> 입혀질 이미지 : Texture
// 물체의 표면 정보

// Texture / Sound 모두 가져야할 공통 부분

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
	wstring m_strKey; // 키값 : 특정 리소스를 찾아오기 위한 목적
	wstring m_strRelativePath; // 리소스 상대경로
};

