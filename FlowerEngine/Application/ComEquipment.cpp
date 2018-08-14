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
	m_pRender= (ComRenderEquipment*)pGOEquipment->GetComponent("ComRenderEquipment");
	m_pAnimation = (ComRenderSkinnedMesh*)m_pGOParent->GetComponent("ComRenderSkinnedMesh");
}

void RenderEquipment::Redner()
{
	Matrix4x4* matFrame = m_pAnimation->GetMatrixByName(szFrameName);
	
	if (matFrame != NULL)
		m_pRender->Render(matFrame, &m_pGOParent->transform->GetWorldMatrix());
}

ComEquipment::ComEquipment(CString szName) :
	Component(szName)
{
	m_vecEquipedItems.resize(eItem_COUNT);
	for (int i = eItem_None; i < eItem_COUNT; ++i)
		m_vecEquipedItems[i] = NULL;
}

ComEquipment::~ComEquipment()
{
	// 장착된 아이템 메모리 해제
	for (size_t i = 0; i < m_vecEquipedItems.size(); ++i)
		SAFE_DELETE(m_vecEquipedItems[i]);

	// 장착된 아이템 렌더링 메모리 해제
	for (size_t i = 0; i < m_vecRenderEquipments.size(); ++i)
		SAFE_DELETE(m_vecRenderEquipments[i]);
}

