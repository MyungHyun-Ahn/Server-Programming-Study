#pragma once
#include "CScene.h"

class CSceneStart : public CScene
{
public:
	CSceneStart();
	~CSceneStart();

	virtual void Enter() override;
	virtual void Exit() override;
};

