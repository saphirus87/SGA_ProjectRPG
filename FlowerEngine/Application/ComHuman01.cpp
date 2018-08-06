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

	GameObject* pEquip = GameObject::Find("Equipment_shoulder");

	Matrix4x4 pFindMatrix;
	//pFindMatrix = m_pAnimation->GetMatrixByName("character_human_male_humanmale_hd_bone_47"); // 47 ¿À¸¥¼Õ
	pFindMatrix = m_pAnimation->GetMatrixByName("character_human_male_humanmale_hd_bone_28"); // ¿ÞÂÊ ¾î±ú

	if (pFindMatrix != NULL)
	{
		ComRenderXMesh* pRenderXMesh = static_cast<ComRenderXMesh*>(pEquip->GetComponent("Shoulder_Left"));
		pRenderXMesh->SetFrameMatrix(&pFindMatrix, &gameObject->transform->GetWorldMatrix());
	}

	pFindMatrix = m_pAnimation->GetMatrixByName("character_human_male_humanmale_hd_bone_88"); // ¿À¸¥ÂÊ ¾î±ú ?

	if (pFindMatrix != NULL)
	{
		ComRenderXMesh* pRenderXMesh = static_cast<ComRenderXMesh*>(pEquip->GetComponent("Shoulder_Right"));
		pRenderXMesh->SetFrameMatrix(&pFindMatrix, &gameObject->transform->GetWorldMatrix());
	}

}

void ComHuman01::Render()
{
}
