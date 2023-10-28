#pragma once

// 오브젝트의 영역을 검사
// 오브젝트가 움직일 때 Collider도 따라가야함

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
	Vec2		m_vFinalPos; // 최종 위치
	Vec2		m_vScale; // 충돌체의 크기
};

// Offset?
// 상대경로, 어떤 위치에서 상대적인 거리값
// 기준이 되는 좌표 : Owner의 Pos
// 거기서 Offset 만큼 이동한 좌표가 Collider의 위치
// 만약 0, 0 이면 완벽히 Owner와 겹침

// 충돌체가 눈에 보였으면 좋겠음.
// 디버깅 모드에서는 선을 보였다가 나중에는 없앰