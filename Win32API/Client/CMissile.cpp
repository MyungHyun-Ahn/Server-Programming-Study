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

/*
�밢������ �̵� ��Ű�� �ʹ���
�ʴ� �̵� �ӵ��� ���Ѿ� ��

*/

CMissile::CMissile()
	: m_fDir(1.f)
	, m_fSpeed(600.f)
	, m_fTheta(PI / 4.f)
	, m_vDir(Vec2(1.f, 1.f))
{
	m_vDir.Normalize();
}

CMissile::~CMissile()
{
}

/*
�ܼ��� x�� y�� ���� ���ϰ� �Ǹ� ���ϴ� �ӵ����� �� ���� ��
������ �����ϵ� �ӵ��� �����ϰ� ����
-> �������� �ﰢ�Լ� ���� �ʿ�

ȣ���� - ����
���� PI : ������
������ ���� �ѷ��� ����
r * 3.14 = ���� ����
�������� ���� �ѷ� ���� = ����

1����?
3.14 * ���� = 180��
1������ �뷫 57��

r * 1 = r = �������� r �� �� ȣ�� ���̰� r -> 1 ����
�츮�� ���� �ﰢ�Լ����� ����ϴ� ���� ����

cos = �غ� / ����
sin = ���� / ����
tan = ���� ��, ���� / �غ�
�ﰢ���� ����

���� ��ǥ�� �� (cos, sin) ������ ��Ÿ

���͸� ����ϴ� ��� - ���Ͱ� ��������� �� ���ϴ�
������� 45�� ����
vector <-> scalar
ũ��, ����
3, 2 -> ��Ʈ 13
tan ��Ÿ = 2 / 3
������ -> ��ũ tan

1, 1 �� �� 45��
���̰� 1�� ���� ��ǥ�� �˾ƾ� ��
(cos45, sin45) * DT

������ ����ȭ normalize
���̸� 1�� �����ִ� ��

ȸ���� ������
�ﰢ�Լ� ���� ���� ���
���ϴ� ������ ȸ��
*/

void CMissile::update()
{
	Vec2 vPos = GetPos();

	// vPos.x += m_fSpeed * cosf(m_fTheta) * fDT;
	// vPos.y += m_fDir * m_fSpeed * sinf(m_fTheta) * fDT;

	vPos.x += m_fSpeed * m_vDir.x * fDT;
	vPos.y += m_fSpeed * m_vDir.y * fDT;

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
