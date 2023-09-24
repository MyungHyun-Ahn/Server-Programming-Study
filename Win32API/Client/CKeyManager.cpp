#include "pch.h"
#include "CKeyManager.h"

#include "CCore.h"

int g_arrVK[(int)KEY::LAST] =
{
	VK_LEFT, // LEFT,
	VK_RIGHT, // RIGHT,
	VK_UP, // UP,
	VK_DOWN, // DOWN,
	// �����ڴ� �빮���� �ƽ�Ű
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
	// KEY ���� ä���ֱ�
	for (int i = 0; i < (int)KEY::LAST; ++i)
	{
		m_vecKey.push_back(stKeyInfo{ KEY_STATE::NONE, false });
	}

	// m_vecKey[(int)KEY::LEFT].m_state; - �̷��� �����ؼ� ���
}

// ������ �� Ű ���� üũ
void CKeyManager::update()
{
	// ������ ��Ŀ�� �˾Ƴ���
	HWND hMainWnd = CCore::GetInstance()->GetMainHandle();
	HWND hWnd = GetFocus(); // ���� ��Ŀ�̵� ������ �ڵ鰪�� �˷���
	// ���� �����찡 ��Ŀ�� �Ǿ��ִ���?
	// ���α׷� ���� ������ �� 1���� ? 
	// Focus �� Null ������ üũ

	if (hWnd != nullptr)
	{
		for (int i = 0; i < (int)KEY::LAST; ++i)
		{
			// VK_LEFT : 0x25
			// ����ü���� ��Ģ�� ����.
			// �迭�� ��Ī
			if (GetAsyncKeyState(g_arrVK[i]) & 0x8000)
			{
				if (m_vecKey[i].m_prevPush)
				{
					// �������� �����־���.
					m_vecKey[i].m_state = KEY_STATE::HOLD;
				}
				else
				{
					// ������ �������� �ʾҴ�.
					m_vecKey[i].m_state = KEY_STATE::TAP;
				}
				m_vecKey[i].m_prevPush = true;
			}
			else // Ű�� �������� �ʴ�.
			{
				if (m_vecKey[i].m_prevPush)
				{
					// �������� �����־���.
					m_vecKey[i].m_state = KEY_STATE::AWAY;
				}
				else
				{
					// �������� �ȴ����־���.
					m_vecKey[i].m_state = KEY_STATE::NONE;
				}
				m_vecKey[i].m_prevPush = false;
			}
		}
	}
	else // ������ ��Ŀ�� ���� ����
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
