#pragma once

class CCollider;

// 8바이트
// [  left  |  Right  ]
union COLLIDER_ID
{
	struct 
	{
		UINT iLeft_id;
		UINT iRight_id;
	};
	ULONGLONG ID;
};

class CCollisionManager
{
	SINGLE(CCollisionManager)

public:
	void init();
	void update();
	void CheckGroup(GROUP_TYPE eLeft_, GROUP_TYPE eRight_);
	void Reset()
	{
		memset(m_arrCheck, 0, sizeof(UINT) * (UINT)GROUP_TYPE::END);
	}

private:
	void CollisionUpdateGroup(GROUP_TYPE eLeft_, GROUP_TYPE eRight_);
	bool IsCollision(CCollider* pLeftCol_, CCollider* pRightCol_);

private:
	UINT m_arrCheck[(UINT)GROUP_TYPE::END]; // 그룹 간 충돌 체크 매트릭스
	map<ULONGLONG, bool> m_mapColInfo;
	// 충돌체 간의 이전 프레임 충돌 정보 - 가지 수가 많다.
	// 조합 정보를 모아두고 빠르게 탐색하는 자료구조. map, hash table, binary search tree
	// 검색 조건, 두 충돌체
	// 키값이 두 충돌체 - 서로 겹칠 수 없는 ID: 고유한 ID값의 조합 
};

// 그룹 개수 총 32개
/* 4 7 (더 작은 수가 행)
  31 ... 3 2 1 0
0
1
2
3
4
5
6
.
.
31


*/
