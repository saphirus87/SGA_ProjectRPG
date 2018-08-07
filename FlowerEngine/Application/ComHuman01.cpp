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

	//static_cast<ComTransform*>(pEquip->GetComponent("ComTransform"))->IsAutoUpdate = false;
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

	GameObject* pEquipShouldRight = GameObject::Find("Equipment_shoulder_Right");
	Matrix4x4 pFindMatrix;
	pFindMatrix = m_pAnimation->GetMatrixByName("Shoulder_Right");

	if (pFindMatrix != NULL)
	{
		ComRenderXMesh* pRenderXMesh = static_cast<ComRenderXMesh*>(pEquipShouldRight->GetComponent("ComRenderXMesh"));
		pRenderXMesh->SetFrameMatrix(&pFindMatrix, &gameObject->transform->GetWorldMatrix());
	}

	GameObject* pEquipShouldLeft = GameObject::Find("Equipment_shoulder_Left");
	pFindMatrix = m_pAnimation->GetMatrixByName("Shoulder_Left");

	if (pFindMatrix != NULL)
	{
		ComRenderXMesh* pRenderXMesh = static_cast<ComRenderXMesh*>(pEquipShouldLeft->GetComponent("ComRenderXMesh"));
		pRenderXMesh->SetFrameMatrix(&pFindMatrix, &gameObject->transform->GetWorldMatrix());
	}

}

void ComHuman01::Render()
{
}
