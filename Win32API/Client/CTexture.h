#pragma once
#include "CResource.h"
// �̹����� dc�� ����
// �̹��� ũ�⿡ �´� ��Ʈ���� ����
// ��� ��Ʈ�ʿ� �ű��

class CTexture :
    public CResource
{
public:
    CTexture();
    ~CTexture();

public:
    void Load(const wstring& strFilePath_);

private:
    // Ŀ�ο�����Ʈ ID
    HDC         m_dc;
    HBITMAP     m_hBit;
};

