#pragma once

// ������Ʈ���� Ư¡�� ����
// ������Ʈ�� �ϰ������� �������ִ� �͵��� �ʿ���
// -> Scene
// ���� ó�� ������ ��
// ��� �� Scene ������ ����

// ���� ���� �Լ�

class CCollider;

class CObject
{
public:
	CObject();
	virtual ~CObject();

	virtual void update() = 0;
	virtual void finalUpdate() final; // final Ű���� �������̵��� ����
	virtual void render(HDC dc_);
	void ComponentRender(HDC dc_);

	void SetPos(Vec2 vPos_) { m_vPos = vPos_; }
	void SetScale(Vec2 vScale_) { m_vScale = vScale_; }

	Vec2 GetPos() { return m_vPos; }
	Vec2 GetScale() { return m_vScale; }

	// �ʿ��ϴٸ� ȣ��
	void CreateCollider();

private:
	Vec2 m_vPos;
	Vec2 m_vScale;

	CCollider* m_pCollider;
};

// �浹ó��
// �浹�� ��� ó��?
// �΋H���� �Ǵ�. �� �����Ӹ���?
// ��ǻ�Ϳ� �ӵ��� ���� ��ǻ�Ͱ� �浹�� ó���ϴ� Ƚ���� �޶������� ����.
// �������� ���� �ʿ�
// �ѹ� �浹�ϸ� 1���̶�� �Ǵ�
// �ʿ��� �̺�Ʈ 
// �� ��ü�� �浹�� ����. Enter ���� ����
// ���Ĵ� �浹 ���� ��
// �浹�� ����� ����. Exit.
// 3���� ���¸� ���� CALLBACK �Լ��� ����

// �浹�� �ʿ� ���� Object�� ���� ���� ���� - ex UI
// Object - Collider, Non-Collider
// ��� ������ ����ϸ� ������ Ȯ�强�� ��������.
// 
// �̶� ����ϴ� ���� ������Ʈ ��� - ��ǰ
// �ʿ��ϸ� Ȱ��ȭ, ���ٸ� ��Ȱ��ȭ