void ComEquipment::Awake()
{
	m_vecRenderEquipments.resize(eEquipment_Count);
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

void ComEquipment::SetOffsetPos(eEquipment type, Vector3 vOffsetPos)
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

void ComEquipment::ChangeTexture(eEquipment type, CString szItemName)
{
	m_vecRenderEquipments[type]->m_pRender->ChangeTexture(0, szItemName);

	switch (type)
	{
	case eEquipment_ShoulderR:
		m_vecRenderEquipments[eEquipment_ShoulderL]->m_pRender->ChangeTexture(0, szItemName);
		break;
	}
}

void ComEquipment::Equip(ItemInfo * pItem)
{
	m_vecEquipedItems[pItem->Type] = pItem;

	FactoryGameObject factory;
	// 렌더링 객체를 추가합니다.
	switch (pItem->Type)
	{
	case eItem_Shoulder:
	{
		// 방어구 어깨 오른쪽
		RenderEquipment * pRenderEquipmentR = new RenderEquipment();
		GameObject* pGOShoulderR = factory.CreateEquipment(pItem, Vector3(3, -10, -8), true);
		((ComRenderEquipment*)pGOShoulderR->GetComponent("ComRenderEquipment"))->IsEquiped = true;
		pGOShoulderR->transform->SetPosition(3, 10, -8);
		pGOShoulderR->transform->SetScale(100, 100, 100);
		pRenderEquipmentR->Set("Shoulder_Right", gameObject, pGOShoulderR);
		m_vecRenderEquipments[eEquipment_ShoulderR] = pRenderEquipmentR;

		// 방어구 어깨 왼쪽
		RenderEquipment * pRenderEquipmentL = new RenderEquipment();
		GameObject* pGOShoulderL = factory.CreateEquipment(pItem, Vector3(3, -10, -8), true);
		pGOShoulderL->transform->SetScale(100, -100, 100);
		pRenderEquipmentL->Set("Shoulder_Left", gameObject, pGOShoulderL);
		((ComRenderEquipment*)pGOShoulderL->GetComponent("ComRenderEquipment"))->IsEquiped = true;
		m_vecRenderEquipments[eEquipment_ShoulderL] = pRenderEquipmentL;

		switch (pItem->ChrType)
		{
		case eChrType_Troll:
			SetOffsetPos(eEquipment_ShoulderR, Vector3(3, 12, -6)); // [z, x, y축]
			//ChangeTexture(eEquipment_ShoulderR, "ShoulderEquipItemName02");
			break;
		}
	}
	break;

	case eItem_Helmet:
	{
		// 방어구 투구
		RenderEquipment * pRenderEquipment = new RenderEquipment();
		GameObject* pGOHelmet = factory.CreateEquipment("Equipment_Helmet", "Resources/character/Equipment/", "Helmet_01.X", Vector3(0, 0, 0));
		((ComRenderEquipment*)pGOHelmet->GetComponent("ComRenderEquipment"))->IsEquiped = true;
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
		((ComRenderEquipment*)pGOWeaponR->GetComponent("ComRenderEquipment"))->IsEquiped = true;																										 // 무기 칼날 아래 방향으로 돌려 잡음
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
		((ComRenderEquipment*)pGOWeaponL->GetComponent("ComRenderEquipment"))->IsEquiped = true;
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
		((ComRenderEquipment*)pGOShield->GetComponent("ComRenderEquipment"))->IsEquiped = true;
		pGOShield->transform->SetRotation(Vector3(D3DXToRadian(90), D3DXToRadian(-90), 0));
		pRenderEquipment->Set("Shield_Left", gameObject, pGOShield); // 보정위치 팔 밖쪽으로 조금
		m_vecRenderEquipments[eEquipment_Shield] = pRenderEquipment;
	}
	break;
	}
}

void ComEquipment::GetEquip(GameObject* pGOEquipment)
{
	ComRenderEquipment* pComRenderEquipment = (ComRenderEquipment*)pGOEquipment->GetComponent("ComRenderEquipment");
	ItemInfo* pItem = pComRenderEquipment->pItemInfo;;
	m_vecEquipedItems[pItem->Type] = pItem;
	
	FactoryGameObject factory;
	// 렌더링 객체를 추가합니다.
	switch (pItem->Type)
	{
	case eItem_Shoulder:
	{
		// 방어구 어깨 오른쪽
		RenderEquipment * pRenderEquipmentR = new RenderEquipment();
		((ComRenderEquipment*)pGOEquipment->GetComponent("ComRenderEquipment"))->IsEquiped = true;
		pGOEquipment->transform->SetPosition(3, 10, -8);
		pGOEquipment->transform->SetScale(100, 100, 100);
		pRenderEquipmentR->Set("Shoulder_Right", gameObject, pGOEquipment);
		m_vecRenderEquipments[eEquipment_ShoulderR] = pRenderEquipmentR;

		// 방어구 어깨 왼쪽
		RenderEquipment * pRenderEquipmentL = new RenderEquipment();
		GameObject* pGOShoulderL = factory.CreateEquipment(pItem, Vector3(3, -10, -8), true);
		pGOShoulderL->transform->SetScale(100, -100, 100);
		pRenderEquipmentL->Set("Shoulder_Left", gameObject, pGOShoulderL);
		((ComRenderEquipment*)pGOShoulderL->GetComponent("ComRenderEquipment"))->IsEquiped = true;
		m_vecRenderEquipments[eEquipment_ShoulderL] = pRenderEquipmentL;

		switch (pItem->ChrType)
		{
		case eChrType_Troll:
			SetOffsetPos(eEquipment_ShoulderR, Vector3(3, 12, -6)); // [z, x, y축]
			ChangeTexture(eEquipment_ShoulderR, "ShoulderEquipItemName02");
			break;
		}
	}
	break;

	case eItem_Helmet:
	{
		// 방어구 투구
		RenderEquipment * pRenderEquipment = new RenderEquipment();
		GameObject* pGOHelmet = factory.CreateEquipment("Equipment_Helmet", "Resources/character/Equipment/", "Helmet_01.X", Vector3(0, 0, 0));
		((ComRenderEquipment*)pGOHelmet->GetComponent("ComRenderEquipment"))->IsEquiped = true;
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
		((ComRenderEquipment*)pGOWeaponR->GetComponent("ComRenderEquipment"))->IsEquiped = true;																										 // 무기 칼날 아래 방향으로 돌려 잡음
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
		((ComRenderEquipment*)pGOWeaponL->GetComponent("ComRenderEquipment"))->IsEquiped = true;
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
		((ComRenderEquipment*)pGOShield->GetComponent("ComRenderEquipment"))->IsEquiped = true;
		pGOShield->transform->SetRotation(Vector3(D3DXToRadian(90), D3DXToRadian(-90), 0));
		pRenderEquipment->Set("Shield_Left", gameObject, pGOShield); // 보정위치 팔 밖쪽으로 조금
		m_vecRenderEquipments[eEquipment_Shield] = pRenderEquipment;
	}
	break;
	}
}
