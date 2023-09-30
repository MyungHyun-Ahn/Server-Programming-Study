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

/*
대각선으로 이동 시키고 싶더라도
초당 이동 속도는 지켜야 함

*/

CMissile::CMissile()
	: m_fDir(1.f)
	, m_fSpeed(600.f)
	, m_fTheta(PI / 4.f)
	, m_vDir(Vec2(1.f, 1.f))
{
	m_vDir.Normalize();
}

CMissile::~CMissile()
{
}

/*
단순히 x와 y에 값을 더하게 되면 원하는 속도보다 더 가게 됨
방향은 변경하되 속도는 유지하고 싶음
-> 기초적인 삼각함수 지식 필요

호도법 - 라디안
파이 PI : 원주율
지름과 원의 둘레의 비율
r * 3.14 = 원의 절반
반지름과 원의 둘레 비율 = 라디안

1라디안?
3.14 * 라디안 = 180도
1라디안은 대략 57도

r * 1 = r = 반지름이 r 일 때 호의 길이가 r -> 1 라디안
우리가 이제 삼각함수에서 사용하는 값은 라디안

cos = 밑변 / 빗변
sin = 높이 / 빗변
tan = 기울기 값, 높이 / 밑변
삼각형의 비율

원의 좌표는 곧 (cos, sin) 각도는 세타

벡터를 사용하는 방법 - 벡터가 상대적으로 더 편하다
예를들어 45도 진행
vector <-> scalar
크기, 방향
3, 2 -> 루트 13
tan 세타 = 2 / 3
각도는 -> 아크 tan

1, 1 일 때 45도
길이가 1일 때의 좌표를 알아야 함
(cos45, sin45) * DT

벡터의 정규화 normalize
길이를 1로 맞춰주는 것

회전도 편해짐
삼각함수 합차 공식 사용
원하는 축으로 회전
*/

void CMissile::update()
{
	Vec2 vPos = GetPos();

	// vPos.x += m_fSpeed * cosf(m_fTheta) * fDT;
	// vPos.y += m_fDir * m_fSpeed * sinf(m_fTheta) * fDT;

	vPos.x += m_fSpeed * m_vDir.x * fDT;
	vPos.y += m_fSpeed * m_vDir.y * fDT;

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
