#pragma once
#include "CObject.h"

// ���� ȣ��Ǵ� �Լ��� CPlayer���� ������ update �Լ�

class CPlayer : public CObject
{
public:
	virtual void update();
	
private:
	void CreateMissile();
};

