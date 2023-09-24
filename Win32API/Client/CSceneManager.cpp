#include "pch.h"
#include "CSceneManager.h"
#include "CSceneStart.h"

CSceneManager::CSceneManager()
	: m_arrScene{}
	, m_pCurScene(nullptr)
{

}

CSceneManager::~CSceneManager()
{
	// Scene 전부 삭제
	// Scene의 소멸자를 호출 - virtual을 사용하지 않으면 자식의 소멸자가 호출되지 않음
	for (UINT i = 0; i < (UINT)SCENE_TYPE::END; ++i)
	{
		// 아직 모든 씬을 지정한 것이 아님
		if (m_arrScene[i] != nullptr)
		{
			delete m_arrScene[i];
		}
	}
}

void CSceneManager::init()
{
	// Scene 생성
	m_arrScene[(UINT)SCENE_TYPE::START] = new CSceneStart;
	// m_arrScene[(UINT)SCENE_TYPE::TOOL] = new CSceneTool;
	// m_arrScene[(UINT)SCENE_TYPE::STAGE_01] = new CSceneStage01;
	// m_arrScene[(UINT)SCENE_TYPE::STAGE_02] = new CSceneStage02;

	// 현재 씬 지정
	m_pCurScene = m_arrScene[(UINT)SCENE_TYPE::START];
	m_pCurScene->Enter();
}

void CSceneManager::update()
{
	m_pCurScene->update();
}

void CSceneManager::render(HDC dc_)
{
	m_pCurScene->render(dc_);
}
