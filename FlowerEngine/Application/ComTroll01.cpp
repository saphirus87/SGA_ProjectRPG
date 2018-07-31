#include "stdafx.h"
#include "ComTroll01.h"
#include "ComObjMap.h"

ComTroll01::ComTroll01(CString szName)
	:Component(szName)
{
}

ComTroll01::~ComTroll01()
{
}

void ComTroll01::Awake()
{
	//CPP ´ÙÇü¼º
	m_pAnimation = (ComRenderSkinnedMesh*)gameObject->GetComponent("ComRenderSkinnedMesh");
	m_pMap = (ComObjMap*)GameObject::Find("ObjMap")->GetComponent("ComObjMap");
}

void ComTroll01::Update()
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
		m_pAnimation->PlayAnimation(0);
	}
	if (Input::KeyDown('2'))
	{
		m_pAnimation->PlayAnimation(1);
	}
}

void ComTroll01::Render()
{
}
