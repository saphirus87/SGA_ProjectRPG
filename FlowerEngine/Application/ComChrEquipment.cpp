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
	for (int i = eItem_Shoulder; i < eItem_COUNT; ++i)
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
	m_vecRenderEquipments.resize(eRenderEquipment_Count);
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

void ComChrEquipment::SetOffsetPos(eRenderEquipment type, Vector3 vOffsetPos)
{
	m_vecRenderEquipments[type]->m_pGOEquipment->transform->SetPosition(vOffsetPos);
	
	switch (type)
	{
	case eRenderEquipment_ShoulderR:
		vOffsetPos.y *= -1; // 위치 반전
		m_vecRenderEquipments[eRenderEquipment_ShoulderL]->m_pGOEquipment->transform->SetPosition(vOffsetPos);
		break;
	}
}

void ComChrEquipment::ChangeTexture(eRenderEquipment type, CString szItemName)
{
	m_vecRenderEquipments[type]->m_pRender->ChangeTexture(0, szItemName);

	switch (type)
	{
	case eRenderEquipment_ShoulderR:
		m_vecRenderEquipments[eRenderEquipment_ShoulderL]->m_pRender->ChangeTexture(0, szItemName);
		break;
	}
}

void ComChrEquipment::Equip(ItemInfo * pItem)
{
	m_vecEquipedItems[pItem->Type] = pItem;

	LPCSTR szShoulder_Right = GetFrameName(pItem, eRenderEquipment_ShoulderR);
	LPCSTR szShoulder_Left = GetFrameName(pItem, eRenderEquipment_ShoulderL);
	LPCSTR szHelmet = GetFrameName(pItem, eRenderEquipment_Helmet);
	LPCSTR szShield = GetFrameName(pItem, eRenderEquipment_Shield);
	LPCSTR szWeapon_Right = GetFrameName(pItem, eRenderEquipment_WeaponR);
	LPCSTR szWeapon_Left = GetFrameName(pItem, eRenderEquipment_WeaponL);

	GameObject* pGOEquipment = factory.CreateEquipment(pItem, Vector3(0, 0, 0));
	pGOEquipment->transform->SetScale(100, 100, 100);
	((ComEquipment*)pGOEquipment->GetComponent("ComEquipment"))->IsEquiped = true;

	RenderEquipment * pRenderEquipment = new RenderEquipment();

	// 렌더링 객체를 추가합니다.
	switch (pItem->Type)
	{
	case eItem_Shoulder:
	{
		// 방어구 어깨 오른쪽
		pGOEquipment->transform->SetPosition(3, 10, -8);

		pRenderEquipment->Set(szShoulder_Right, gameObject, pGOEquipment);
		m_vecRenderEquipments[eRenderEquipment_ShoulderR] = pRenderEquipment;

		// 방어구 어깨 왼쪽
		GameObject* pGOShoulderL = factory.CreateEquipment(pItem, Vector3(3, -10, -8), true);
		((ComEquipment*)pGOShoulderL->GetComponent("ComEquipment"))->IsEquiped = true;
		pGOShoulderL->transform->SetScale(100, -100, 100);

		RenderEquipment * pRenderEquipmentL = new RenderEquipment();
		pRenderEquipmentL->Set(szShoulder_Left, gameObject, pGOShoulderL);
		m_vecRenderEquipments[eRenderEquipment_ShoulderL] = pRenderEquipmentL;

		switch (pItem->ChrType)
		{
		case eChrType_Troll:
			SetOffsetPos(eRenderEquipment_ShoulderR, Vector3(3, 12, -6)); // [z, x, y축]
			break;
		}
	}
	break;

	case eItem_Helmet:
	{
		// 방어구 투구
		pGOEquipment->transform->SetRotation(Vector3(D3DXToRadian(90), 0, 0));
		pRenderEquipment->Set(szHelmet, gameObject, pGOEquipment);
		m_vecRenderEquipments[eRenderEquipment_Helmet] = pRenderEquipment;
	}
	break;

	case eItem_WeaponR:
	{
		// 무기 오른손
		pGOEquipment->transform->SetPosition(0, 0, -6); // 보정위치 y축 아래로 조금 내림

		pGOEquipment->transform->SetRotation(Vector3(D3DXToRadian(90), 0, 0));
		pRenderEquipment->Set(szWeapon_Right, gameObject, pGOEquipment);
		m_vecRenderEquipments[eRenderEquipment_WeaponR] = pRenderEquipment;
	}
	break;

	case eItem_WeaponL:
	{
		// 무기 왼손
		pGOEquipment->transform->SetPosition(0, 0, -6); // 보정위치 y축 아래로 조금 내림

		pGOEquipment->transform->SetRotation(Vector3(D3DXToRadian(90), 0, 0));
		pRenderEquipment->Set(szWeapon_Left, gameObject, pGOEquipment);
		m_vecRenderEquipments[eRenderEquipment_WeaponL] = pRenderEquipment;
	}
	break;

	case eItem_Shield:
	{
		// 방어구 방패 왼손
		pGOEquipment->transform->SetPosition(0, -5, 0); // 보정위치 y축 아래로 조금 내림

		pGOEquipment->transform->SetRotation(Vector3(D3DXToRadian(90), D3DXToRadian(-90), 0));
		pRenderEquipment->Set(szShield, gameObject, pGOEquipment); // 보정위치 팔 밖쪽으로 조금
		m_vecRenderEquipments[eRenderEquipment_Shield] = pRenderEquipment;
	}
	break;
	}
}

