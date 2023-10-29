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

// Object 위치로 따라갈 수 있게
// Object의 위치가 충돌체의 위치가 아닐까?
// 유연한 프로그래밍이 불가능해짐
// ex) 헤드샷 판정, 몸통, 부위별로 충돌 판정
// 동일 시 되면 안됨
void CCollider::finalUpdate()
{
	Vec2 vObjectPos = m_pOwner->GetPos();
	m_vFinalPos = vObjectPos + m_vOffsetPos;
}

// Object 쪽에서 호출
// 사각형을 그리면 이미지가 지워짐
// 라인만 그리기 - DrawLine

// 사각형을 그리고 hollow 브러쉬를 선택함 - 채우기를 투명으로
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
