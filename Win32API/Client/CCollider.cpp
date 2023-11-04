#include "pch.h"
#include "CCollider.h"
#include "CObject.h"
#include "CCore.h"
#include "SelectGDI.h"

UINT CCollider::g_iNextID = 0;

// Collider�� �����ϴ� ��� ���� ������
// ��������� �ʾƵ� �ڵ����� ���� ������ ����
// �⺻ ���� �����ڴ� �����͸� �״�� ����
// ���縦 �ϴ� �� �ϵ� ������ ��ü���� ID ���� �޶�� ��
// ���� ���簡 �ƴ� ���� ���縦 ���־�� ��
// + ���� ���� : ������ ���� ������?
CCollider::CCollider()
	: m_pOwner(nullptr)
	, m_iID(g_iNextID++) // 42�ﰳ
	, m_iCol(0)
{
}

CCollider::CCollider(const CCollider& origin_)
	: m_pOwner(nullptr) // �����ϸ� ���ο� Collider - ���� �������� �ʾ����Ƿ� nullptr
	, m_vOffsetPos(origin_.m_vOffsetPos)
	, m_vScale(origin_.m_vScale)
	, m_iID(g_iNextID++)
	, m_iCol(origin_.m_iCol)
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

	// m_iCol�� ������ �߸��� ��Ȳ Crash
	assert(m_iCol >= 0);
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

	PEN_TYPE ePen = PEN_TYPE::GREEN;

	if (m_iCol)
		ePen = PEN_TYPE::RED;

	SelectGDI sPen(dc_, ePen);
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


// 2 ���̿��� �浹
// �� 3�ڰ� �����
// Exit�� �߻�
// false�� �� -> 3�ڿʹ� ���� �������� �ʾ����� ���� ó��
// Count�� ���� ó��
void CCollider::OnCollision(CCollider* pOther_)
{

}

void CCollider::OnCollisionEnter(CCollider* pOther_)
{
	++m_iCol;
}

void CCollider::OnCollisionExit(CCollider* pOther_)
{
	--m_iCol;
}
