#pragma once
#include "CObject.h"

// 실제 호출되는 함수는 CPlayer에서 구현한 update 함수

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

