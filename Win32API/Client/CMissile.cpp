#include "pch.h"
#include "CMissile.h"
#include "CTimeManager.h"

/*
���� : �� ���� ��������
���� ��ġ - ���ϴ� ��ġ��
���͵��� �����ð����� �̻����� ��� �����
Player �̻��� �پ��� ������
 - ������ �پ�ȭ - 3 ����
*/

/*
���� �ð����� ���ӿ� ���ҽ� �߰�
���ҽ� - �Ļ� Ŭ���� ���� - Image, Sound
�ڿ�
���� ����

���� ����
�ﰢ�Լ� ����
ȣ���� (����), ������
����, ����

3D ���� ����
���� (x, y, z) - ���� ���� ���
���, �����

���� 1
�Ÿ� �ӷ� �ð� ���ӵ� ��ӵ�

�籸 ����, �� ��� ����ź
*/

CMissile::CMissile() 
	: m_fDir(1.f)
{
}

CMissile::~CMissile()
{
}

void CMissile::update()
{
	Vec2 vPos = GetPos();

	vPos.y += 600.f * fDT * m_fDir;

	SetPos(vPos);
}

void CMissile::render(HDC dc_)
{
	Vec2 vPos = GetPos();
	Vec2 vScale = GetScale();

	Ellipse(dc_,
		(int)(vPos.x - vScale.x / 2.f),
		(int)(vPos.y - vScale.y / 2.f),
		(int)(vPos.x + vScale.x / 2.f),
		(int)(vPos.y + vScale.y / 2.f));
}
