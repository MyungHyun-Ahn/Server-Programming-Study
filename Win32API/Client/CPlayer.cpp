#include "pch.h"
#include "CPlayer.h"
#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CMissile.h"
#include "CScene.h"
#include "CSceneManager.h"
#include "CCurvedMissile.h"

void CPlayer::update()
{
	// 지역 변수
	Vec2 vPos = GetPos();

	if (KEY_HOLD(KEY::W))
	{
		vPos.y -= 200.f * fDT;
	}

	if (KEY_HOLD(KEY::S))
	{
		vPos.y += 200.f * fDT;
	}

	if (KEY_HOLD(KEY::A))
	{
		vPos.x -= 200.f * fDT;
	}

	if (KEY_HOLD(KEY::D))
	{
		vPos.x += 200.f * fDT;
	}

	if (KEY_TAP(KEY::SPACE))
	{
		CreateMissile();
	}

	SetPos(vPos);
}

void CPlayer::CreateMissile()
{
	Vec2 vMissilePos = GetPos();
	vMissilePos.y -= GetScale().y / 2.f;

	CMissile* pMissile = new CMissile;

	pMissile->SetPos(vMissilePos);
	pMissile->SetScale(Vec2(GetScale().x / 4.f, GetScale().y / 4.f));
	pMissile->SetDir(Vec2(-1.f, -7.f));

	CScene* pCurScene = CSceneManager::GetInstance()->GetCurScene();

	pCurScene->AddObject(pMissile, GROUP_TYPE::DEFAULT);
}

void CPlayer::CreateCurvedMissile()
{
	Vec2 vMissilePos = GetPos();
	vMissilePos.y -= GetScale().y / 2.f;

	CCurvedMissile* pMissile = new CCurvedMissile;

	pMissile->SetPos(vMissilePos);
	pMissile->SetScale(Vec2(GetScale().x / 4.f, GetScale().y / 4.f));

	// 미사일 발사 시 현재 좌표
	pMissile->SetCenter(GetPos());
	pMissile->SetDir(true);

	CScene* pCurScene = CSceneManager::GetInstance()->GetCurScene();

	pCurScene->AddObject(pMissile, GROUP_TYPE::DEFAULT);
}
