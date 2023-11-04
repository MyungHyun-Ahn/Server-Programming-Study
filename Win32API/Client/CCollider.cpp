#include "pch.h"
#include "CCollider.h"
#include "CObject.h"
#include "CCore.h"
#include "SelectGDI.h"

UINT CCollider::g_iNextID = 0;

// Collider를 복사하는 경우 복사 생성자
// 만들어주지 않아도 자동으로 복사 생성자 생성
// 기본 복사 생성자는 데이터를 그대로 복사
// 복사를 하던 뭘 하든 각각의 개체들은 ID 값은 달라야 함
// 얕은 복사가 아닌 깊은 복사를 해주어야 함
// + 대입 연산 : 대입할 일이 있을까?
CCollider::CCollider()
	: m_pOwner(nullptr)
	, m_iID(g_iNextID++) // 42억개
	, m_iCol(0)
{
}

CCollider::CCollider(const CCollider& origin_)
	: m_pOwner(nullptr) // 복사하면 새로운 Collider - 따로 지정하진 않았으므로 nullptr
	, m_vOffsetPos(origin_.m_vOffsetPos)
	, m_vScale(origin_.m_vScale)
	, m_iID(g_iNextID++)
	, m_iCol(origin_.m_iCol)
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

	// m_iCol이 음수면 잘못된 상황 Crash
	assert(m_iCol >= 0);
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


// 2 사이에서 충돌
// 제 3자가 끼어듬
// Exit가 발생
// false가 들어감 -> 3자와는 아직 떨어지지 않았지만 해제 처리
// Count를 세서 처리
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
