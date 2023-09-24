#pragma once

// Singleton Mecro
// 2�� �̻� ȣ��Ǹ� �ʱ�ȭ�� ����
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


// �� ��ŭ�� ����� ��
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