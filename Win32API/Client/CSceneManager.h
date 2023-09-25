#pragma once

class CScene;

class CSceneManager
{
	SINGLE(CSceneManager);

public:
	void init();

	void update();
	void render(HDC dc_);

	CScene* GetCurScene() { return m_pCurScene; }

private:
	CScene* m_arrScene[(UINT)SCENE_TYPE::END];		// ��� �� ���
	CScene* m_pCurScene;							// ���� �� ���
};

