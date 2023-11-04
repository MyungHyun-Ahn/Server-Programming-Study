#include "pch.h"
#include "CCollisionManager.h"
#include "CSceneManager.h"
#include "CScene.h"
#include "CObject.h"
#include "CCollider.h"

// union
// 구조체랑 비슷
// 유니온의 특징 - 구조체와 비교

// 유니온은 제일 큰 사이즈의 타입을 기준으로 같은 공간을 공유함
//union Test
//{
//	int a;
//	float b;
//};
// 4바이트
// 정수 타입으로 부르면 a, 실수 타입으로 부르면 b
 
// LARGE_INTEGER 8 바이트 정수
// [   LowPart  |   HighPart  ] QuadPart; 
// QuadPart 로 사용하면 전체를 8바이트 정수
// LowPart 4바이트, HighPart 4바이트


CCollisionManager::CCollisionManager()
	: m_arrCheck{}
{

}

CCollisionManager::~CCollisionManager()
{

}

void CCollisionManager::init()
{

}

void CCollisionManager::update()
{
	// 체크된 것의 충돌 검사
	for (UINT iRow = 0; iRow < (UINT)GROUP_TYPE::END; ++iRow)
	{
		// 중복되는 부분은 검사하지 않을 것임
		for (UINT iCol = iRow; iCol < (UINT)GROUP_TYPE::END; ++iCol)
		{
			if (m_arrCheck[iRow] & (1 << iCol))
			{
				// 체크한 자리 검사
				// 현재 Scene의 그룹 물체를 가져와서 비교
				CollisionUpdateGroup((GROUP_TYPE)iRow, (GROUP_TYPE)iCol);
			}
		}
	}
}

void CCollisionManager::CheckGroup(GROUP_TYPE eLeft_, GROUP_TYPE eRight_)
{
	// 더 작은 값의 그룹 타입을 행으로
	// 큰 값을 열(비트)로 사용

	UINT iRow = (UINT)eLeft_;
	UINT iCol = (UINT)eRight_;

	if (iCol < iRow)
	{
		std::swap(iCol, iRow);
	}

	// 1을 컬럼만큼 좌측 쉬프트
	// 이미 값이 1이면?
	if (m_arrCheck[iRow] & (1 << iCol))
	{
		m_arrCheck[iRow] &= ~(1 << iCol); // 원하는 곳의 비트를 빼줌
	}
	else
	{
		m_arrCheck[iRow] |= (1 << iCol); // 원하는 곳에 체크
	}
}

// 항상 작은 값이 왼쪽
void CCollisionManager::CollisionUpdateGroup(GROUP_TYPE eLeft_, GROUP_TYPE eRight_)
{
	CScene* pCurScene = CSceneManager::GetInstance()->GetCurScene();

	// 문제? - 의도랑은 어긋나는 부분
	// 원본 벡터를 참조해서 받아왔음. 근데 받은 변수는 지역 변수 = 복사 받음
	// 받을 때도 const vec& 변수로 받아야 함
	const vector<CObject*>& vecLeft = pCurScene->GetGroupObject(eLeft_);
	const vector<CObject*>& vecRight = pCurScene->GetGroupObject(eRight_);

	map<ULONGLONG, bool>::iterator it;

	for (size_t i = 0; i < vecLeft.size(); ++i)
	{
		if (vecLeft[i]->GetCollider() == nullptr) // 충돌체가 없다면 건너뛰기
			continue;

		for (size_t j = 0; j < vecRight.size(); ++j)
		{
			// 충돌체가 없거나, 자기 자신과 충돌인 경우
			if (vecRight[j]->GetCollider() == nullptr
				|| vecLeft[i] == vecRight[j])
				continue;

			CCollider* pLeftCol = vecLeft[i]->GetCollider();
			CCollider* pRightCol = vecRight[j]->GetCollider();


			// 두 충돌체 조합 ID 생성
			COLLIDER_ID ID;
			ID.iLeft_id = pLeftCol->GetID();
			ID.iRight_id = pRightCol->GetID();
			// ID.ID = iLeft + iRight // 두 충돌체만으로밖에 나올 수 없는 값

			it = m_mapColInfo.find(ID.ID);
			if (it == m_mapColInfo.end())
			{
				// 정보 없음
				// 검사를 한번이라도 한 경우에는 정보를 등록해야 함
				// 미 등록 상태라면 등록
				m_mapColInfo.insert(std::make_pair(ID.ID, false));
				it = m_mapColInfo.find(ID.ID);
			}

			// 충돌 검사
			// 충돌 이벤트 - Key입력 이벤트와 비슷하게 만들어 주어야 함
			if (IsCollision(pLeftCol, pRightCol))
			{
				// 현재 충돌 중이다.

				if (it->second)
				{
					// 이전에도 충돌하고 있었음 - ing~
					pLeftCol->OnCollision(pRightCol);
					pRightCol->OnCollision(pLeftCol);

				}
				else
				{
					// 이전에는 충돌하지 않았다. - 첫 시점
					pLeftCol->OnCollisionEnter(pRightCol);
					pRightCol->OnCollisionEnter(pLeftCol);
					it->second = true;
				}

			}
			else // 충돌 X
			{
				// 현재 충돌하지 않고 있다.
				if (it->second)
				{
					// 이전에는 충돌하고 있었다.
					pLeftCol->OnCollisionExit(pRightCol);
					pRightCol->OnCollisionExit(pLeftCol);
					it->second = false;
				}
			}
		}
	}
}

bool CCollisionManager::IsCollision(CCollider* pLeftCol_, CCollider* pRightCol_)
{
	// finalPos를 가져와야 함
	Vec2 vLeftPos = pLeftCol_->GetFinalPos();
	Vec2 vLeftScale = pLeftCol_->GetScale();

	Vec2 vRightPos = pRightCol_->GetFinalPos();
	Vec2 vRightScale = pRightCol_->GetScale();
	// 좌표가 겹치는지
	// -----.-----
	//         -----.-----
	// X축 좌표의 차이값이 둘의 size 합보다 더 까까워야 함
	if (abs(vRightPos.x - vLeftPos.x) < (vLeftScale.x + vRightScale.x) / 2.f
		&& abs(vRightPos.y - vLeftPos.y) < (vLeftScale.y + vRightScale.y) / 2.f)
		return true;

	return false;
}

