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


	GameObject* pEquip = GameObject::Find("Equipment_shoulder");
	//static_cast<ComTransform*>(pEquip->GetComponent("ComTransform"))->IsAutoUpdate = false;

	Matrix4x4 pFindMatrix;
	D3DXMatrixIdentity(&pFindMatrix);
	m_pAnimation->GetMatrixByName("character_scourge_male_scourgemale_hd_bone_140", &pFindMatrix);
	if (pFindMatrix != NULL)
	{
		static_cast<ComRenderXMesh*>(pEquip->GetComponent("ComRenderXMesh"))->matWorld = pFindMatrix;
	}
}

void ComHuman01::Update()
{
	if (Input::KeyDown('1'))
		m_pAnimation->PlayAnimation(eAni_Stand);
	
	if (Input::KeyDown('2'))
		m_pAnimation->PlayAnimation(eAni_Walk);

	Vector3 pos = gameObject->transform->GetPosition();
	float fHeight = 0.f;
	if (m_pMap->GetHeight(fHeight, pos) == true)
	{
		pos.y = fHeight;
		gameObject->transform->SetPosition(pos);
	}
}

void ComHuman01::Render()
{
}
