#pragma once

// Singleton Mecro
#define SINGLE(type)  public:							\
						static type* GetInstance()		\
						{								\
						static type mgr;				\
							return &mgr;				\
						}								\
					private:							\
						type();							\
						~type();


#define DT CTimeManager::GetInstance()->GetDeltaTime()
#define fDT CTimeManager::GetInstance()->GetfDeltaTime()
