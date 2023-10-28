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
//	// ���� ������ ���� �ڵ��ϱ�?
//	// ������ �Ļ��� ��� ������Ʈ�� �θ� ������Ʈ
//	// �ٸ� �͵鵵 �����̰� ��
//	// Scene -> update�� ��� update
//	// ������Ʈ���� �ϰ��� �ϴ� ���� ��� �ٸ���.
//	// Scene�� �θ� �������� object�� ������ ������ ���� object�� �ٸ� �� �ִ�.
//	// ���� �Լ��� ���ؼ� ���ϴ� ������ �� �� �ְ� ����
//	// ��� - ������
//}

// �ڽ� Ŭ������ �������̵��ؼ� ����ϸ� �ȵ�
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

	// ������Ʈ �浹ü,,, etc,,, �� �ִ� ���
	ComponentRender(dc_);
}

// ���� ������ ����.
// render �Լ��� �������̵��ؼ� ����ϴ� ��찡 �ֱ� ������
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
