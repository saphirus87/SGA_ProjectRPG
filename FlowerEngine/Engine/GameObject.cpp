#include "StdAfx.h"
#include "GameObject.h"

std::multimap<CString, GameObject*> GameObject::m_mapGameObjects;

GameObject::GameObject(CString szName) :
	m_szName(szName),
	m_pParent(NULL),
	m_bIsActive(true),
	m_pCollider(NULL),
	m_bIsDestroy(false),
	IsAlwaysRender(false),
	Tag(-1)
{
	transform = new ComTransform("ComTransform");
	AddComponent(transform);
	m_mapGameObjects.insert(MGOPair(szName, this));
}

GameObject::~GameObject(void)
{
	CleanUpComponent();
}

void GameObject::AddComponent(Component* pComponent)
{
	if (pComponent->Name() == "ComCollider")
	{
		m_pCollider = (ComCollider*)pComponent;
		Component::m_vecColliders.push_back(m_pCollider);
	}

	m_Components.insert(MComPair(pComponent->Name(), pComponent));
	pComponent->gameObject = this;
	
	// ���� ���� AddComponent �Ǹ�
	if (pComponent->IsAwaked == false)
	{
		pComponent->Awake();
		pComponent->IsAwaked = true;
	}
}

void GameObject::RemoveComponent(CString & szName)
{
	// ���� �̱���
	return;

	std::map<CString, Component*>::iterator iter = m_Components.find(szName);
	if (iter->second != NULL)
	{
		m_Components.erase(iter);
		//SAFE_DELETE(iter->second);
	}
}

void GameObject::AddChild(GameObject * pGO)
{
	m_listChild.push_back(pGO);
}

void GameObject::CleanUpComponent()
{
	for (IterGOCom iter = m_Components.begin(); iter != m_Components.end(); ++iter)
		SAFE_DELETE((*iter).second);
	m_Components.clear();
}

void GameObject::Awake()
{
	for (auto & com : m_Components)
	{
		if (com.second->IsAwaked == false)
		{
			com.second->Awake();
			com.second->IsAwaked = true;
		}
	}
}

void GameObject::Update()
{
	if (m_bIsActive == false)
		return;
	
	////////////////////////////////////////////////////////////////////////////////////////////////////// 
	// �浹 �˻� START -> Scene���� �̵�?
	if (m_pCollider)
		m_pCollider->oriPos = transform->GetPosition();
	
	for (int i = 0; i < Component::m_vecColliders.size(); ++i)
	{
		// �浹��ü�� �� ��ü��
		if (((ComCollider*)Component::m_vecColliders[i]) == m_pCollider)
			continue;

		if (m_pCollider != NULL && m_pCollider->IsIntersected(*Component::m_vecColliders[i]) == true)
		{
			for (IterGOCom iter = m_Components.begin(); iter != m_Components.end(); ++iter)
			{
				// �浹 ��밡 Active ����������
				if (Component::m_vecColliders[i]->gameObject->IsActive())
					(*iter).second->OnTriggerEnter(*Component::m_vecColliders[i]);
			}
			break;
		}
	}
	// �浹 �˻� END
	////////////////////////////////////////////////////////////////////////////////////////////////////// 

	for (auto & com : m_Components)
	{
		if (com.second->Enable == true)
			com.second->Update();
	}
}

void GameObject::Render()
{
	if (m_bIsActive == false)
		return;

	for (auto & com : m_Components)
	{
		if (com.second->Enable == true)
			com.second->Render();
	}
}

void GameObject::ResetDevice()
{
	for (auto & com : m_Components)
		com.second->ResetDevice();
}

void GameObject::SetActive(bool bActive)
{
	m_bIsActive = bActive;
	for (auto & go : m_listChild)
		go->SetActive(bActive);
}

void GameObject::SetParent(GameObject * pParent)
{
	if (pParent == NULL)
		return;
	m_pParent = pParent;
	m_pParent->AddChild(this);
}

void GameObject::CleanUpGameObject()
{
	for (auto & go : m_mapGameObjects)
		SAFE_DELETE(go.second);

	m_mapGameObjects.clear();
}

void GameObject::Destroy(GameObject* pGO)
{
	// Collider �ڷᱸ������ ���ְ�
	std::vector<ComCollider*>::iterator iter;
	for (iter = Component::m_vecColliders.begin(); iter != Component::m_vecColliders.end(); ++iter)
	{
		if ((*iter) == pGO->GetComponent("ComCollider"))
		{
			Component::m_vecColliders.erase(iter);
			break;
		}
	}

	SAFE_DELETE(pGO);
}

list<GameObject*> GameObject::FindAll(CString szName)
{
	list<GameObject*> listGO;
	
	for (IterGO iter = m_mapGameObjects.begin(); iter != m_mapGameObjects.end(); ++iter)
	{
		GameObject* pGO = (*iter).second;

		if (pGO != NULL && pGO->Name().Find(szName) >= 0)
			listGO.push_back(pGO);
	}

	return listGO;
}

list<GameObject*> GameObject::FindAll(int iTag)
{
	list<GameObject*> listGO;

	for (IterGO iter = m_mapGameObjects.begin(); iter != m_mapGameObjects.end(); ++iter)
	{
		GameObject* pGO = (*iter).second;

		if (pGO != NULL && pGO->Tag == iTag)
			listGO.push_back(pGO);
	}

	return listGO;
}

GameObject * GameObject::Find(CString szName)
{
	IterGO iter = m_mapGameObjects.find(szName);
	if (iter != m_mapGameObjects.end())
		return iter->second;
	else
		return NULL;
}

Component* GameObject::GetComponent(const CString & szName)
{
	IterGOCom iter = m_Components.find(szName);
	if (iter != m_Components.end())
		return iter->second;
	else
		return NULL;
}