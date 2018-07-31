#include "stdafx.h"
#include "ComHuman01.h"
#include "ComObjMap.h"

ComHuman01::ComHuman01(CString szName) : 
	Component(szName)
{
}


ComHuman01::~ComHuman01()
{
}

void ComHuman01::Awake()
{
	m_pAnimation = (ComRenderSkinnedMesh*)gameObject->GetComponent("ComRenderSkinnedMesh");
	GameObject* pObjMap = GameObject::Find("ObjMap");
	m_pMap = (ComObjMap*)pObjMap->GetComponent("ComObjMap");
}

void ComHuman01::Update()
{
	if (Input::KeyDown('1'))
		m_pAnimation->PlayAnimation(eAni_Stand);
	
	if (Input::KeyDown('2'))
		m_pAnimation->PlayAnimation(eAni_Walk);

	Vector3 pos = gameObject->transform->GetPosition();
	float fHeight = 0.f;
	pos.y = m_pMap->GetHeight(fHeight, pos);
	gameObject->transform->SetPosition(pos);
}

void ComHuman01::Render()
{
}
