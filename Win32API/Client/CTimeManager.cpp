#include "pch.h"
#include "CTimeManager.h"
#include "CCore.h"

CTimeManager::CTimeManager() : m_llCurCount{}
							, m_llPrevCount{}
							, m_llFrequency{}
							, m_dDeltaTime(0.1)
							, m_dAcc(0.)
							, m_iCallCount(0)
{

}

CTimeManager::~CTimeManager()
{

}

void CTimeManager::init()
{
	// 현재 카운트
	QueryPerformanceCounter(&m_llPrevCount);

	// 초당 카운트 횟수
	QueryPerformanceFrequency(&m_llFrequency);
}

void CTimeManager::update()
{
	QueryPerformanceCounter(&m_llCurCount);

	// 프레임마다 차이값
	m_dDeltaTime = (double)(m_llCurCount.QuadPart - m_llPrevCount.QuadPart) / (double)m_llFrequency.QuadPart;
	m_llPrevCount = m_llCurCount; // 다음 계산을 위하여 갱신
}

void CTimeManager::render()
{
	++m_iCallCount;
	m_dAcc += m_dDeltaTime; // DT 누적

	if (m_dAcc >= 1.)
	{
		m_iFPS = m_iCallCount;

		m_dAcc = 0.;
		m_iCallCount = 0; // fps

		wchar_t szBuffer[255] = {};
		swprintf_s(szBuffer, L"FPS : %d, DT : %f", m_iFPS, m_dDeltaTime);
		SetWindowText(CCore::GetInstance()->GetMainHandle(), szBuffer);
	}
}
