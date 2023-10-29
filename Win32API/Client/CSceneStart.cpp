#include "pch.h"
#include "CSceneStart.h"
#include "CObject.h"
#include "CPlayer.h"
#include "CMonster.h"
#include "CCore.h"
#include "CTexture.h"
#include "CPathManager.h"
#include "CCollisionManager.h"

CSceneStart::CSceneStart()
{
}

CSceneStart::~CSceneStart()
{
}

void CSceneStart::Enter()
{
	// Object 추가 - 실제 생성된 것은 Player 객체
	CObject* pObj = new CPlayer;

	pObj->SetPos(Vec2(640.f, 384.f));
	pObj->SetScale(Vec2(100.f, 100.f));

	AddObject(pObj, GROUP_TYPE::PLAYER);

	Vec2 vResolution = CCore::GetInstance()->GetResolution();

	int iMonCount = 10;
	float fMoveDist = 25.f;
	float fObjScale = 50.f;
	float fTerm = (vResolution.x - ((fMoveDist + fObjScale / 2.f) * 2)) / (float)(iMonCount - 1);

	// 몬스터 Object 추가
	CMonster* pMonsterObj = nullptr;
	for (int i = 0; i < iMonCount; ++i)
	{
		pMonsterObj = new CMonster;

		pMonsterObj->SetPos(Vec2((fMoveDist + fObjScale / 2.f) + fTerm * (float)i, 50.f));
		pMonsterObj->SetScale(Vec2(fObjScale, fObjScale));
		pMonsterObj->SetMoveDIstance(fMoveDist);
		pMonsterObj->SetCenterPos(pMonsterObj->GetPos());

		AddObject(pMonsterObj, GROUP_TYPE::MONSTER);
	}

	// 충돌 지정
	// Player 그룹과 Monster 그룹 간의 충돌체크
	CCollisionManager::GetInstance()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::MONSTER);
}

void CSceneStart::Exit()
{
	// 충돌 지정해놨던 것을 해제
	CCollisionManager::GetInstance()->Reset();

}
