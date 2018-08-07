#include "stdafx.h"
#include "ComEquipmentShoulder.h"
#include "ItemInfo.h"

ComEquipmentShoulder::ComEquipmentShoulder(CString szName) :
	Component(szName),
	pDataSholder(NULL),
	m_pSholderRight(NULL),
	m_pSholderLeft(NULL),
	m_pAnimation(NULL)
{
	D3DXMatrixIdentity(&m_matSholderRight);
	D3DXMatrixIdentity(&m_matSholderLeft);
}


ComEquipmentShoulder::~ComEquipmentShoulder()
{
	SAFE_DELETE(pDataSholder);
}

void ComEquipmentShoulder::Awake()
{
	pDataSholder = new EquipmentShoulder();

	m_pAnimation = (ComRenderSkinnedMesh*)gameObject->GetComponent("ComRenderSkinnedMesh");

	// ¾î±ú ·»´õ¸µ ÄÄÆ÷³ÍÆ® ¹Ì¸® Ã£¾ÆµÒ
	GameObject* pEquipShouldRight = GameObject::Find("Equipment_shoulder_Right");
	if (pEquipShouldRight)
		m_pSholderRight = (ComRenderXMesh*)pEquipShouldRight->GetComponent("ComRenderXMesh");
	GameObject* pEquipShouldLeft = GameObject::Find("Equipment_shoulder_Left");
	if (pEquipShouldLeft)
		m_pSholderLeft = (ComRenderXMesh*)pEquipShouldLeft->GetComponent("ComRenderXMesh");
}

void ComEquipmentShoulder::Update()
{
	// ¾î±ú ¹æ¾î±¸
	m_matSholderRight = m_pAnimation->GetMatrixByName("Shoulder_Right");
	m_matSholderLeft = m_pAnimation->GetMatrixByName("Shoulder_Left");
	m_pSholderRight->SetFrameMatrix(&m_matSholderRight, &gameObject->transform->GetWorldMatrix());
	m_pSholderLeft->SetFrameMatrix(&m_matSholderLeft, &gameObject->transform->GetWorldMatrix());
}

void ComEquipmentShoulder::Render()
{
}
