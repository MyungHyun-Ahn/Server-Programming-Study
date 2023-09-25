#pragma once
#include "CObject.h"

// 실제 호출되는 함수는 CPlayer에서 구현한 update 함수

class CPlayer : public CObject
{
public:
	virtual void update();
	
private:
	void CreateMissile();
};

