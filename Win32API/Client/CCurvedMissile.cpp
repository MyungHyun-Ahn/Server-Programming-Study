#include "pch.h"
#include "CCurvedMissile.h"
#include "CTimeManager.h"
#include "CCore.h"

CCurvedMissile::CCurvedMissile()
	: m_vCenter(Vec2(0.f, 0.f))
	, m_fDir(1.f)
	, m_fSpeed(200.f)
	, m_fDistance(150.f)
{
}

CCurvedMissile::~CCurvedMissile()
{
}

void CCurvedMissile::update()
{
	Vec2 vPos = GetPos();

	POINT resolution = CCore::GetInstance()->GetResolution();

	vPos.y += m_fSpeed * fDT * m_fDir;
	vPos.x = (m_fDistance * sinf((vPos.y - m_vCenter.y) * PI / 180.f) + m_vCenter.x);

	SetPos(vPos);

}

void CCurvedMissile::render(HDC dc_)
{
	Vec2 vPos = GetPos();
	Vec2 vScale = GetScale();

	Ellipse(dc_,
		(int)(vPos.x - vScale.x / 2.f),
		(int)(vPos.y - vScale.y / 2.f),
		(int)(vPos.x + vScale.x / 2.f),
		(int)(vPos.y + vScale.y / 2.f));
}
