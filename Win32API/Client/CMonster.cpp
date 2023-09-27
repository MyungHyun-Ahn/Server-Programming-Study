#include "pch.h"
#include "CMonster.h"
#include "CTimeManager.h"

CMonster::CMonster()
	: m_vCenterPos(Vec2(0.f, 0.f))
	, m_fSpeed(100.f)
	, m_fMaxDistance(50.f)
	, m_fDir(1.f)
{
}

CMonster::~CMonster()
{
}

void CMonster::update()
{
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
