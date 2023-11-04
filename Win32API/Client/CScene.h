#pragma once
// Scene도 상속 가능
// 모든 Scene들이 공통적으로 가져야할 것
// 오브젝트를 그룹 단위로 구분

// 전방 선언 시 포인터 타입만 사용가능
// 포인터의 주소 크기는 알기 때문
// 헤더 파일의 중복된 선언을 막기 위함
class CObject;

// Object 포인터를 관리하는 이유
// 자식들의 포인터 타입들은 모두 부모 타입으로 받을 수 있음
// Enum 값으로 각 그룹에 접근 가능

class CScene
{
public:
	CScene();
	virtual ~CScene();


	// 순수 가상 함수 CScene의 객체 생성 불가
	virtual void	Enter() = 0; // 해당 Scene 에 진입
	virtual void	Exit() = 0;  // 해당 Scene 에서 퇴장

	void			update();
	void			finalUpdate();
	void			render(HDC dc_);

	void			SetName(const wstring& strName_) { m_strName = strName_; }
	const wstring&	GetName() { return m_strName; }

	// 헤더에 함수를 작성하면 인라인
	// 함수 호출 비용이 없음
	void			AddObject(CObject* pObj_, GROUP_TYPE type_) { m_vecObj[(UINT)type_].push_back(pObj_); }

	// 벡터를 반환하면 복사해서 반환
	// 참조 타입으로 줌으로 원본을 그대로 전달
	// 윈본을 수정하면 안됨, 충돌 체크에만 사용
	const vector<CObject*>& GetGroupObject(GROUP_TYPE eType_) { return m_vecObj[(UINT)eType_]; }

private:
	wstring				m_strName; // Scene 이름
	vector<CObject*>	m_vecObj[(UINT)GROUP_TYPE::END]; // 오브젝트를 저장, 관리할 벡터를 그룹 개수만큼 선언
};

