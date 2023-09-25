#pragma once

// 오브젝트마다 특징이 있음
// 오브젝트를 일괄적으로 관리해주는 것들이 필요함
// -> Scene
// 게임 처음 시작할 때
// 장면 각 Scene 단위로 구분

// 순수 가상 함수
class CObject
{
public:
	CObject();
	virtual ~CObject();

	virtual void update() = 0;
	virtual void render(HDC dc_);

	void SetPos(Vec2 vPos_) { m_vPos = vPos_; }
	void SetScale(Vec2 vScale_) { m_vScale = vScale_; }

	Vec2 GetPos() { return m_vPos; }
	Vec2 GetScale() { return m_vScale; }

private:
	Vec2 m_vPos;
	Vec2 m_vScale;
};

