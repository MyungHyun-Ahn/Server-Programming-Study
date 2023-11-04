#include "pch.h"
#include "CCollisionManager.h"
#include "CSceneManager.h"
#include "CScene.h"
#include "CObject.h"
#include "CCollider.h"

// union
// ����ü�� ���
// ���Ͽ��� Ư¡ - ����ü�� ��

// ���Ͽ��� ���� ū �������� Ÿ���� �������� ���� ������ ������
//union Test
//{
//	int a;
//	float b;
//};
// 4����Ʈ
// ���� Ÿ������ �θ��� a, �Ǽ� Ÿ������ �θ��� b
 
// LARGE_INTEGER 8 ����Ʈ ����
// [   LowPart  |   HighPart  ] QuadPart; 
// QuadPart �� ����ϸ� ��ü�� 8����Ʈ ����
// LowPart 4����Ʈ, HighPart 4����Ʈ


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
	// üũ�� ���� �浹 �˻�
	for (UINT iRow = 0; iRow < (UINT)GROUP_TYPE::END; ++iRow)
	{
		// �ߺ��Ǵ� �κ��� �˻����� ���� ����
		for (UINT iCol = iRow; iCol < (UINT)GROUP_TYPE::END; ++iCol)
		{
			if (m_arrCheck[iRow] & (1 << iCol))
			{
				// üũ�� �ڸ� �˻�
				// ���� Scene�� �׷� ��ü�� �����ͼ� ��
				CollisionUpdateGroup((GROUP_TYPE)iRow, (GROUP_TYPE)iCol);
			}
		}
	}
}

void CCollisionManager::CheckGroup(GROUP_TYPE eLeft_, GROUP_TYPE eRight_)
{
	// �� ���� ���� �׷� Ÿ���� ������
	// ū ���� ��(��Ʈ)�� ���

	UINT iRow = (UINT)eLeft_;
	UINT iCol = (UINT)eRight_;

	if (iCol < iRow)
	{
		std::swap(iCol, iRow);
	}

	// 1�� �÷���ŭ ���� ����Ʈ
	// �̹� ���� 1�̸�?
	if (m_arrCheck[iRow] & (1 << iCol))
	{
		m_arrCheck[iRow] &= ~(1 << iCol); // ���ϴ� ���� ��Ʈ�� ����
	}
	else
	{
		m_arrCheck[iRow] |= (1 << iCol); // ���ϴ� ���� üũ
	}
}

// �׻� ���� ���� ����
void CCollisionManager::CollisionUpdateGroup(GROUP_TYPE eLeft_, GROUP_TYPE eRight_)
{
	CScene* pCurScene = CSceneManager::GetInstance()->GetCurScene();

	// ����? - �ǵ����� ��߳��� �κ�
	// ���� ���͸� �����ؼ� �޾ƿ���. �ٵ� ���� ������ ���� ���� = ���� ����
	// ���� ���� const vec& ������ �޾ƾ� ��
	const vector<CObject*>& vecLeft = pCurScene->GetGroupObject(eLeft_);
	const vector<CObject*>& vecRight = pCurScene->GetGroupObject(eRight_);

	map<ULONGLONG, bool>::iterator it;

	for (size_t i = 0; i < vecLeft.size(); ++i)
	{
		if (vecLeft[i]->GetCollider() == nullptr) // �浹ü�� ���ٸ� �ǳʶٱ�
			continue;

		for (size_t j = 0; j < vecRight.size(); ++j)
		{
			// �浹ü�� ���ų�, �ڱ� �ڽŰ� �浹�� ���
			if (vecRight[j]->GetCollider() == nullptr
				|| vecLeft[i] == vecRight[j])
				continue;

			CCollider* pLeftCol = vecLeft[i]->GetCollider();
			CCollider* pRightCol = vecRight[j]->GetCollider();


			// �� �浹ü ���� ID ����
			COLLIDER_ID ID;
			ID.iLeft_id = pLeftCol->GetID();
			ID.iRight_id = pRightCol->GetID();
			// ID.ID = iLeft + iRight // �� �浹ü�����ιۿ� ���� �� ���� ��

			it = m_mapColInfo.find(ID.ID);
			if (it == m_mapColInfo.end())
			{
				// ���� ����
				// �˻縦 �ѹ��̶� �� ��쿡�� ������ ����ؾ� ��
				// �� ��� ���¶�� ���
				m_mapColInfo.insert(std::make_pair(ID.ID, false));
				it = m_mapColInfo.find(ID.ID);
			}

			// �浹 �˻�
			// �浹 �̺�Ʈ - Key�Է� �̺�Ʈ�� ����ϰ� ����� �־�� ��
			if (IsCollision(pLeftCol, pRightCol))
			{
				// ���� �浹 ���̴�.

				if (it->second)
				{
					// �������� �浹�ϰ� �־��� - ing~
					pLeftCol->OnCollision(pRightCol);
					pRightCol->OnCollision(pLeftCol);

				}
				else
				{
					// �������� �浹���� �ʾҴ�. - ù ����
					pLeftCol->OnCollisionEnter(pRightCol);
					pRightCol->OnCollisionEnter(pLeftCol);
					it->second = true;
				}

			}
			else // �浹 X
			{
				// ���� �浹���� �ʰ� �ִ�.
				if (it->second)
				{
					// �������� �浹�ϰ� �־���.
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
	// finalPos�� �����;� ��
	Vec2 vLeftPos = pLeftCol_->GetFinalPos();
	Vec2 vLeftScale = pLeftCol_->GetScale();

	Vec2 vRightPos = pRightCol_->GetFinalPos();
	Vec2 vRightScale = pRightCol_->GetScale();
	// ��ǥ�� ��ġ����
	// -----.-----
	//         -----.-----
	// X�� ��ǥ�� ���̰��� ���� size �պ��� �� ������ ��
	if (abs(vRightPos.x - vLeftPos.x) < (vLeftScale.x + vRightScale.x) / 2.f
		&& abs(vRightPos.y - vLeftPos.y) < (vLeftScale.y + vRightScale.y) / 2.f)
		return true;

	return false;
}

