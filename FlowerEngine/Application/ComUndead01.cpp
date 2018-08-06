#include "stdafx.h"
#include "ComUndead01.h"
#include "ComObjMap.h"

ComUndead01::ComUndead01(CString szName)
	:Component(szName)
{
}

ComUndead01::~ComUndead01()
{
}

void ComUndead01::Awake()
{
	//CPP ´ÙÇü¼º
	m_pAnimation = (ComRenderSkinnedMesh*)gameObject->GetComponent("ComRenderSkinnedMesh");
	m_pMap = (ComObjMap*)GameObject::Find("ObjMap")->GetComponent("ComObjMap");
}

void ComUndead01::Update()
{
	Vector3 pos = gameObject->transform->GetPosition();
	float fHeight = 0.0f;

	if (m_pMap->GetHeight(fHeight, pos))
	{
		pos.y = fHeight;
		gameObject->transform->SetPosition(pos);
	}
	
	//Test Code
	if (Input::KeyDown('1'))
	{
		m_pAnimation->PlayAnimation(eAni_Stand);
	}
	if (Input::KeyDown('2'))
	{
		m_pAnimation->PlayAnimation(eAni_Walk);
	}
}

void ComUndead01::Render()
{
}
