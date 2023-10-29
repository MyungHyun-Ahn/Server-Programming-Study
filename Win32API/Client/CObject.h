#pragma once

// 오브젝트마다 특징이 있음
// 오브젝트를 일괄적으로 관리해주는 것들이 필요함
// -> Scene
// 게임 처음 시작할 때
// 장면 각 Scene 단위로 구분

// 순수 가상 함수

class CCollider;

class CObject
{
public:
	CObject();
	virtual ~CObject();

	virtual void update() = 0;
	virtual void finalUpdate() final; // final 키워드 오버라이딩을 막음
	virtual void render(HDC dc_);
	void ComponentRender(HDC dc_);

	void SetPos(Vec2 vPos_) { m_vPos = vPos_; }
	void SetScale(Vec2 vScale_) { m_vScale = vScale_; }

	Vec2 GetPos() { return m_vPos; }
	Vec2 GetScale() { return m_vScale; }

	CCollider* GetCollider() { return m_pCollider; }

	// 필요하다면 호출
	void CreateCollider();

private:
	Vec2 m_vPos;
	Vec2 m_vScale;

	CCollider* m_pCollider;
};

// 충돌처리
// 충돌을 어떻게 처리?
// 부딫힘을 판단. 매 프레임마다?
// 컴퓨터에 속도에 따라 컴퓨터가 충돌을 처리하는 횟수가 달라질수도 있음.
// 디테일한 조건 필요
// 한번 충돌하면 1번이라고 판단
// 필요한 이벤트 
// 두 물체가 충돌한 순간. Enter 에서 조건
// 이후는 충돌 중인 것
// 충돌을 벗어나는 시점. Exit.
// 3가지 상태를 관리 CALLBACK 함수를 통해

// 충돌이 필요 없는 Object도 있을 수도 있음 - ex UI
// Object - Collider, Non-Collider
// 상속 구조를 사용하면 구조의 확장성이 떨어진다.
// 
// 이때 사용하는 것이 컴포넌트 기반 - 부품
// 필요하면 활성화, 없다면 비활성화