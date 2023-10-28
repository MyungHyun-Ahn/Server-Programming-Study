#pragma once
#include "CObject.h"

// ���� ȣ��Ǵ� �Լ��� CPlayer���� ������ update �Լ�

class CTexture;

class CPlayer : public CObject
{
public:
	CPlayer();
	~CPlayer();

	virtual void update();
	virtual void render(HDC dc_);
	
private:
	void CreateMissile();
	void CreateCurvedMissile();

private:
	CTexture*	m_pTex;
};

