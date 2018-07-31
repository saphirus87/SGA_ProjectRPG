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

	GameObject* pEquip = GameObject::Find("Equipment_shoulder");
	//static_cast<ComTransform*>(pEquip->GetComponent("ComTransform"))->IsAutoUpdate = false;
		
	Matrix4x4 pFindMatrix;
	D3DXMatrixIdentity(&pFindMatrix);
	m_pAnimation->GetMatrixByName("character_scourge_male_scourgemale_hd_bone_140", &pFindMatrix);
	if (pFindMatrix != NULL)
	{
		static_cast<ComXMesh*>(pEquip->GetComponent("ComXMesh"))->matWorld = pFindMatrix;
	}
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
		m_pAnimation->PlayAnimation(0);
	}
	if (Input::KeyDown('2'))
	{
		m_pAnimation->PlayAnimation(1);
	}
}

void ComUndead01::Render()
{
}
