#pragma once
#include "CObject.h"

class CMonster : public CObject
{
public:
	CMonster();
	~CMonster();

	virtual void	update();

	float			GetSpeed() { return m_fSpeed; }
	void			SetSpeed(float speed_) { m_fSpeed = speed_; }
	void			SetMoveDIstance(float distance_) { m_fMaxDistance = distance_; }
	void			SetCenterPos(Vec2 vPos_) { m_vCenterPos = vPos_; }


private:
	void			CheckMissileCoolTime();
	void			CreateMissile();

private:
	Vec2		m_vCenterPos;

	float		m_fSpeed;
	float		m_fMaxDistance;
	float		m_fDir; // 1, -1 πÊ«‚

	int			m_iStartTick;
	int			m_iCulTick;
	int			m_iMissileCool;
};

