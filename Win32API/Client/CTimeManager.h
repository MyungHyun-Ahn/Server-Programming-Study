#pragma once

// �ð� ����ȭ�� �ϴ� ���
// 100 frame / s
// �̵��� X 1 / frame
// 1 / frame �ǹ�? -> ������ �� �ð� (Delta Time)
// ���߳��� �� �� �ִ�.
// Delta Time �� ��� ����� �Ǿ�� �Ѵ�.


class CTimeManager
{
	SINGLE(CTimeManager);

public:
	void		init();
	void		update();

	double		GetDeltaTime() { return m_dDeltaTime; }
	float		GetfDeltaTime() { return (float)m_dDeltaTime; }

private:
	LARGE_INTEGER	m_llCurCount;
	LARGE_INTEGER	m_llPrevCount;
	LARGE_INTEGER	m_llFrequency;

	double			m_dDeltaTime; // ������ ���� �ð� ��
	double			m_dAcc;		  // 1�� üũ�� ���� ���� �ð�

	UINT			m_iCallCount; // �ʴ� ȣ�� Ƚ��
	UINT			m_iFPS;
};

