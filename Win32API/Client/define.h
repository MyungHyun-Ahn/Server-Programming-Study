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