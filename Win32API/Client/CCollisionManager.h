#pragma once
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
	UINT m_arrCheck[(UINT)GROUP_TYPE::END];
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
