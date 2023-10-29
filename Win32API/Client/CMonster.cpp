#include "pch.h"
#include "CMonster.h"
#include "CTimeManager.h"
#include "CMissile.h"
#include "CScene.h"
#include "CSceneManager.h"
#include "CCollider.h"

CMonster::CMonster()
	: m_vCenterPos(Vec2(0.f, 0.f))
	, m_fSpeed(100.f)
	, m_fMaxDistance(50.f)
	, m_fDir(1.f)
	, m_iStartTick(0)
	, m_iCulTick(0)
	, m_iMissileCool(5000)
{
	CreateCollider();
	GetCollider()->SetScale(Vec2(50.f, 50.f));
}

CMonster::~CMonster()
{
}

void CMonster::update()
{
	CheckMissileCoolTime();

	Vec2 vCurPos = GetPos();

	// 이동 속도 만큼 이동 - 방향
	vCurPos.x += fDT * m_fSpeed * m_fDir;

	// 배회 거리 체크
	// 디테일한 상황
	// 가기로한 만큼보다 초과할 수 있음
	// DT 단위로 쪼개져서 시간이 흐르기 때문 - 짤려서 툭툭 시간이 흐름
	// 즉, 오차가 생길 수 있음

	// 초과량
	float fDiff = abs(m_vCenterPos.x - vCurPos.x) - m_fMaxDistance;

	if (0.f < fDiff)
	{
		m_fDir *= -1;
		// 초과한 만큼 되돌려주기
		vCurPos.x += fDiff * m_fDir;
	}

	SetPos(vCurPos);
}

void CMonster::CheckMissileCoolTime()
{
	m_iCulTick = (int)GetTickCount64();

	int diff = m_iCulTick - m_iStartTick;
	if (diff > m_iMissileCool)
	{
		CreateMissile();
		m_iStartTick = (int)GetTickCount64();
	}
}

void CMonster::CreateMissile()
{
	Vec2 vMissilePos = GetPos();
	vMissilePos.y += GetScale().y / 2.f;

	CMissile* pMissile = new CMissile;

	pMissile->SetPos(vMissilePos);
	pMissile->SetScale(Vec2(GetScale().x / 4.f, GetScale().y / 4.f));
	pMissile->SetDir(false);

	CScene* pCurScene = CSceneManager::GetInstance()->GetCurScene();

	pCurScene->AddObject(pMissile, GROUP_TYPE::DEFAULT);
}
