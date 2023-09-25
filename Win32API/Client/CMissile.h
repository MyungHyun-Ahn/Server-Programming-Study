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

private:
	float m_fDir; // 위 아래 방향

};