LPCSTR ComChrEquipment::GetFrameName(ItemInfo * itemInfo, eRenderEquipment renderType)
{
	switch (itemInfo->ChrType)
	{
	case eChrType_Human:
		switch (renderType)
		{
		case eRenderEquipment_ShoulderR:
			return "character_human_male_humanmale_hd_bone_28";
		case eRenderEquipment_ShoulderL:
			return "character_human_male_humanmale_hd_bone_27";
		case eRenderEquipment_Helmet:
			return "character_human_male_humanmale_hd_bone_39";
		case eRenderEquipment_Shield:
			return "";
		case eRenderEquipment_WeaponR:
			return "character_human_male_humanmale_hd_bone_47";
		case eRenderEquipment_WeaponL:
			return "character_human_male_humanmale_hd_bone_41";
		}

	case eChrType_Troll:
		switch (renderType)
		{
		case eRenderEquipment_ShoulderR:
			return "character_troll_male_trollmale_hd_bone_34";
		case eRenderEquipment_ShoulderL:
			return "character_troll_male_trollmale_hd_bone_33";
		case eRenderEquipment_Helmet:
			return "character_troll_male_trollmale_hd_bone_46";
		case eRenderEquipment_Shield:
			return "character_troll_male_trollmale_hd_bone_49";
		case eRenderEquipment_WeaponR:
			return "character_troll_male_trollmale_hd_bone_53";
		case eRenderEquipment_WeaponL:
			return "character_troll_male_trollmale_hd_bone_48";
		}

	case eChrType_Undead:
		switch (renderType)
		{
		case eRenderEquipment_ShoulderR:
			return "Shoulder_Right";
		case eRenderEquipment_ShoulderL:
			return "Shoulder_Left";
		case eRenderEquipment_Helmet:
			return "Helmet";
		case eRenderEquipment_Shield:
			return "Shield_Left";
		case eRenderEquipment_WeaponR:
			return "Weapon_Right";
		case eRenderEquipment_WeaponL:
			return "Weapon_Left";
		}

		// 예전것
	default:
		switch (renderType)
		{
		case eRenderEquipment_ShoulderR:
			return "Shoulder_Right";
		case eRenderEquipment_ShoulderL:
			return "Shoulder_Left";
		case eRenderEquipment_Helmet:
			return "Helmet";
		case eRenderEquipment_Shield:
			return "Shield_Left";
		case eRenderEquipment_WeaponR:
			return "Weapon_Right";
		case eRenderEquipment_WeaponL:
			return "Weapon_Left";
		}
		break;
	}

	return "";
}
