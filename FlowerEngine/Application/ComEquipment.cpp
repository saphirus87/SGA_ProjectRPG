#include "stdafx.h"
#include "ComEquipment.h"
#include "ComRenderEquipment.h"
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
	m_pRender= (ComRenderEquipment*)pGOEquipment->GetComponent("ComRenderXMesh");
	m_pAnimation = (ComRenderSkinnedMesh*)m_pGOParent->GetComponent("ComRenderSkinnedMesh");
}

void RenderEquipment::Redner()
{
	Matrix4x4* matFrame = m_pAnimation->GetMatrixByName(szFrameName);
	
	if (matFrame != NULL)
		m_pRender->Render(matFrame, &m_pGOParent->transform->GetWorldMatrix());
}

ComEquipment::ComEquipment(CString szName) :
	Component(szName),
	pDataSholder(NULL)
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

	m_vecRenderEquipments.resize(eEquipment_Count);

	// 방어구 어깨 오른쪽
	RenderEquipment* pRenderEquipment = new RenderEquipment();
	pRenderEquipment->Set("Shoulder_Right", gameObject, factory.CreateEquipment("Equipment_shoulder", "Resources/character/Equipment/", "shoulder_01.X", Vector3(3, 10, -8)));
	m_vecRenderEquipments[eEquipment_ShoulderR] = pRenderEquipment;

	// 방어구 어깨 왼쪽
	pRenderEquipment = new RenderEquipment();
	pRenderEquipment->Set("Shoulder_Left", gameObject, factory.CreateEquipment("Equipment_shoulder", "Resources/character/Equipment/", "shoulder_01.X", Vector3(3, -10, -8), true));
	m_vecRenderEquipments[eEquipment_ShoulderL] = pRenderEquipment;

	// 방어구 투구
	pRenderEquipment = new RenderEquipment();
	GameObject* pGOHelmet = factory.CreateEquipment("Equipment_Helmet", "Resources/character/Equipment/", "Helmet_01.X", Vector3(0, 0, 0));
	pGOHelmet->transform->SetRotation(Vector3(D3DXToRadian(90), 0, 0));
	pRenderEquipment->Set("Helmet", gameObject, pGOHelmet);
	m_vecRenderEquipments[eEquipment_Helmet] = pRenderEquipment;

	// 무기 오른손
	pRenderEquipment = new RenderEquipment();
	GameObject* pGOWeaponR = factory.CreateEquipment("Equipment_weapon", "Resources/character/Equipment/", "Sword_01.X", Vector3(0, 0, -6)); // 보정위치 y축 아래로 조금 내림
	// 무기 칼날 아래 방향으로 돌려 잡음
	pGOWeaponR->transform->SetRotation(Vector3(D3DXToRadian(90), 0, 0));
	pRenderEquipment->Set("Weapon_Right", gameObject, pGOWeaponR);
	m_vecRenderEquipments[eEquipment_WeaponR] = pRenderEquipment;

	// 무기 왼손
	pRenderEquipment = new RenderEquipment();
	GameObject* pGOWeaponL = factory.CreateEquipment("Equipment_weapon", "Resources/character/Equipment/", "Sword_01.X", Vector3(0, 0, -6)); // 보정위치 y축 아래로 조금 내림
	pGOWeaponL->transform->SetRotation(Vector3(D3DXToRadian(90), 0, 0));
	pRenderEquipment->Set("Weapon_Left", gameObject, pGOWeaponR);
	m_vecRenderEquipments[eEquipment_WeaponL] = pRenderEquipment;

	// 방어구 방패 왼손
	pRenderEquipment = new RenderEquipment();
	GameObject* pGOShield = factory.CreateEquipment("Equipment_Shield", "Resources/character/Equipment/", "Shield_01.X", Vector3(0, -5, 0));
	pGOShield->transform->SetRotation(Vector3(D3DXToRadian(90), D3DXToRadian(-90), 0));
	pRenderEquipment->Set("Shield_Left", gameObject, pGOShield); // 보정위치 팔 밖쪽으로 조금
	m_vecRenderEquipments[eEquipment_Shield] = pRenderEquipment;
}

void ComEquipment::Update()
{

}

void ComEquipment::Render()
{
	for (auto & equipment : m_vecRenderEquipments)
		if (equipment != NULL)
			equipment->Redner();
}

void ComEquipment::SetOffsetPos(Vector3 vOffsetPosR)
{
	m_vecRenderEquipments[eEquipment_ShoulderR]->m_pGOEquipment->transform->SetPosition(vOffsetPosR);
	vOffsetPosR.y *= -1;
	m_vecRenderEquipments[eEquipment_ShoulderL]->m_pGOEquipment->transform->SetPosition(vOffsetPosR);
}

void ComEquipment::ChangeTexture(eEquipment type, CString szItemName)
{
	m_vecRenderEquipments[type]->m_pRender->ChangeTexture(0, m_mapTextureName[szItemName]);
}
