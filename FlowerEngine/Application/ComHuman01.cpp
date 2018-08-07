#include "stdafx.h"
#include "ComHuman01.h"
#include "ComObjMap.h"

ComHuman01::ComHuman01(CString szName) : 
	Component(szName),
	m_pSholderRight(NULL),
	m_pSholderLeft(NULL)
{
	D3DXMatrixIdentity(&m_matSholderRight);
	D3DXMatrixIdentity(&m_matSholderLeft);
}


ComHuman01::~ComHuman01()
{
}

void ComHuman01::Awake()
{
	m_pAnimation = (ComRenderSkinnedMesh*)gameObject->GetComponent("ComRenderSkinnedMesh");
	GameObject* pObjMap = GameObject::Find("ObjMap");
	m_pMap = (ComObjMap*)pObjMap->GetComponent("ComObjMap");

	// ¾î±ú ·»´õ¸µ ÄÄÆ÷³ÍÆ® ¹Ì¸® Ã£¾ÆµÒ
	GameObject* pEquipShouldRight = GameObject::Find("Equipment_shoulder_Right");
	if (pEquipShouldRight)
		m_pSholderRight = (ComRenderXMesh*)pEquipShouldRight->GetComponent("ComRenderXMesh");
	GameObject* pEquipShouldLeft = GameObject::Find("Equipment_shoulder_Left");
	if (pEquipShouldLeft)
		m_pSholderLeft = (ComRenderXMesh*)pEquipShouldLeft->GetComponent("ComRenderXMesh");
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

	// ¾î±ú ¹æ¾î±¸
	m_matSholderRight = m_pAnimation->GetMatrixByName("Shoulder_Right");
	m_matSholderLeft = m_pAnimation->GetMatrixByName("Shoulder_Left");
	m_pSholderRight->SetFrameMatrix(&m_matSholderRight, &gameObject->transform->GetWorldMatrix());
	m_pSholderLeft->SetFrameMatrix(&m_matSholderLeft, &gameObject->transform->GetWorldMatrix());
}

void ComHuman01::Render()
{
}
