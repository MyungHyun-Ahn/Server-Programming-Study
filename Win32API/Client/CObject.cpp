#include "pch.h"
#include "CObject.h"
#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CCollider.h"


CObject::CObject() 
	: m_vPos{}
	, m_vScale{}
	, m_pCollider(nullptr)
{
}

CObject::~CObject()
{
	if (m_pCollider != nullptr)
		delete m_pCollider;
}

//void CObject::update()
//{
//	// 과연 문제가 없는 코드일까?
//	// 앞으로 파생될 모든 오브젝트의 부모 오브젝트
//	// 다른 것들도 움직이게 됨
//	// Scene -> update가 모두 update
//	// 오브젝트마다 하고자 하는 일은 모두 다르다.
//	// Scene은 부모 포인터인 object로 관리는 하지만 실제 object는 다를 수 있다.
//	// 가상 함수를 통해서 원하는 행위를 할 수 있게 해줌
//	// 상속 - 다형성
//}

// 자식 클래스가 오버라이딩해서 사용하면 안됨
void CObject::finalUpdate()
{
	if (m_pCollider)
		m_pCollider->finalUpdate();
}

void CObject::render(HDC dc_)
{
	Rectangle(dc_, 
		(int)(m_vPos.x - m_vScale.x / 2.f),
		(int)(m_vPos.y - m_vScale.y / 2.f),
		(int)(m_vPos.x + m_vScale.x / 2.f),
		(int)(m_vPos.y + m_vScale.y / 2.f));

	// 컴포넌트 충돌체,,, etc,,, 가 있는 경우
	ComponentRender(dc_);
}

// 따로 구현한 이유.
// render 함수를 오버라이딩해서 사용하는 경우가 있기 때문에
void CObject::ComponentRender(HDC dc_)
{
	if (m_pCollider != nullptr)
	{
		m_pCollider->render(dc_);
	}
}

void CObject::CreateCollider()
{
	m_pCollider = new CCollider;
	m_pCollider->m_pOwner = this;
}
