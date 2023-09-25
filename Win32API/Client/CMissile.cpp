#include "pch.h"
#include "CMissile.h"
#include "CTimeManager.h"

/*
과제 : 이 정도 구조에서
몬스터 배치 - 원하는 위치에
몬스터들이 일정시간마다 미사일을 쏘게 만들기
Player 미사일 다양한 움직임
 - 움직임 다양화 - 3 방향
*/

/*
다음 시간부터 게임에 리소스 추가
리소스 - 파생 클래스 설계 - Image, Sound
자원
공유 구조

기초 수학
삼각함수 개념
호도법 (라디안), 원주율
내적, 외적

3D 공간 도형
벡터 (x, y, z) - 공간 직선 평면
행렬, 역행렬

물리 1
거리 속력 시간 가속도 등가속도

당구 게임, 힘 기반 유도탄
*/

CMissile::CMissile() 
	: m_fDir(1.f)
{
}

CMissile::~CMissile()
{
}

void CMissile::update()
{
	Vec2 vPos = GetPos();

	vPos.y += 600.f * fDT * m_fDir;

	SetPos(vPos);
}

void CMissile::render(HDC dc_)
{
	Vec2 vPos = GetPos();
	Vec2 vScale = GetScale();

	Ellipse(dc_,
		(int)(vPos.x - vScale.x / 2.f),
		(int)(vPos.y - vScale.y / 2.f),
		(int)(vPos.x + vScale.x / 2.f),
		(int)(vPos.y + vScale.y / 2.f));
}
