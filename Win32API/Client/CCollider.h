#pragma once

// ������Ʈ�� ������ �˻�
// ������Ʈ�� ������ �� Collider�� ���󰡾���

class CObject;

class CCollider
{
public:
	CCollider();
	CCollider(const CCollider& origin_);

	~CCollider();

	// ���� ������ ����
	CCollider& operator = (CCollider& origin_) = delete;

	friend class CObject;

	void finalUpdate();
	void render(HDC dc_);

	void OnCollision(CCollider* pOther_); // �浹 ���� ��� ȣ��
	void OnCollisionEnter(CCollider* pOther_); // �浹 ���� ��
	void OnCollisionExit(CCollider* pOther_); // �浹 ���� ��
	
	void SetOffsetPos(Vec2 vPos_) { m_vOffsetPos = vPos_; }
	void SetScale(Vec2 vScale_) { m_vScale = vScale_; }

	Vec2 GetOffsetPos() { return m_vOffsetPos; }
	Vec2 GetScale() { return m_vScale; }
	Vec2 GetFinalPos() { return m_vFinalPos; }

	UINT GetID() { return m_iID; }

private:
	static UINT g_iNextID;

	CObject*	m_pOwner;
	Vec2		m_vOffsetPos;
	Vec2		m_vFinalPos; // ���� ��ġ
	Vec2		m_vScale; // �浹ü�� ũ��

	UINT		m_iID; // �浹ü ������ ID
	UINT		m_iCol;
};

// Offset?
// �����, � ��ġ���� ������� �Ÿ���
// ������ �Ǵ� ��ǥ : Owner�� Pos
// �ű⼭ Offset ��ŭ �̵��� ��ǥ�� Collider�� ��ġ
// ���� 0, 0 �̸� �Ϻ��� Owner�� ��ħ

// �浹ü�� ���� �������� ������.
// ����� ��忡���� ���� �����ٰ� ���߿��� ����