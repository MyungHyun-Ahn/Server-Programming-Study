#pragma once

// ������Ʈ�� ������ �˻�
// ������Ʈ�� ������ �� Collider�� ���󰡾���

class CObject;

class CCollider
{
public:
	CCollider();
	~CCollider();

	friend class CObject;

	void finalUpdate();
	void render(HDC dc_);
	
	void SetOffsetPos(Vec2 vPos_) { m_vOffsetPos = vPos_; }
	void SetScale(Vec2 vScale_) { m_vScale = vScale_; }

	Vec2 GetOffsetPos() { return m_vOffsetPos; }
	Vec2 GetScale() { return m_vScale; }

private:
	CObject*	m_pOwner;
	Vec2		m_vOffsetPos;
	Vec2		m_vFinalPos; // ���� ��ġ
	Vec2		m_vScale; // �浹ü�� ũ��
};

// Offset?
// �����, � ��ġ���� ������� �Ÿ���
// ������ �Ǵ� ��ǥ : Owner�� Pos
// �ű⼭ Offset ��ŭ �̵��� ��ǥ�� Collider�� ��ġ
// ���� 0, 0 �̸� �Ϻ��� Owner�� ��ħ

// �浹ü�� ���� �������� ������.
// ����� ��忡���� ���� �����ٰ� ���߿��� ����