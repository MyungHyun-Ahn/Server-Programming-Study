#pragma once

// �̱��� ����
// ��ü�� ������ 1���� ����
// ��𼭵� ���� ���� ����

// static ���� ��� (������ ������ ����) -> �ݵ�� �ʱ�ȭ ���־�� ��
// Ŭ���� ���ο� �����ص� Ŭ������ ũ�⿡ ������ ���� ����
class CCore
{


public:

	// static ���� ��� �Լ�, ��ü ���� ȣ�� ����, this�� ���� (��� ���� �Ұ�), ���� ����� ���� ����
	static CCore* GetInstance()
	{
		// ���� ȣ�� �� ���
		if (pCore == nullptr)
		{
			pCore = new CCore;
		}
		// 2�� �̻� ȣ��� ��� - �ƹ��͵� �� �ʿ� ����


		return pCore;
	}

	static void Release()
	{
		if (pCore != nullptr)
		{
			delete pCore;
			pCore = nullptr;
		}
	}

private:
	static CCore* pCore;

	CCore() = default;
	~CCore() = default;
};

