#pragma once
#include "CResource.h"
// 이미지를 dc에 연결
// 이미지 크기에 맞는 비트맵을 생성
// 출력 비트맵에 옮기기

class CTexture :
    public CResource
{
public:
    CTexture();
    ~CTexture();

public:
    void Load(const wstring& strFilePath_);

private:
    // 커널오브젝트 ID
    HDC         m_dc;
    HBITMAP     m_hBit;
};

