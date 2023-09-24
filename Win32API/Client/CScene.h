#pragma once
// Scene�� ��� ����
// ��� Scene���� ���������� �������� ��
// ������Ʈ�� �׷� ������ ����

// ���� ���� �� ������ Ÿ�Ը� ��밡��
// �������� �ּ� ũ��� �˱� ����
// ��� ������ �ߺ��� ������ ���� ����
class CObject;

// Object �����͸� �����ϴ� ����
// �ڽĵ��� ������ Ÿ�Ե��� ��� �θ� Ÿ������ ���� �� ����
// Enum ������ �� �׷쿡 ���� ����

class CScene
{
public:
	CScene();
	virtual ~CScene();


	// ���� ���� �Լ� CScene�� ��ü ���� �Ұ�
	virtual void Enter() = 0; // �ش� Scene �� ����
	virtual void Exit() = 0;  // �ش� Scene ���� ����

	void update();
	void render(HDC dc_);

	void SetName(const wstring& strName_) { m_strName = strName_; }
	const wstring& GetName() { return m_strName; }

protected:
	// ����� �Լ��� �ۼ��ϸ� �ζ���
	// �Լ� ȣ�� ����� ����
	void AddObject(CObject* pObj_, GROUP_TYPE type_)
	{
		m_vecObj[(UINT)type_].push_back(pObj_);
	}

private:
	wstring				m_strName; // Scene �̸�
	vector<CObject*>	m_vecObj[(UINT)GROUP_TYPE::END]; // ������Ʈ�� ����, ������ ���͸� �׷� ������ŭ ����
};

