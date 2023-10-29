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

// 리소스 매니저가 없다면 어디서 리소스가 로딩되는지 추적이 불가능

CPlayer::CPlayer() : m_pTex(nullptr)
{
	// Texture 로딩
	m_pTex = CResourceManager::GetInstance()->LoadTexture(L"PlayerTex", L"texture\\Player.bmp");

	CreateCollider();

	GetCollider()->SetOffsetPos(Vec2(0.f, 12.f));
	GetCollider()->SetScale(Vec2(20.f, 30.f));

	//m_pTex = new CTexture;
	//wstring strFilepath = CPathManager::GetInstance()->GetContentPath();

	//// 이미지 파일은 24비트 bmp 파일이어야 함
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
	// 지역 변수
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
	// 여기서 texture를 관리하는 것이 아닌 Manager 객체를 통해 관리
	int iWidth = (int)m_pTex->GetWidth();
	int iHeight = (int)m_pTex->GetHeight();

	Vec2 vPos = GetPos();

	// 좌 상단
	(int)(vPos.x - (float)(iWidth / 2));
	(int)(vPos.y - (float)(iHeight / 2));

	/*BitBlt(dc_
		, (int)(vPos.x - (float)(iWidth / 2))
		, (int)(vPos.y - (float)(iHeight / 2))
		, iWidth, iHeight
		, m_pTex->GetDC()
		, 0, 0
		, SRCCOPY);*/

	// 설정한 색상을 투명화 - 주로 마젠타, 255, 0, 255
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

	// 미사일 발사 시 현재 좌표
	pMissile->SetCenter(GetPos());
	pMissile->SetDir(true);

	CScene* pCurScene = CSceneManager::GetInstance()->GetCurScene();

	pCurScene->AddObject(pMissile, GROUP_TYPE::DEFAULT);
}