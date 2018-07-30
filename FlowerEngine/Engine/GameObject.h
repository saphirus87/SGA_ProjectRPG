/*
송현국
H.P		: 010-3592-5921
E-Mail	: hkn10004@naver.com

NOTE :
이 코드는 직접 설계하고 작성한 코드입니다. 이론은 GAME PROGRAMMING GEMS 6권이나 인터넷을 참고하세요.

설명 :
우리가 현실에서 볼 수 있는 모든 생명체나 물체 즉, 객체(Object)를 게임상에서 표현한 것.
그러므로 게임 객체(GameObject)라고 한다.

예를 들어 현실에서 지형(Terrain)을 게임상에서 표현하면 GameObject가 되고 그 이름은 Terrain이 된다.
예를 들어 현실에서 인물(Character)를 게임상에서 표현함면 GameObject가 되고 그 이름은 Character이다.
물건(Item) 또는 식탁, 의자, 맥주잔, 무기, 검, 권총등 모든 물체를 GameObject로 만들 수 있다.

모든 게임 객체(GameObject)에는 구성요소(Component)가 있다.

즉, 인물(Chracter) 게임 객체에 컨트롤 구성요소(CharacterControl Component)를 추가(Add)하면 그 인물(Character)를 컨트롤 할 수 있다.

자세한건 역시 책이나 인터넷을 참고하세요. 개발자들끼리 함께 물어보면서 개발해도 됩니다.
*/
#pragma once
#include "stdafx.h"

typedef std::map<CString, Component*> MComponent;
typedef MComponent::value_type MComPair;
typedef MComponent::iterator IterGOCom;

typedef std::multimap<CString, GameObject*> MGameObject;
typedef MGameObject::value_type MGOPair;
typedef MGameObject::iterator IterGO;

class GameObject
{
public:
	GameObject(CString szName);
	virtual ~GameObject(void);

	// 구성요소를 추가합니다.
	void AddComponent(Component* pComponent);
	void RemoveComponent(CString& szName);

	void AddChild(GameObject* pGO);

	// 구성요소를 반환합니다.
	Component* GetComponent(const CString& familyID);
	
	// 모든 구성요소를 파괴합니다.
	void CleanUpComponent();
	
	// 이름
	inline CString& Name(){ return m_szName; }

	// 게임 오브젝트를 파괴합니다.
	inline void Destroy() { m_bIsDestroy = true; }

	// 파괴해야 할 게임 오브젝트인지 여부를 반환합니다.
	inline bool IsDestroy() { return m_bIsDestroy; }

	void Awake();

	// 업데이트
	void Update();

	// 렌더링
	void Render();

	// 디바이스가 변경되었을 때 (전체화면 같은)
	void ResetDevice();

	// 활성화 여부를 설정합니다.
	void SetActive(bool bActive);

	// 활성화 여부를 반환합니다.
	inline bool IsActive() { return m_bIsActive; }

	// 부모 노드를 설정합니다.
	void SetParent(GameObject* pParent);

	// 부모 노드를 반환합니다.
	inline GameObject* GetParent() { return m_pParent; }

	// map에 담겨있는 모든 게임 오브젝트를 파괴합니다.
	static void CleanUpGameObject();

	// 게임 오브젝트를 파괴합니다.
	static void Destroy(GameObject* pGO);

	// 해당 이름의 게임 오브젝트를 찾습니다.
	static GameObject* Find(CString szName);

	// 해당 이름으로 모든 게임 오브젝트를 찾아서 리스트로 반환합니다.
	static list<GameObject*> FindAll(CString szName);

public:
	// 최적화 필요
	// public말고 처리방안 필요
	// 관련 함수들과 함께 Scene쪽으로 옮길것, 현재 GameObject 안에 있는 변수들 사용하기 위해 여기에 존재?
	// 해당 씬 마다 mapGameObject가 있어야 할 듯, 현재 모든 씬에 다 들어감
	static std::multimap<CString, GameObject*> m_mapGameObjects; // 중복 키 값 허용을 위해 multimap 사용
	ComTransform* transform;
	bool IsAlwaysRender;	// 항상 렌더링 해야하는 객체
	bool IsInFrustum;		// 프러스텀 안에 있는지 여부

private:
	// 이름
	CString m_szName;

	// 구성요소
	std::map<CString, Component*> m_Components;

	// 부모 노드
	GameObject* m_pParent;

	// 월드 행렬
	//Matrix4x4 m_matWorld;

	// 활성화 여부
	bool m_bIsActive;

	// 파괴될 오브젝트 (예약개념, Component.Update()에서 파괴되면 에러 나므로)
	bool m_bIsDestroy;
	
	// 충돌 박스
	ComCollider* m_pCollider;

	list<GameObject*> m_listChild;
};
