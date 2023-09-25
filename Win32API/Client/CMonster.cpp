#include "pch.h"
#include "CMonster.h"
#include "CTimeManager.h"

CMonster::CMonster()
	: m_vCenterPos(Vec2(0.f, 0.f))
	, m_fSpeed(100.f)
	, m_fMaxDistance(50.f)
	, m_iDir(1)
{
}

CMonster::~CMonster()
{
}

void CMonster::update()
{
	Vec2 vCurPos = GetPos();

	// 이동 속도 만큼 이동 - 방향
	vCurPos.x += fDT * m_fSpeed * m_iDir;

	// 배회 거리 체크
	// 디테일한 상황
	// 가기로한 만큼보다 초과할 수 있음
	// DT 단위로 쪼개져서 시간이 흐르기 때문 - 짤려서 툭툭 시간이 흐름
	// 즉, 오차가 생길 수 있음

	// 초과량
	float fDiff = abs(m_vCenterPos.x - vCurPos.x) - m_fMaxDistance;

	if (0.f < fDiff)
	{
		m_iDir *= -1;
		// 초과한 만큼 되돌려주기
		vCurPos.x += fDiff * m_iDir;
	}

	SetPos(vCurPos);
}
