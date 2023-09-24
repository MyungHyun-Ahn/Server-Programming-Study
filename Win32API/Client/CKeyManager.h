#pragma once

/*
Ű �Ŵ����� ������
���� update �Լ����� Ű�� �����ؾ� �׶����� ������ �� ����
�� ���� ���α׷��� update�� ��ġ�� ��� update�� ������ render�� ����
1 frame�� �ɸ� �ð� -> Delta Time
��ǻ�ʹ� ������ �۾��� �ϳ��ϳ� ó��
�����δ� DT ��� �ð��� �带 �� �ؾ��ϴ� ��� �ϵ��� 1���� ó���ϴµ�
���� �ִ� ȭ�� �������δ� ���ÿ� ����Ǵ� ������ ����
- ������ �����
Ÿ�̹��� ��߳��� Ű�� ���� ó���� �� ���� ���� ����

Key Manager �� �ʿ��� ���� - Frame ����ȭ
Ű�� ���� ���¸� üũ�صΰ� �̰��� �޾Ƽ� ó��

������ ����ȭ
 - ���� ������ ������ ���� Ű�� ����, ������ �̺�Ʈ�� ��������.

�������� ������ ���� �����Ƿ� �ѹ� Ű�� ������ ���� ���ε��� ������ ���� ó���� �� �� ����
+ up event

Ű �Է� �̺�Ʈ�� �Է� ó��
 - tap, hold, away


������ �帧
Tap -> Away -> None
Hold -> Away -> None

Away -> None
None -> Npne

*/

// Ű �Է� �̺�Ʈ ����
enum class KEY_STATE
{
	TAP,		// ���� ����
	HOLD,		// ������ �ִ�
	AWAY,		// �� ����
	NONE,		// ������ �ʰ�, �������� ������ ���� ����
};

// �����ϴ� Ű�� ����
enum class KEY
{
	LEFT,
	RIGHT,
	UP,
	DOWN,

	Q, W, E, R, T, Y, U, I, O, P,
	A, S, D, F, G,
	Z, X, C, V, B,

	ALT,
	CTRL,
	LSHIFT,
	SPACE,
	ENTER,
	ESC,

	LAST, // enum�� ���� �˸�
};

struct stKeyInfo
{
	KEY_STATE	m_state;	// ���°�
	bool		m_prevPush;		// ���� �����ӿ� ���ȴ���
};

class CKeyManager
{
	SINGLE(CKeyManager);

public:
	void init();
	void update();

	KEY_STATE GetKeyState(KEY key_) { return m_vecKey[(int)key_].m_state; }

private:
	vector<stKeyInfo> m_vecKey;
};

