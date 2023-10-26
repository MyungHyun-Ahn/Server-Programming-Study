#pragma once

// 시간 동기화를 하는 방법
// 100 frame / s
// 이동량 X 1 / frame
// 1 / frame 의미? -> 프레임 당 시간 (Delta Time)
// 들쭉날쭉 할 수 있다.
// Delta Time 은 계속 계산이 되어야 한다.


class CTimeManager
{
	SINGLE(CTimeManager);

public:
	void		init();
	void		update();
	void		render();

	double		GetDeltaTime() { return m_dDeltaTime; }
	float		GetfDeltaTime() { return (float)m_dDeltaTime; }

private:
	LARGE_INTEGER	m_llCurCount;
	LARGE_INTEGER	m_llPrevCount;
	LARGE_INTEGER	m_llFrequency;

	double			m_dDeltaTime; // 프레임 간의 시간 값
	double			m_dAcc;		  // 1초 체크를 위한 누적 시간

	UINT			m_iCallCount; // 초당 호출 횟수
	UINT			m_iFPS;
};

