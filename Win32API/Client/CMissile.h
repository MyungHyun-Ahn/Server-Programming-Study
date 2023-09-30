#pragma once
#include "CObject.h"

class CMissile : public CObject
{
public:
	CMissile();
	~CMissile();

	virtual void update();
	virtual void render(HDC dc_);

	void SetDir(bool bUp_)
	{
		if (bUp_)
		{
			m_fDir = -1.f;
		}
		else
		{
			m_fDir = 1.f;
		}
	}

	void SetDir(float fTheta_) { m_fTheta = fTheta_; }
	void SetDir(Vec2 vDir_) 
	{ 
		m_vDir = vDir_;
		m_vDir.Normalize();
	}

private:
	float	m_fDir; // 위 아래 방향
	float	m_fSpeed;
	float	m_fTheta;
	Vec2	m_vDir; // 방향 정보
};

