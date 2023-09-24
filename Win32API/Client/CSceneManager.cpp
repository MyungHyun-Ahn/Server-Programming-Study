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
	// Scene ���� ����
	// Scene�� �Ҹ��ڸ� ȣ�� - virtual�� ������� ������ �ڽ��� �Ҹ��ڰ� ȣ����� ����
	for (UINT i = 0; i < (UINT)SCENE_TYPE::END; ++i)
	{
		// ���� ��� ���� ������ ���� �ƴ�
		if (m_arrScene[i] != nullptr)
		{
			delete m_arrScene[i];
		}
	}
}

void CSceneManager::init()
{
	// Scene ����
	m_arrScene[(UINT)SCENE_TYPE::START] = new CSceneStart;
	// m_arrScene[(UINT)SCENE_TYPE::TOOL] = new CSceneTool;
	// m_arrScene[(UINT)SCENE_TYPE::STAGE_01] = new CSceneStage01;
	// m_arrScene[(UINT)SCENE_TYPE::STAGE_02] = new CSceneStage02;

	// ���� �� ����
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
