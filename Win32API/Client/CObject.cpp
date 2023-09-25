#include "pch.h"
#include "CObject.h"
#include "CKeyManager.h"
#include "CTimeManager.h"


CObject::CObject() : m_vPos{}
				, m_vScale{}
{
}

CObject::~CObject()
{
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

void CObject::render(HDC dc_)
{
	Rectangle(dc_, 
		(int)(m_vPos.x - m_vScale.x / 2.f),
		(int)(m_vPos.y - m_vScale.y / 2.f),
		(int)(m_vPos.x + m_vScale.x / 2.f),
		(int)(m_vPos.y + m_vScale.y / 2.f));
}
