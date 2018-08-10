#include "stdafx.h"
#include "ComEquipment.h"
#include "ItemInfo.h"

RenderEquipment::RenderEquipment() :
	m_pGOEquipment(NULL),
	m_pRender(NULL),
	m_vOffsetPos(Vector3(0, 0, 0))
{
}

RenderEquipment::~RenderEquipment()
{
}

void RenderEquipment::Set(LPCSTR szName, GameObject* pGOParent, GameObject * pGOEquipment)
{
	szFrameName = szName;
	m_pGOParent = pGOParent;
	m_pGOEquipment = pGOEquipment;
	m_pRender= (ComRenderXMesh*)pGOEquipment->GetComponent("ComRenderXMesh");
	m_pAnimation = (ComRenderSkinnedMesh*)m_pGOParent->GetComponent("ComRenderSkinnedMesh");
}

void RenderEquipment::Update()
{
	Matrix4x4* matFrame = m_pAnimation->GetMatrixByName(szFrameName);
	
	if (matFrame != NULL)
		m_pRender->SetFrameMatrix(matFrame, &m_pGOParent->transform->GetWorldMatrix());
}

ComEquipment::ComEquipment(CString szName) :
	Component(szName),
	pDataSholder(NULL),
	m_pRenderRight(NULL),
	m_pRenderLeft(NULL),
	m_pAnimation(NULL),
	m_pGOShoulderRight(NULL),
	m_pGOShoulderLeft(NULL),
	m_pGOHelmet(NULL),
	m_pRenderHelmet(NULL)
{
}

ComEquipment::~ComEquipment()
{
	SAFE_DELETE(pDataSholder);

	for (size_t i = 0; i < m_vecRenderEquipments.size(); ++i)
		SAFE_DELETE(m_vecRenderEquipments[i]);
}

void ComEquipment::Awake()
{
	FactoryGameObject factory;

	pDataSholder = new EquipmentShoulder();
	
	m_mapTextureName.insert(map<CString, CString>::value_type("ShoulderEquipItemName01", "Resources/character/Equipment/shoulder_plate_d_02copper.png"));
	m_mapTextureName.insert(map<CString, CString>::value_type("ShoulderEquipItemName02", "Resources/character/Equipment/shoulder_robe_b_03blue.png"));

	m_pAnimation = (ComRenderSkinnedMesh*)gameObject->GetComponent("ComRenderSkinnedMesh");

	m_vecRenderEquipments.resize(eEquipment_Count);

	RenderEquipment* pRenderEquipment = new RenderEquipment();
	pRenderEquipment->Set("Shoulder_Right", gameObject, factory.CreateEquipment("Equipment_shoulder", "Resources/character/Equipment/", "shoulder_01.X", Vector3(3, 10, -8)));
	m_vecRenderEquipments[eEquipment_ShoulderR] = pRenderEquipment;

	pRenderEquipment = new RenderEquipment();
	pRenderEquipment->Set("Shoulder_Left", gameObject, factory.CreateEquipment("Equipment_shoulder", "Resources/character/Equipment/", "shoulder_01.X", Vector3(3, -10, -8), true));
	m_vecRenderEquipments[eEquipment_ShoulderL] = pRenderEquipment;

	pRenderEquipment = new RenderEquipment();
	GameObject* pGOHelmet = factory.CreateEquipment("Equipment_Helmet", "Resources/character/Equipment/", "Helmet_01.X", Vector3(0, 0, 0));
	pGOHelmet->transform->SetRotation(Vector3(D3DXToRadian(90), 0, 0));
	pRenderEquipment->Set("Helmet", gameObject, pGOHelmet);
	m_vecRenderEquipments[eEquipment_Helmet] = pRenderEquipment;
}

void ComEquipment::Update()
{
	for (auto & equipment : m_vecRenderEquipments)
		if (equipment != NULL)
			equipment->Update();
}

void ComEquipment::Render()
{
}

void ComEquipment::SetOffsetPos(Vector3 vOffsetPosR)
{
	m_vecRenderEquipments[eEquipment_ShoulderR]->m_pGOEquipment->transform->SetPosition(vOffsetPosR);
	vOffsetPosR.y *= -1;
	m_vecRenderEquipments[eEquipment_ShoulderL]->m_pGOEquipment->transform->SetPosition(vOffsetPosR);
}

void ComEquipment::ChangeTexture(CString szItemName)
{
	m_vecRenderEquipments[eEquipment_ShoulderR]->m_pRender->ChangeTexture(0, m_mapTextureName[szItemName]);
	m_vecRenderEquipments[eEquipment_ShoulderL]->m_pRender->ChangeTexture(0, m_mapTextureName[szItemName]);
}
