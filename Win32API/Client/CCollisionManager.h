#pragma once

class CCollider;

// 8����Ʈ
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
	UINT m_arrCheck[(UINT)GROUP_TYPE::END]; // �׷� �� �浹 üũ ��Ʈ����
	map<ULONGLONG, bool> m_mapColInfo;
	// �浹ü ���� ���� ������ �浹 ���� - ���� ���� ����.
	// ���� ������ ��Ƶΰ� ������ Ž���ϴ� �ڷᱸ��. map, hash table, binary search tree
	// �˻� ����, �� �浹ü
	// Ű���� �� �浹ü - ���� ��ĥ �� ���� ID: ������ ID���� ���� 
};

// �׷� ���� �� 32��
/* 4 7 (�� ���� ���� ��)
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
