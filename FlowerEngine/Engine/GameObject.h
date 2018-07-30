/*
������
H.P		: 010-3592-5921
E-Mail	: hkn10004@naver.com

NOTE :
�� �ڵ�� ���� �����ϰ� �ۼ��� �ڵ��Դϴ�. �̷��� GAME PROGRAMMING GEMS 6���̳� ���ͳ��� �����ϼ���.

���� :
�츮�� ���ǿ��� �� �� �ִ� ��� ����ü�� ��ü ��, ��ü(Object)�� ���ӻ󿡼� ǥ���� ��.
�׷��Ƿ� ���� ��ü(GameObject)��� �Ѵ�.

���� ��� ���ǿ��� ����(Terrain)�� ���ӻ󿡼� ǥ���ϸ� GameObject�� �ǰ� �� �̸��� Terrain�� �ȴ�.
���� ��� ���ǿ��� �ι�(Character)�� ���ӻ󿡼� ǥ���Ը� GameObject�� �ǰ� �� �̸��� Character�̴�.
����(Item) �Ǵ� ��Ź, ����, ������, ����, ��, ���ѵ� ��� ��ü�� GameObject�� ���� �� �ִ�.

��� ���� ��ü(GameObject)���� �������(Component)�� �ִ�.

��, �ι�(Chracter) ���� ��ü�� ��Ʈ�� �������(CharacterControl Component)�� �߰�(Add)�ϸ� �� �ι�(Character)�� ��Ʈ�� �� �� �ִ�.

�ڼ��Ѱ� ���� å�̳� ���ͳ��� �����ϼ���. �����ڵ鳢�� �Բ� ����鼭 �����ص� �˴ϴ�.
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

	// ������Ҹ� �߰��մϴ�.
	void AddComponent(Component* pComponent);
	void RemoveComponent(CString& szName);

	void AddChild(GameObject* pGO);

	// ������Ҹ� ��ȯ�մϴ�.
	Component* GetComponent(const CString& familyID);
	
	// ��� ������Ҹ� �ı��մϴ�.
	void CleanUpComponent();
	
	// �̸�
	inline CString& Name(){ return m_szName; }

	// ���� ������Ʈ�� �ı��մϴ�.
	inline void Destroy() { m_bIsDestroy = true; }

	// �ı��ؾ� �� ���� ������Ʈ���� ���θ� ��ȯ�մϴ�.
	inline bool IsDestroy() { return m_bIsDestroy; }

	void Awake();

	// ������Ʈ
	void Update();

	// ������
	void Render();

	// ����̽��� ����Ǿ��� �� (��üȭ�� ����)
	void ResetDevice();

	// Ȱ��ȭ ���θ� �����մϴ�.
	void SetActive(bool bActive);

	// Ȱ��ȭ ���θ� ��ȯ�մϴ�.
	inline bool IsActive() { return m_bIsActive; }

	// �θ� ��带 �����մϴ�.
	void SetParent(GameObject* pParent);

	// �θ� ��带 ��ȯ�մϴ�.
	inline GameObject* GetParent() { return m_pParent; }

	// map�� ����ִ� ��� ���� ������Ʈ�� �ı��մϴ�.
	static void CleanUpGameObject();

	// ���� ������Ʈ�� �ı��մϴ�.
	static void Destroy(GameObject* pGO);

	// �ش� �̸��� ���� ������Ʈ�� ã���ϴ�.
	static GameObject* Find(CString szName);

	// �ش� �̸����� ��� ���� ������Ʈ�� ã�Ƽ� ����Ʈ�� ��ȯ�մϴ�.
	static list<GameObject*> FindAll(CString szName);

public:
	// ����ȭ �ʿ�
	// public���� ó����� �ʿ�
	// ���� �Լ���� �Բ� Scene������ �ű��, ���� GameObject �ȿ� �ִ� ������ ����ϱ� ���� ���⿡ ����?
	// �ش� �� ���� mapGameObject�� �־�� �� ��, ���� ��� ���� �� ��
	static std::multimap<CString, GameObject*> m_mapGameObjects; // �ߺ� Ű �� ����� ���� multimap ���
	ComTransform* transform;
	bool IsAlwaysRender;	// �׻� ������ �ؾ��ϴ� ��ü
	bool IsInFrustum;		// �������� �ȿ� �ִ��� ����

private:
	// �̸�
	CString m_szName;

	// �������
	std::map<CString, Component*> m_Components;

	// �θ� ���
	GameObject* m_pParent;

	// ���� ���
	//Matrix4x4 m_matWorld;

	// Ȱ��ȭ ����
	bool m_bIsActive;

	// �ı��� ������Ʈ (���ళ��, Component.Update()���� �ı��Ǹ� ���� ���Ƿ�)
	bool m_bIsDestroy;
	
	// �浹 �ڽ�
	ComCollider* m_pCollider;

	list<GameObject*> m_listChild;
};
