#pragma once
#include "CResource.h"
// 이미지를 dc에 연결
// 이미지 크기에 맞는 비트맵을 생성
// 출력 비트맵에 옮기기

class CTexture :
    public CResource
{
private:
    CTexture();
    ~CTexture();

    friend class CResourceManager; // ResorceManager 에서만 생성 가능

public:
    void Load(const wstring& strFilePath_);

    UINT GetWidth() { return m_bitInfo.bmWidth; }
    UINT GetHeight() { return m_bitInfo.bmHeight; }

    HDC GetDC() { return m_dc; }

private:
    // 커널오브젝트 ID
    HDC         m_dc;
    HBITMAP     m_hBit;
    BITMAP      m_bitInfo;
};

