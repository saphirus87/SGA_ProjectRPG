#include "stdafx.h"
#include "ComChrEquipment.h"
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
	m_pRender= (ComEquipment*)pGOEquipment->GetComponent("ComEquipment");
	m_pAnimation = (ComRenderSkinnedMesh*)m_pGOParent->GetComponent("ComRenderSkinnedMesh");
}

void RenderEquipment::Redner()
{
	Matrix4x4* matFrame = m_pAnimation->GetMatrixByName(szFrameName);
	
	if (matFrame != NULL)
		m_pRender->Render(matFrame, &m_pGOParent->transform->GetWorldMatrix());
}

ComChrEquipment::ComChrEquipment(CString szName) :
	Component(szName)
{
	m_vecEquipedItems.resize(eItem_COUNT);
	for (int i = eItem_None; i < eItem_COUNT; ++i)
		m_vecEquipedItems[i] = NULL;
}

ComChrEquipment::~ComChrEquipment()
{
	// 장착된 아이템 메모리 해제
	for (size_t i = 0; i < m_vecEquipedItems.size(); ++i)
		SAFE_DELETE(m_vecEquipedItems[i]);

	// 장착된 아이템 렌더링 메모리 해제
	for (size_t i = 0; i < m_vecRenderEquipments.size(); ++i)
		SAFE_DELETE(m_vecRenderEquipments[i]);
}

void ComChrEquipment::Awake()
{
	m_vecRenderEquipments.resize(eEquipment_Count);
}

void ComChrEquipment::Update()
{

}

void ComChrEquipment::Render()
{
	for (auto & equipment : m_vecRenderEquipments)
		if (equipment != NULL)
			equipment->Redner();
}

void ComChrEquipment::SetOffsetPos(eEquipment type, Vector3 vOffsetPos)
{
	m_vecRenderEquipments[type]->m_pGOEquipment->transform->SetPosition(vOffsetPos);
	
	switch (type)
	{
	case eEquipment_ShoulderR:
		vOffsetPos.y *= -1; // 위치 반전
		m_vecRenderEquipments[eEquipment_ShoulderL]->m_pGOEquipment->transform->SetPosition(vOffsetPos);
		break;
	}
}

void ComChrEquipment::ChangeTexture(eEquipment type, CString szItemName)
{
	m_vecRenderEquipments[type]->m_pRender->ChangeTexture(0, szItemName);

	switch (type)
	{
	case eEquipment_ShoulderR:
		m_vecRenderEquipments[eEquipment_ShoulderL]->m_pRender->ChangeTexture(0, szItemName);
		break;
	}
}

void ComChrEquipment::Equip(ItemInfo * pItem)
{
	m_vecEquipedItems[pItem->Type] = pItem;

	// 렌더링 객체를 추가합니다.
	switch (pItem->Type)
	{
	case eItem_Shoulder:
	{
		// 방어구 어깨 오른쪽
		GameObject* pGOShoulderR = factory.CreateEquipment(pItem, Vector3(3, 10, -8));
		pGOShoulderR->transform->SetScale(100, 100, 100);
		((ComEquipment*)pGOShoulderR->GetComponent("ComEquipment"))->IsEquiped = true;

		RenderEquipment * pRenderEquipmentR = new RenderEquipment();
		pRenderEquipmentR->Set("Shoulder_Right", gameObject, pGOShoulderR);
		m_vecRenderEquipments[eEquipment_ShoulderR] = pRenderEquipmentR;

		// 방어구 어깨 왼쪽
		GameObject* pGOShoulderL = factory.CreateEquipment(pItem, Vector3(3, -10, -8), true);
		((ComEquipment*)pGOShoulderL->GetComponent("ComEquipment"))->IsEquiped = true;
		pGOShoulderL->transform->SetScale(100, -100, 100);

		RenderEquipment * pRenderEquipmentL = new RenderEquipment();
		pRenderEquipmentL->Set("Shoulder_Left", gameObject, pGOShoulderL);
		m_vecRenderEquipments[eEquipment_ShoulderL] = pRenderEquipmentL;

		switch (pItem->ChrType)
		{
		case eChrType_Troll:
			SetOffsetPos(eEquipment_ShoulderR, Vector3(3, 12, -6)); // [z, x, y축]
			break;
		}
	}
	break;

	case eItem_Helmet:
	{
		// 방어구 투구
		RenderEquipment * pRenderEquipment = new RenderEquipment();
		GameObject* pGOHelmet = factory.CreateEquipment("Equipment_Helmet", "Resources/character/Equipment/", "Helmet_01.X", Vector3(0, 0, 0));
		((ComEquipment*)pGOHelmet->GetComponent("ComEquipment"))->IsEquiped = true;
		pGOHelmet->transform->SetRotation(Vector3(D3DXToRadian(90), 0, 0));
		pRenderEquipment->Set("Helmet", gameObject, pGOHelmet);
		m_vecRenderEquipments[eEquipment_Helmet] = pRenderEquipment;
	}
	break;

	case eItem_WeaponR:
	{
		// 무기 오른손
		RenderEquipment* pRenderEquipment = new RenderEquipment();
		GameObject* pGOWeaponR = factory.CreateEquipment("Equipment_weapon", "Resources/character/Equipment/", "Sword_01.X", Vector3(0, 0, -6)); // 보정위치 y축 아래로 조금 내림
		((ComEquipment*)pGOWeaponR->GetComponent("ComEquipment"))->IsEquiped = true;																										 // 무기 칼날 아래 방향으로 돌려 잡음
		pGOWeaponR->transform->SetRotation(Vector3(D3DXToRadian(90), 0, 0));
		pRenderEquipment->Set("Weapon_Right", gameObject, pGOWeaponR);
		m_vecRenderEquipments[eEquipment_WeaponR] = pRenderEquipment;
	}
	break;

	case eItem_WeaponL:
	{
		// 무기 왼손
		RenderEquipment* pRenderEquipment = new RenderEquipment();
		GameObject* pGOWeaponL = factory.CreateEquipment("Equipment_weapon", "Resources/character/Equipment/", "Sword_01.X", Vector3(0, 0, -6)); // 보정위치 y축 아래로 조금 내림
		((ComEquipment*)pGOWeaponL->GetComponent("ComEquipment"))->IsEquiped = true;
		pGOWeaponL->transform->SetRotation(Vector3(D3DXToRadian(90), 0, 0));
		pRenderEquipment->Set("Weapon_Left", gameObject, pGOWeaponL);
		m_vecRenderEquipments[eEquipment_WeaponL] = pRenderEquipment;
	}
	break;

	case eItem_Shield:
	{
		// 방어구 방패 왼손
		RenderEquipment* pRenderEquipment = new RenderEquipment();
		GameObject* pGOShield = factory.CreateEquipment("Equipment_Shield", "Resources/character/Equipment/", "Shield_01.X", Vector3(0, -5, 0));
		((ComEquipment*)pGOShield->GetComponent("ComEquipment"))->IsEquiped = true;
		pGOShield->transform->SetRotation(Vector3(D3DXToRadian(90), D3DXToRadian(-90), 0));
		pRenderEquipment->Set("Shield_Left", gameObject, pGOShield); // 보정위치 팔 밖쪽으로 조금
		m_vecRenderEquipments[eEquipment_Shield] = pRenderEquipment;
	}
	break;
	}
}