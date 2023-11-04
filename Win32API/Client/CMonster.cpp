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
	GetCollider()->SetScale(Vec2(40.f, 40.f));
}

CMonster::~CMonster()
{
}

void CMonster::update()
{
	return;
	CheckMissileCoolTime();

	Vec2 vCurPos = GetPos();

	// �̵� �ӵ� ��ŭ �̵� - ����
	vCurPos.x += fDT * m_fSpeed * m_fDir;

	// ��ȸ �Ÿ� üũ
	// �������� ��Ȳ
	// ������� ��ŭ���� �ʰ��� �� ����
	// DT ������ �ɰ����� �ð��� �帣�� ���� - ©���� ���� �ð��� �帧
	// ��, ������ ���� �� ����

	// �ʰ���
	float fDiff = abs(m_vCenterPos.x - vCurPos.x) - m_fMaxDistance;

	if (0.f < fDiff)
	{
		m_fDir *= -1;
		// �ʰ��� ��ŭ �ǵ����ֱ�
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
