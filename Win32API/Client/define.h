#pragma once

// Singleton Mecro
// 2번 이상 호출되면 초기화는 무시
#define SINGLE(type)									\
					public:								\
						static type* GetInstance()		\
						{								\
							static type mgr;			\
							return &mgr;				\
						}								\
					private:							\
						type();							\
						~type();


#define DT CTimeManager::GetInstance()->GetDeltaTime()
#define fDT CTimeManager::GetInstance()->GetfDeltaTime()

#define KEY_CHECK(key, state) CKeyManager::GetInstance()->GetKeyState(key) == state
#define KEY_HOLD(key) KEY_CHECK(key, KEY_STATE::HOLD)
#define KEY_TAP(key) KEY_CHECK(key, KEY_STATE::TAP)
#define KEY_AWAY(key) KEY_CHECK(key, KEY_STATE::AWAY)
#define KEY_NONE(key) KEY_CHECK(key, KEY_STATE::NONE)

#define PI 3.1415926

// 쓸 만큼만 만들면 됨
enum class GROUP_TYPE
{
	DEFAULT,	// 0

	PLAYER,		// 1

	MISSILE,	// 2
	 
	MONSTER,	// 3

	END = 32,
};

enum class SCENE_TYPE
{
	TOOL,

	START,

	STAGE_01,
	STAGE_02,

	END,
};