#pragma once

/*
키 매니저가 없으면
기존 update 함수에서 키를 감지해야 그때서야 수행할 수 있음
매 순간 프로그램은 update를 거치고 모든 update가 끝나면 render를 수행
1 frame이 걸린 시간 -> Delta Time
컴퓨터는 지정된 작업을 하나하나 처리
실제로는 DT 라는 시간이 흐를 때 해야하는 모든 일들을 1개씩 처리하는데
보고 있는 화면 기준으로는 동시에 진행되는 것으로 보임
- 사진을 찍듯이
타이밍을 어긋나서 키를 누른 처리를 못 받을 수도 있음

Key Manager 가 필요한 이유 - Frame 동기화
키가 눌린 상태를 체크해두고 이것을 받아서 처리

프레임 동기화
 - 동일 프레임 내에서 같은 키에 대해, 동일한 이벤트를 가져간다.

프레임이 빠르게 돌고 있으므로 한번 키를 눌렀다 땠을 뿐인데도 여러번 눌린 처리가 될 수 있음
+ up event

키 입력 이벤트를 입력 처리
 - tap, hold, away


상태의 흐름
Tap -> Away -> None
Hold -> Away -> None

Away -> None
None -> Npne

*/

// 키 입력 이벤트 정의
enum class KEY_STATE
{
	TAP,		// 누른 시점
	HOLD,		// 누르고 있는
	AWAY,		// 뗀 시점
	NONE,		// 눌리지 않고, 이전에도 눌리지 않은 상태
};

// 지원하는 키의 종류
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

	LAST, // enum의 끝을 알림
};

struct stKeyInfo
{
	KEY_STATE	m_state;	// 상태값
	bool		m_prevPush;		// 이전 프레임에 눌렸는지
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

