#pragma once
class CObject
{
public:
	CObject();
	~CObject();

	void SetPos(Vec2 vPos_) { m_vPos = vPos_; }
	void SetScale(Vec2 vScale_) { m_vScale = vScale_; }

	Vec2 GetPos() { return m_vPos; }
	Vec2 GetScale() { return m_vScale; }

private:
	Vec2 m_vPos;
	Vec2 m_vScale;
};

