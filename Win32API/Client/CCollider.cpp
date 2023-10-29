#include "pch.h"
#include "CCollider.h"
#include "CObject.h"
#include "CCore.h"
#include "SelectGDI.h"

CCollider::CCollider()
	: m_pOwner(nullptr)
{
}

CCollider::~CCollider()
{
}

// Object ��ġ�� ���� �� �ְ�
// Object�� ��ġ�� �浹ü�� ��ġ�� �ƴұ�?
// ������ ���α׷����� �Ұ�������
// ex) ��弦 ����, ����, �������� �浹 ����
// ���� �� �Ǹ� �ȵ�
void CCollider::finalUpdate()
{
	Vec2 vObjectPos = m_pOwner->GetPos();
	m_vFinalPos = vObjectPos + m_vOffsetPos;
}

// Object �ʿ��� ȣ��
// �簢���� �׸��� �̹����� ������
// ���θ� �׸��� - DrawLine

// �簢���� �׸��� hollow �귯���� ������ - ä��⸦ ��������
void CCollider::render(HDC dc_)
{
	// HPEN hGreenPen = CCore::GetInstance()->GetPen(PEN_TYPE::GREEN);
	// HPEN hDefaultPen = (HPEN)SelectObject(dc_, hGreenPen);

	// HBRUSH hBrush = CCore::GetInstance()->GetBrush(BRUSH_TYPE::HOLLOW);
	// HBRUSH hDefaultBrush = (HBRUSH)SelectObject(dc_, hBrush);

	SelectGDI sPen(dc_, PEN_TYPE::GREEN);
	SelectGDI sBrush(dc_, BRUSH_TYPE::HOLLOW);

	Rectangle(dc_
		, m_vFinalPos.x - m_vScale.x / 2.f
		, m_vFinalPos.y - m_vScale.y / 2.f
		, m_vFinalPos.x + m_vScale.x / 2.f
		, m_vFinalPos.y + m_vScale.y / 2.f
		);

	// SelectObject(dc_, hDefaultPen);
	// SelectObject(dc_, hDefaultBrush);
}
