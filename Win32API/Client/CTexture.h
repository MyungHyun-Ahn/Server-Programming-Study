#pragma once
#include "CResource.h"
// �̹����� dc�� ����
// �̹��� ũ�⿡ �´� ��Ʈ���� ����
// ��� ��Ʈ�ʿ� �ű��

class CTexture :
    public CResource
{
private:
    CTexture();
    ~CTexture();

    friend class CResourceManager; // ResorceManager ������ ���� ����

public:
    void Load(const wstring& strFilePath_);

    UINT GetWidth() { return m_bitInfo.bmWidth; }
    UINT GetHeight() { return m_bitInfo.bmHeight; }

    HDC GetDC() { return m_dc; }

private:
    // Ŀ�ο�����Ʈ ID
    HDC         m_dc;
    HBITMAP     m_hBit;
    BITMAP      m_bitInfo;
};

