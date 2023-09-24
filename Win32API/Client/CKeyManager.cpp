#include "pch.h"
#include "CKeyManager.h"

#include "CCore.h"

int g_arrVK[(int)KEY::LAST] =
{
	VK_LEFT, // LEFT,
	VK_RIGHT, // RIGHT,
	VK_UP, // UP,
	VK_DOWN, // DOWN,
	// 영문자는 대문자의 아스키
	'Q',
	'W',
	'E',
	'R',
	'T',
	'Y',
	'U',
	'I',
	'O',
	'P',
	'A',
	'S',
	'D',
	'F',
	'G',
	'Z',
	'X',
	'C',
	'V',
	'B',

	VK_MENU,
	VK_CONTROL,
	VK_LSHIFT,
	VK_SPACE,
	VK_RETURN,
	VK_ESCAPE,
};

CKeyManager::CKeyManager() {}

CKeyManager::~CKeyManager() {}

void CKeyManager::init()
{
	// KEY 벡터 채워주기
	for (int i = 0; i < (int)KEY::LAST; ++i)
	{
		m_vecKey.push_back(stKeyInfo{ KEY_STATE::NONE, false });
	}

	// m_vecKey[(int)KEY::LEFT].m_state; - 이렇게 접근해서 사용
}

// 프레임 당 키 상태 체크
void CKeyManager::update()
{
	// 윈도우 포커싱 알아내기
	HWND hMainWnd = CCore::GetInstance()->GetMainHandle();
	HWND hWnd = GetFocus(); // 현재 포커싱된 윈도우 핸들값을 알려줌
	// 메인 윈도우가 포커싱 되어있는지?
	// 프로그램 내부 윈도우 중 1개라도 ? 
	// Focus 가 Null 인지만 체크

	if (hWnd != nullptr)
	{
		for (int i = 0; i < (int)KEY::LAST; ++i)
		{
			// VK_LEFT : 0x25
			// 구조체랑은 규칙이 없다.
			// 배열에 매칭
			if (GetAsyncKeyState(g_arrVK[i]) & 0x8000)
			{
				if (m_vecKey[i].m_prevPush)
				{
					// 이전에도 눌려있었다.
					m_vecKey[i].m_state = KEY_STATE::HOLD;
				}
				else
				{
					// 이전에 눌려있지 않았다.
					m_vecKey[i].m_state = KEY_STATE::TAP;
				}
				m_vecKey[i].m_prevPush = true;
			}
			else // 키가 눌려있지 않다.
			{
				if (m_vecKey[i].m_prevPush)
				{
					// 이전에는 눌려있었다.
					m_vecKey[i].m_state = KEY_STATE::AWAY;
				}
				else
				{
					// 이전에도 안눌려있었다.
					m_vecKey[i].m_state = KEY_STATE::NONE;
				}
				m_vecKey[i].m_prevPush = false;
			}
		}
	}
	else // 윈도우 포커싱 해제 상태
	{
		for (int i = 0; i < (int)KEY::LAST; ++i)
		{
			m_vecKey[i].m_prevPush = false;

			if (m_vecKey[i].m_state == KEY_STATE::TAP || m_vecKey[i].m_state == KEY_STATE::HOLD)
			{
				m_vecKey[i].m_state = KEY_STATE::AWAY;
			}
			else if (m_vecKey[i].m_state == KEY_STATE::AWAY)
			{
				m_vecKey[i].m_state = KEY_STATE::NONE;
			}
		}
	}
}
