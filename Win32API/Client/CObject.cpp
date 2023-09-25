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
//	// ���� ������ ���� �ڵ��ϱ�?
//	// ������ �Ļ��� ��� ������Ʈ�� �θ� ������Ʈ
//	// �ٸ� �͵鵵 �����̰� ��
//	// Scene -> update�� ��� update
//	// ������Ʈ���� �ϰ��� �ϴ� ���� ��� �ٸ���.
//	// Scene�� �θ� �������� object�� ������ ������ ���� object�� �ٸ� �� �ִ�.
//	// ���� �Լ��� ���ؼ� ���ϴ� ������ �� �� �ְ� ����
//	// ��� - ������
//}

void CObject::render(HDC dc_)
{
	Rectangle(dc_, 
		(int)(m_vPos.x - m_vScale.x / 2.f),
		(int)(m_vPos.y - m_vScale.y / 2.f),
		(int)(m_vPos.x + m_vScale.x / 2.f),
		(int)(m_vPos.y + m_vScale.y / 2.f));
}
