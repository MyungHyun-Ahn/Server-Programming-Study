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
	// ���� ī��Ʈ
	QueryPerformanceCounter(&m_llPrevCount);

	// �ʴ� ī��Ʈ Ƚ��
	QueryPerformanceFrequency(&m_llFrequency);
}

void CTimeManager::update()
{
	QueryPerformanceCounter(&m_llCurCount);

	// �����Ӹ��� ���̰�
	m_dDeltaTime = (double)(m_llCurCount.QuadPart - m_llPrevCount.QuadPart) / (double)m_llFrequency.QuadPart;
	m_llPrevCount = m_llCurCount; // ���� ����� ���Ͽ� ����
}

void CTimeManager::render()
{
	++m_iCallCount;
	m_dAcc += m_dDeltaTime; // DT ����

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
