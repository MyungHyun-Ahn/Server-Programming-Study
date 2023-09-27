#pragma once
#include "CObject.h"
class CCurvedMissile : public CObject
{
public:
	CCurvedMissile();
	~CCurvedMissile();

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

	void SetCenter(Vec2 center_)
	{
		m_vCenter = center_;
	}

private:
	Vec2	m_vCenter;

	float	m_fDir; // 위 아래 방향
	float	m_fSpeed;
	float	m_fDistance;
};

