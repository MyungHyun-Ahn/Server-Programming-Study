#include "pch.h"
#include "CPlayer.h"
#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CMissile.h"
#include "CScene.h"
#include "CSceneManager.h"
#include "CCurvedMissile.h"
#include "CTexture.h"
#include "CPathManager.h"
#include "CResourceManager.h"
#include "CCollider.h"

// ���ҽ� �Ŵ����� ���ٸ� ��� ���ҽ��� �ε��Ǵ��� ������ �Ұ���

CPlayer::CPlayer() : m_pTex(nullptr)
{
	// Texture �ε�
	m_pTex = CResourceManager::GetInstance()->LoadTexture(L"PlayerTex", L"texture\\Player.bmp");

	CreateCollider();

	GetCollider()->SetOffsetPos(Vec2(0.f, 12.f));
	GetCollider()->SetScale(Vec2(20.f, 30.f));

	//m_pTex = new CTexture;
	//wstring strFilepath = CPathManager::GetInstance()->GetContentPath();

	//// �̹��� ������ 24��Ʈ bmp �����̾�� ��
	//strFilepath += L"texture\\Player.bmp";
	//m_pTex->Load(strFilepath);
}

CPlayer::~CPlayer()
{
	/*if (m_pTex != nullptr)
	{
		delete m_pTex;
		m_pTex = nullptr;
	}*/
}

void CPlayer::update()
{
	// ���� ����
	Vec2 vPos = GetPos();

	if (KEY_HOLD(KEY::W))
	{
		vPos.y -= 200.f * fDT;
	}

	if (KEY_HOLD(KEY::S))
	{
		vPos.y += 200.f * fDT;
	}

	if (KEY_HOLD(KEY::A))
	{
		vPos.x -= 200.f * fDT;
	}

	if (KEY_HOLD(KEY::D))
	{
		vPos.x += 200.f * fDT;
	}

	if (KEY_TAP(KEY::SPACE))
	{
		CreateMissile();
	}

	SetPos(vPos);
}

void CPlayer::render(HDC dc_)
{
	// ���⼭ texture�� �����ϴ� ���� �ƴ� Manager ��ü�� ���� ����
	int iWidth = (int)m_pTex->GetWidth();
	int iHeight = (int)m_pTex->GetHeight();

	Vec2 vPos = GetPos();

	// �� ���
	(int)(vPos.x - (float)(iWidth / 2));
	(int)(vPos.y - (float)(iHeight / 2));

	/*BitBlt(dc_
		, (int)(vPos.x - (float)(iWidth / 2))
		, (int)(vPos.y - (float)(iHeight / 2))
		, iWidth, iHeight
		, m_pTex->GetDC()
		, 0, 0
		, SRCCOPY);*/

	// ������ ������ ����ȭ - �ַ� ����Ÿ, 255, 0, 255
	TransparentBlt(dc_
		, (int)(vPos.x - (float)(iWidth / 2))
		, (int)(vPos.y - (float)(iHeight / 2))
		, iWidth, iHeight
		, m_pTex->GetDC()
		, 0, 0, iWidth, iHeight
		, RGB(255, 0, 255));

	ComponentRender(dc_);
}

void CPlayer::CreateMissile()
{
	Vec2 vMissilePos = GetPos();
	vMissilePos.y -= GetScale().y / 2.f;

	CMissile* pMissile = new CMissile;

	pMissile->SetPos(vMissilePos);
	pMissile->SetScale(Vec2(GetScale().x / 4.f, GetScale().y / 4.f));
	pMissile->SetDir(Vec2(-1.f, -1.f));

	CScene* pCurScene = CSceneManager::GetInstance()->GetCurScene();

	pCurScene->AddObject(pMissile, GROUP_TYPE::DEFAULT);
}

void CPlayer::CreateCurvedMissile()
{
	Vec2 vMissilePos = GetPos();
	vMissilePos.y -= GetScale().y / 2.f;

	CCurvedMissile* pMissile = new CCurvedMissile;

	pMissile->SetPos(vMissilePos);
	pMissile->SetScale(Vec2(GetScale().x / 4.f, GetScale().y / 4.f));

	// �̻��� �߻� �� ���� ��ǥ
	pMissile->SetCenter(GetPos());
	pMissile->SetDir(true);

	CScene* pCurScene = CSceneManager::GetInstance()->GetCurScene();

	pCurScene->AddObject(pMissile, GROUP_TYPE::DEFAULT);
}