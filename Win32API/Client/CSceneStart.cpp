#include "pch.h"
#include "CSceneStart.h"
#include "CObject.h"

CSceneStart::CSceneStart()
{
}

CSceneStart::~CSceneStart()
{
}

void CSceneStart::Enter()
{
	// Object Ãß°¡
	CObject* pObj = new CObject;

	pObj->SetPos(Vec2(640.f, 384.f));
	pObj->SetScale(Vec2(100.f, 100.f));

	AddObject(pObj, GROUP_TYPE::DEFAULT);
}

void CSceneStart::Exit()
{
}
